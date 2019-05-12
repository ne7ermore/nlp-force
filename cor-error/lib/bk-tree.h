#pragma once

#include <string>
#include <unordermap>
#include <vector>

#include "utils.h"

namespace Bkt {
    struct Node {
        Node() : value() {};
        Node(std::string wstr, float score) : value(wstr), score(score) {};
        float score;
        std::wstring value;
        std::unordermap<int, Node *> leafs;
    }

    class Tree {
        public:
            Tree() : root() { 
                root = new Node;
                fn = utils::levenshtein_distance;
             };
            void add(std::string);
            std::pair<std::wstring, float> get(std::string, int);
            int fn(std::wstring, std::wstring);
            Node* root();
            void read_file(std::string);

            ~Tree();
            void destroy(Node *)

        private:
            float pow_num = 0.5, min_score = 100;
            Node * root;
    }
} //namespace Bkt