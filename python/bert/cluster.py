import numpy as np
from numpy.linalg import norm

from sklearn.neighbors import KDTree
from sklearn.cluster import MiniBatchKMeans

import collections
import re


def find(x, seen, graph, all_cluster):
    if x not in seen:
        if graph[x] != -1:
            seen.append(x)
            find(graph[x], seen, graph, all_cluster)
            graph[x] = -1

        elif len(seen) != 0:
            for c in all_cluster:
                if x in c:
                    c.extend(seen)
                    seen.clear()
                    break


def get_kmeans(n_clusters, vectors):
    return MiniBatchKMeans(n_clusters=n_clusters).fit(vectors)


def cosine(x, y):
    return abs(np.dot(x, y) / (norm(x)*norm(y)))


def merge_centers(kmeans, cluster_throd):
    nlen = kmeans.cluster_centers_.shape[0]
    scores, vecs, graph = np.zeros((nlen, nlen)), [], [-1] * nlen

    for row in range(nlen):
        for col in range(nlen):
            if row != col:
                scores[row][col] = cosine(kmeans.cluster_centers_[
                                          row], kmeans.cluster_centers_[col])

        idx = np.argmax(scores[row])
        if scores[row][idx] > cluster_throd:
            graph[row] = idx
        else:
            vecs.append(kmeans.cluster_centers_[row])

    all_cluster = []
    for x in range(nlen):
        cluster = []
        find(x, cluster, graph, all_cluster)
        if len(cluster) != 0:
            all_cluster.append(cluster)
            vecs.append(np.average([kmeans.cluster_centers_[row]
                                    for row in cluster], axis=0))

    return np.array(vecs)


def gen_kdt(vecs):
    return KDTree(vecs, leaf_size=5, metric="euclidean")


def cluster(texts, all_vecs, kdtree, vecs):
    ret = collections.defaultdict(list)
    for idx, text in enumerate(texts):
        [distances], [points] = kdtree.query(np.array([all_vecs[idx]]), k=10)

        best_id = best_score = -1
        for label_idx in points:
            score = cosine(vecs[label_idx], all_vecs[idx])
            if score > best_score:
                best_score = score
                best_id = label_idx

        if best_score > 0.95:
            ret[best_id].append(text)

    return ret


if __name__ == "__main__":
    import pandas as pd
    all_vecs = np.squeeze(np.load("../data/vecs.npy"))
    texts = [line.strip() for line in open(
        "../data/query.txt", encoding="utf8")]

    kmeans = get_kmeans(100, all_vecs)
    vecs = merge_centers(kmeans, .96)
    kdt = gen_kdt(vecs)
    ret = cluster(texts, all_vecs, kdt, vecs)

    writer = pd.ExcelWriter('周杰伦.xlsx')
    for idx, texts in ret.items():
        if idx == -1 or len(texts) < 10:
            continue
        pd.DataFrame(texts).to_excel(writer, f"{idx}")
    writer.save()
