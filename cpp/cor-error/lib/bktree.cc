#include <string>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <queue>
#include <stack>
#include <algorithm>
#include <stdio.h> 
#include <fstream>
#include <numeric>
#include <iostream>
#include <cmath>

#include "bktree.h"
#include "utils.h"

namespace BkTree {
    bool sort_by_score(const std::pair<float, std::wstring> &a, const std::pair<float, std::wstring> &b) {
        return (a.first > b.first);
    } 

    std::wstring node::get_item() const {
        return item;
    }

    void node::set_item(std::wstring& _item) {
        item = _item;
    }

    float node::get_hot() const {
        return hot;
    }

    void node::set_hot(const float &h) {
        hot = h;
    }

    int tree::edit_distance(const std::wstring& wstr1, const std::wstring& wstr2) {
        int m = wstr1.size(), n = wstr2.size(), dp[m + 1], i, j, prev, tmp;
        for (i = 0; i <= m; i++) dp[i] = i;
        for (i = 1; i <= n; i++) {
            prev = i-1;
            dp[0] = i;
            for (j = 1; j <= m; j++) {
                tmp = dp[j];
                if (wstr1[j - 1] == wstr2[i - 1]) dp[j] = prev;
                else dp[j] = std::min(std::min(dp[j], prev), dp[j - 1]) + 1;
                prev = tmp;
            }
        }

        return dp[m];          
    }

    tree::~tree() {
        tree::destroy(root);
    }

    void tree::destroy(node* n) {
        if (n->get_item().empty()) return;

        n->get_item().clear();
        for (auto c:n->children) tree::destroy(c.second);
    }

    void tree::add(std::string str, float hot) {
        node *n = root;
        std::wstring wstr = utils::str2wstr(str);
        if (root->get_item().empty()) {
            root->set_item(wstr); 
            root->set_hot(hot);
        }
        else while (1) {
            int d = edit_distance(n->get_item(), wstr);
            if (d == 0) break;
            auto search = n->children.find(d);
            if (search != n->children.end()) n = search->second;
            else {
                n->children[d] = new node(wstr, hot);
                break;
            }
        }
    }

    std::vector<std::pair<float, std::wstring>> tree::search(std::string item, int value) {
        std::vector<std::pair<float, std::wstring>> res;
        if (root->get_item().empty()) return res;
        int d, idx;
        node *n;
        std::queue<node *> q;
        q.push(root);
        std::wstring wstr = utils::str2wstr(item);
        float score = 0, _score;
        while (q.size()) {
            n = q.front();
            q.pop();
            
            d = edit_distance(n->get_item(), wstr);
            _score = pow(pow_count, d)*n->get_hot();
            if (d <= value && score < _score) {
                score = _score;
                res.push_back(make_pair(_score, n->get_item()));
            }

            for (idx = std::max(1, d-value); idx <= d+value; idx++) if (n->children.count(idx)) if (pow_count*n->children[idx]->get_hot() > score) q.push(n->children[idx]);
        }

        if (res.size()) std::sort(res.begin(), res.end(), sort_by_score); 

        return res;        
    }  

    int tree::levenshtein_distance(const std::string &str1, const std::string &str2) {
        std::wstring wstr1 = utils::str2wstr(str1), wstr2 = utils::str2wstr(str2);
        return edit_distance(wstr1, wstr2);
    }

    int tree::longest_common_seq(const std::string &str1, const std::string &str2) {
        int len1 = str1.size(), len2 = str2.size(), dp[len1 + 1], i, j;
        for (i = 0; i <= len1; i++) dp[i] = 0;
        for (j = 1; j <= len2; j++) for (i = 1; i <= len1; i++) {
            if (str1[i - 1] == str2[j - 1]) dp[i] = dp[i-1] + 1;
            else dp[i] = std::max(dp[i-1], dp[i]);
        }
        return dp[len1];           
    }
} // namespace BkTree