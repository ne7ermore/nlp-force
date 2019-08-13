import codecs
import gzip
import sys
import pickle
import numpy as np

def is_chinese_char(c):
    if ((c >= 0x4E00 and c <= 0x9FFF) or
            (c >= 0x3400 and c <= 0x4DBF) or
            (c >= 0x20000 and c <= 0x2A6DF) or
            (c >= 0x2A700 and c <= 0x2B73F) or
            (c >= 0x2B740 and c <= 0x2B81F) or
            (c >= 0x2B820 and c <= 0x2CEAF) or
            (c >= 0xF900 and c <= 0xFAFF) or
            (c >= 0x2F800 and c <= 0x2FA1F)):
        return True
    return False

def uutf8(s, from_enc='utf8'):
    if isinstance(s, bytes):
        return s
    return s.encode(from_enc)

def rstr(s, from_enc='utf8', to_enc='utf8'):
    if isinstance(s, bytes):
        return s.decode(to_enc)
    if from_enc == to_enc:
        return s
    return uutf8(s, from_enc).decode(to_enc)

def sopen(filename, mode='rb', enc='utf8', errors='strict'):
    readMode = 'r' in mode
    if readMode and 'w' in mode:
        raise("Must be either read mode or write, but not both")

    elif filename.endswith('.gz'):
        stream = gzip.GzipFile(filename, mode)
    elif filename == '-':
        if readMode:
            stream = sys.stdin
        else:
            stream = sys.stdout
    else:
        stream = open(filename, mode)

    if enc not in (None, 'byte'):
        if readMode:
            return codecs.getreader(enc)(stream, errors)
        else:
            return codecs.getwriter(enc)(stream, errors)
    return stream

def load_pre_w2c(inf, only_cn=True):
    word2idx = {}
    vecs = []

    for line in open(inf):
        temp = line.strip().split(" ")
        word = temp[0]
        weights = temp[1:]

        if only_cn:
            for c in word:
                if is_chinese_char(ord(c)):
                    break
            else:
                continue

        if len(weights) != 200:
            continue

        word2idx[temp[0]] = len(word2idx)
        vecs.append(list(map(float, temp[1:])))

    vecs = np.array(vecs, dtype=np.float32)
    return word2idx, vecs

def load_corp_vec(inf):
    cor_file, npy_file = inf
    word2idx = {word.strip(): idx for idx, word in enumerate(open(cor_file, encoding="utf8"))}
    vecs =  np.load(npy_file)

    return word2idx, vecs