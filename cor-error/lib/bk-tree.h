#include <string>
#include <unordermap>
#include <vector>


namespace Bkt {
    struct Node {
        Node() : value() {};
        Node(std::string in) : value(in) {};

        std::string value;
        std::unordermap<int, Node *> leafs;
    }

    class Tree {
        public:
            Tree() : root() {};
            ~Tree();
            void add(std::string);
            std::vector search(std::string, int);
            int fn(std::string, std::string);
        private:
            Node * root;
    }
} //namespace Bkt