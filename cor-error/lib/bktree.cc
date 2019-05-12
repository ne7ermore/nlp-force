#include <string>
#include <algorithm>
#include <queue>
#include <complex>
#include <fstream>
#include <cstdio>
 
#include "bktree.h"

namespace Bkt {
    int Tree::levenshtein_distance(std::wstring s1, std::wstring s2) {
        int n = s1.size(), m = s2.size();
        int dp[n + 1][m + 1], i, j;
        for (i = 0; i <= n; i++) dp[i][0] = i;
        for (i = 0; i <= m; i++) dp[0][i] = i;
        for (i = 1; i <= n; i++) for (j = 1; j <= m; j++)
        {
            if (s1[i - 1] == s2[j - 1]) dp[i][j] = dp[i - 1][j - 1];
            else dp[i][j] = std::min(std::min(dp[i - 1][j], dp[i - 1][j - 1]), dp[i][j - 1]) + 1;
        }
        return dp[n][m];        
    }

    void Tree::add(std::string s, float score) {
        std::wstring ws = utils::str2wstr(s);
        
        Node *n = root;
        int dist;
        if (root->value.empty()) {
            root->value = ws;
            root->score = score;
        } else while (1) {
            dist = levenshtein_distance(n->value, ws);
            auto idx = n->leafs.find(dist);
            if (idx != n->leafs.end()) n = idx->second;
            else {
                n->leafs[dist] = new Node(ws, score);
                break;
            }
        }
        return;
    }

    std::pair<std::wstring, float> Tree::get(std::string s, int score) {
        std::pair<std::wstring, float> result;
        if (root->value.empty()) return result;
        
        std::queue<Node *> q;
        q.push(root);

        std::wstring ws = utils::str2wstr(s);

        int dist, idx;
        float p1, p2 = 0;
        Node* n;

        while (q.size()) {
            n = q.front();
            q.pop();

            dist = levenshtein_distance(n->value, ws);
            p1 = std::pow(pow_num, dist)*n->score;
            if (dist <= score && p2 < p1) {
                p2 = p1;
                result = make_pair(n->value, p2);
            }

            for (auto leaf: n->leafs) if (leaf.first >= (dist-score) && leaf.first <= (dist+score)) q.push(leaf.second);
        }
        return result;
    }

    void Tree::read_file(std::string inf) {
        std::ifstream fin(inf);
        if (!fin.is_open()) {
            printf("ERROR: %s can`t be opened", inf.c_str());
            exit(EXIT_FAILURE);
        }
        std::vector<std::pair<float, std::string>> items;
        std::string s;
        std::vector<std::string> v;
        float fnum;
        unsigned int _min = INT_MAX, _max = 0;

        while(getline(fin, s)) {
            v.clear();
            utils::split_string(s, v, "\t");
            if (v.size() != 2) {
                printf("ERROR: %s - format is wrong", inf.c_str());
                exit(EXIT_FAILURE);                
            }
            fnum = std::stod(v[1]);
            if (fnum < min_score) continue;
            items.push_back(make_pair(fnum, v[0]));
            
            _min = _min > fnum ? fnum : _min;
            _max = _max < fnum ? fnum : _max;
        }

        sort(items.begin(), items.end(), utils::pair_sort);
        for (auto item: items) add(item.second, (item.first - _min)/_max);
    }

    Tree::~Tree() {
        Tree::destroy(root);
    }

    void Tree::destroy(Node* n) {
        if (n->value.empty()) return;

        n->value.clear();
        for (auto leaf: n->leafs) destroy(leaf.second);
    }
} //namespace Bkt