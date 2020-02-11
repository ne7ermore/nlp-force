import collections
import re

import elasticsearch
from pypinyin import lazy_pinyin


class LyricsSearch(object):
    def __init__(self, index_name="lyrics", index_type="lyrics_word", port=9200, passwd=None, host="127.0.0.1"):
        self.index_name = index_name
        self.index_type = index_type

        if passwd:
            self.es = elasticsearch.Elasticsearch(
                [host], http_auth=('elastic', passwd), port=port)
        else:
            self.es = elasticsearch.Elasticsearch([host], port=port)

        self.init_mappings()

        rules = open("temp")
        self.rules = {}
        for line in rules:
            rule, label = line.strip().split(" ")
            self.rules[rule] = label
        rules.close()

    def delete(self):
        self.es.indices.delete(index=self.index_name)

    def init_mappings(self):
        chinese_field_config = {
            "type": "text",
            "analyzer": "whitespace",
            "search_analyzer": "whitespace"
        }

        white_space_config = {
            "type": "text",
            "analyzer": "ik_max_word",
            "search_analyzer": "ik_max_word"
        }

        mapping = {
            self.index_type: {
                "_all": {"enabled": False},

                "properties": {
                    "content": chinese_field_config,
                    "pinyin": white_space_config
                }
            }
        }

        if not self.es.indices.exists(index=self.index_name):
            self.es.indices.create(index=self.index_name)
            self.es.indices.put_mapping(
                index=self.index_name, doc_type=self.index_type, body=mapping)

    def insert_data(self, content):
        self.es.index(index=self.index_name, doc_type=self.index_type, body={
            "content": content,
            "pinyin": " ".join(lazy_pinyin(content))
        })

    def search(self, query, search_offset, search_size):
        res = self.slot(query)
        if res is None:
            return None

        slot, label = res
        es_search_options = self.set_search_optional(slot)
        es_result = self.get_search_result(
            es_search_options, search_offset, search_size)
        final_result = self.get_result_list(es_result)

        return final_result

    def slot(self, content):
        for rule, label in self.rules.items():
            res = re.findall(rule, content)
            if len(res):
                return (res[0], label)
        return None

    def get_result_list(self, es_result):
        result_items = es_result['hits']['hits']
        final_result = []
        for item in result_items:
            final_result.append((item["_score"], item["_source"]["content"]))
        return final_result

    def get_search_result(self, es_search_options, search_offset, search_size):
        return self.es.search(
            index=self.index_name,
            doc_type=self.index_type,
            body=es_search_options,
            from_=search_offset,
            size=search_size)

    def set_search_optional(self, query):
        es_search_options = {
            "query": {
                "match": {
                    "content": {
                        "query": query
                    },
                },
                "match": {
                    "pinyin": {
                        "query": " ".join(lazy_pinyin(query))
                    }
                }
            }
        }
        return es_search_options


if __name__ == '__main__':
    # LyricsSearch().delete()
    es = LyricsSearch()

    # content_ls = [
    #     u"把该说的话好好说假如把犯得起的错应该还来得及去悔过那一场小风波将一笑带过",
    #     u"有多少你觉得不能够被懂的痛只能沉默有多少夜晚没有尽头",
    #     u"幸福的样子我感觉好真实有一种踏实当你口中喊我名字",
    #     u"修炼爱情的心酸学会放好以前的渴望"
    # ]

    # for index, content in enumerate(content_ls):
    #     es.insert_data(content)

    final_results = LyricsSearch().search("这句修炼爱情的心酸是哪首歌里面的", 0, 30)
    print(final_results)
