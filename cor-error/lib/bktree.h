#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "utils.h"

namespace Bkt {
    struct Node {
        Node() : value() {};
        Node(std::wstring wstr, float score) : value(wstr), score(score) {};
        float score;
        std::wstring value;
        std::unordered_map<int, Node *> leafs;
    };

    class Tree {
        public:
            Tree() : root() { root = new Node; };
            
            void add(std::string, float score);
            void read_file(std::string);

            int levenshtein_distance(std::wstring, std::wstring);
            
            std::pair<std::wstring, float> get(std::string, int);
            
            ~Tree();
            void destroy(Node *);

        private:
            float pow_num = 0.5, min_score = 100;
            Node * root;
    };
} //namespace Bkt