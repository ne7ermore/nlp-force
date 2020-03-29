import collections


class Score(collections.namedtuple("Score", ["precision", "recall", "fmeasure"])):
    pass


def _fmeasure(precision, recall):
    if precision + recall > 0:
        return 2 * precision * recall / (precision + recall)
    else:
        return 0.0


def _score_lcs(target_tokens, prediction_tokens):

    if not target_tokens or not prediction_tokens:
        return Score(precision=0, recall=0, fmeasure=0)

    # Compute length of LCS from the bottom up in a table (DP appproach).
    lcs_table = _lcs_table(target_tokens, prediction_tokens)
    lcs_length = lcs_table[-1][-1]

    precision = lcs_length / len(prediction_tokens)
    recall = lcs_length / len(target_tokens)
    fmeasure = _fmeasure(precision, recall)

    return Score(precision=precision, recall=recall, fmeasure=fmeasure)


def _lcs_table(ref, can):
    rows, cols = len(ref), len(can)

    lcs_table = [[0] * (cols + 1) for _ in range(rows + 1)]
    for i in range(1, rows + 1):
        for j in range(1, cols + 1):
            if ref[i - 1] == can[j - 1]:
                lcs_table[i][j] = lcs_table[i - 1][j - 1] + 1
            else:
                lcs_table[i][j] = max(lcs_table[i - 1][j], lcs_table[i][j - 1])
    return lcs_table


def _backtrack_norec(t, ref, can):
    i, j = len(ref), len(can)

    lcs = []
    while i > 0 and j > 0:
        if ref[i - 1] == can[j - 1]:
            lcs.insert(0, i-1)
            i -= 1
            j -= 1
        elif t[i][j - 1] > t[i - 1][j]:
            j -= 1
        else:
            i -= 1
    return lcs


def _create_ngrams(tokens, n):
    ngrams = collections.Counter()
    for ngram in (tuple(tokens[i:i + n]) for i in range(len(tokens) - n + 1)):
        ngrams[ngram] += 1
    return ngrams
