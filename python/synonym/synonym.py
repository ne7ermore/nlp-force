from segmenter import Jieba
from lru import LRU
from util import *

from sklearn.neighbors import KDTree
import numpy as np
from numpy.linalg import norm

def cosine(x, y):
    return abs(np.dot(x,y) / (norm(x)*norm(y)))

class Synonyms(object):
    def __init__(self, jieba_path, lru_cap):
        self.psg = Jieba(jieba_path)
        self.cache = LRU(lru_cap)

    def load_vecs(self, load_fn, w2vFile):
        """
        params:
            load_fn | function - must return dict && 2 dims numpy.array
        """
        self.word2idx, self.weights = load_fn(w2vFile)
        self.idx2word = {v: k for k, v in self.word2idx.items()}
        print(f"load vector completed, vec size - {self.weights.shape}")

        print("build kd tree")
        self.kdtree = KDTree(self.weights, leaf_size=50, metric = "euclidean")

        self.vec_len = self.weights.shape[1]

    def _word2vec(self, word, ignore_type="random"):
        """
        ignore_type | string  - "zeros" or "random"
        """
        if word in self.word2idx:
            return self.weights[self.word2idx[word]]
        else:
            if ignore_type == "zeros":
                return np.zeros(self.vec_len, dtype=np.float32)
            elif ignore_type == "random":
                return np.random.uniform(size=self.vec_len)
            else:
                raise ValueError(f"invalid ignore_type {ignore_type}, should be zeros or random")

    def _text2vec(self, text, ret_num=5):
        vectors = []
        for w in text:
            c = []
            if w in self.word2idx:
                c.append(self._word2vec(w))
                for nw, _ in self.nearby(w, ret_num=ret_num):
                    c.append(self._word2vec(nw))

                r = np.average(c, axis=0)
                vectors.append(r)

        if len(vectors):
            return np.sum(vectors, axis=0)

        return np.random.uniform(size=self.vec_len)

    def nearby(self, word, ret_num=10, ignore_type="random"):
        word = word.strip()

        # cache
        rets = self.cache.search(word)
        if rets is not None:
            for ret in rets:
                yield ret

        vec = self._word2vec(word, ignore_type)
        [distances], [points] = self.kdtree.query(np.array([vec]), k=ret_num, return_distance=True)
        rets = []
        for (x, y) in zip(points, distances):
            w = self.idx2word[x]
            if w == word: 
                s = 1.0
            else: 
                s = cosine(vec, self.weights[x])
            rets.append((w, min(s, 1.0)))

        rets = list(sorted(rets, key=lambda x: x[1], reverse=True))
        self.cache.add(word, rets)
        for ret in rets:
            yield ret

    def similarity(self, s1, s2, top=5):
        if s1 == s2:
            return 1.

        s1 = self.psg.segment(s1)
        s2 = self.psg.segment(s2)
        
        assert len(s1) > 0 and len(s2) > 0, "The length of s1 and s2 should > 0."

        return cosine(self._text2vec(s1, top), self._text2vec(s2, top))