#include "../include/node.hpp"
class ResultSet;

class b_plus_tree{
    public:
        b_plus_tree(int p);
        void insert_key(int key, void* value);
        void* search_key(int key);
        ResultSet* range(int start, int end);
        bool delete_key(int key);
        int get_depth();
        int get_num_keys();
        node* root;


    private:
        int p;
        int depth;
        int num_keys;

        node* getLeftNeighbhor(node* child, node*parent);
        node* getRightNeighbhor(node* child, node* parent);
        void updateWithInorderSuccessor(node* n, int key);
        node* merge_nodes(key_node* root,bool incl_root);
        

};

class ResultSet{
    public:
        ResultSet();
        void* next();
        void* current();
        // void* prev();
        void setStart(node* n,key_node* k);
        void setEnd(int b);
    private: 
        node* curNode;
        key_node* curKey;
        int end;
};