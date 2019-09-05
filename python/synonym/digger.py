import time

from synonyms import Synonyms
from bloomfilter import BloomFilter
from util import load_corp_vec


class Digger(object):
    def __init__(self, pro_num, ratio, jieba_path="./", lru_cap=5000):
        self.pro_num = pro_num
        self.bf = BloomFilter()
        self.simi = Synonyms(jieba_path, lru_cap)
        self.ratio = ratio

    def gCorpus(self, inf):
        print("reading file...")
        now = time.time()
        corps = set([line.strip() for line in open(inf)])
        self.corps = list(corps)
        print(f"query nums - {len(corps)}, time cost {time.time()-now:.2f}s")

    def prepare(self, inf):
        print("reading query...")
        self.querys = [line.strip() for line in open(inf)]
        self.simi.load_vecs(load_corp_vec, ("data/corpus", "data/vector.npy"))

    def similarity(self, s1, s2):
        if self.simi.similarity(s1, s2) > self.ratio:
            return True
        return False
