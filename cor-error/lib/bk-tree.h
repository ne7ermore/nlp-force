#include <string>
#include <unordermap>
#include <vector>


namespace Bkt {
    struct Node {
        Node() : value() {};
        Node(std::string wstr, float score) : value(wstr), score(score) {};
        float score;
        std::string value;
        std::unordermap<int, Node *> leafs;
    }

    class Tree {
        public:
            Tree() : root() { root = new Node; };
            void add(std::string);
            std::pair<std::wstring, float> get(std::string, int);
            int fn(std::string, std::string);
            Node* root();
            void read_file(std::string);

            ~Tree();
            void destroy(Node *)

        private:
            float pow_num = 0.5, min_score = 100;
            Node * root;
    }
} //namespace Bkt