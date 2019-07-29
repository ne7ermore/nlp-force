#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace BkTree {
    struct node {
        public:
            node() : item() {};
            node(std::wstring wstr, float hot) : item(wstr), hot(hot) {};
            std::unordered_map<int, node *> children;

            std::wstring get_item() const;
            void set_item(std::wstring&);

            float get_hot() const;
            void set_hot(const float&);

        private:
            std::wstring item;
            float hot;        
    };

    class tree {
        public:
            tree() : root() { root = new node; };
            ~tree();
            void add(std::string, float);
            std::vector<std::pair<float, std::wstring>> search(std::string, int);
            int levenshtein_distance(const std::string&, const std::string&);
            int longest_common_seq(const std::string&, const std::string&);
            
        private:
            struct node *root;
            float pow_count = 0.1, threshold;

            int edit_distance(const std::wstring&, const std::wstring&);
            void destroy(node*);
            void read_file(std::string);
    };    
}; // namespace BkTree