#include <string>
#include <algorithm>
#include <queque>
#include <complex>
#include <fstream>
#include <cstdio>
 
#include "bk-tree.h"

namespace Bkt {
    void Tree::add(std::string s, float score) {
        std::wstring ws = utils::str2wstr(s);
        
        node *n = root;
        int dist;
        if (root->value.empty()) {
            root->value = ws;
            root->score = score;
        } else while (1) {
            dist = fn(n->value, ws);
            auto idx = n->leafs.find(d);
            if (idx != n->leafs.end()) n = idx->second;
            else {
                n->leafs[d] = new Node(ws, score);
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

        while (q.size()) {
            n = q.front();
            q.pop();

            dist = fn(n->value, ws);
            p1 = std::pow(pow_num, dist)*n->score
            if (dist <= score && p2 < p1) {
                p2 = p1;
                result = make_pair(n->value, p2);
            }

            for (auto leaf: n->leafs) if (leaf.first >= (dist-score) && leaf.first <= (dist+score)) q.push(leaf.second);
        }
        return result
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
        for (auto item: items) add(item.second, (d.first - _min)/_max);
    }

    Tree::~Tree() {
        Tree::destroy(root);
    }

    void Tree::destroy(Node* n) {
        if (n->value.empty()) return;

        n->value.clear();
        for (auto leaf: n.leafs) tree::destroy(leaf.second);
    }
} //namespace Bkt