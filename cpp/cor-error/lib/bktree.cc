#include <string>
#include <algorithm>
#include <queue>
#include <complex>
#include <fstream>
#include <cstdio>
 
#include "bktree.h"

namespace Bkt {
    int Tree::levenshtein_distance(std::wstring s1, std::wstring s2) {
        int m = s1.size(), n = s2.size(), dp[m + 1], i, j, prev, tmp;
        for (i = 0; i <= m; i++) dp[i] = i;
        for (i = 1; i <= n; i++) {
            prev = i-1;
            dp[0] = i;
            for (j = 1; j <= m; j++) {
                tmp = dp[j];
                if (s1[i - 1] == s2[j - 1]) dp[j] = prev;
                else dp[j] = std::min(std::min(dp[j], prev), dp[j - 1]) + 1;
                prev = tmp;
            }
        }
        return dp[m];        
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
        float fnum, fmin = 100000, fmax = 0.0;

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
            
            fmin = fmin > fnum ? fnum : fmin;
            fmax = fmax < fnum ? fnum : fmax;
        }

        sort(items.begin(), items.end(), utils::pair_sort);
        for (auto item: items) add(item.second, (item.first - fmin)/fmax);
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