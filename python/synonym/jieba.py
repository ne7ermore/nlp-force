import os

import jieba
import jieba.posseg as pseg
from util import sopen, uutf8, rstr

class Jieba(object):
    def __init__(self, default_path, HMM=True):
        self.pseg = pseg
        self.HMM = HMM
        jieba.load_userdict(os.path.join(
            default_path, "segmenter_dicts/default.dict"))

        self.swords = set()
        for line in sopen(os.path.join(default_path, "segmenter_dicts/stopwords")):
            self.swords.add(line.strip())

    def segment(self, text):
        if not text:
            return []
        words = self.pseg.cut(uutf8(text, from_enc='utf8'), HMM=self.HMM)
        return [rstr(w) for w, f in words if rstr(w) not in self.swords]