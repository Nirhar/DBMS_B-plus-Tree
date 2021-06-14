#include <iostream>

#ifndef NULL
    #define NULL 0
#endif

class node;
class tree_pointer_node;
class key_node;

class tree_pointer_node{
    public:
        tree_pointer_node();
        key_node* next;
        key_node* prev;
        node* ptr;
};

class key_node{
    public:
        key_node(int key,void* value);
        tree_pointer_node* next;
        tree_pointer_node* prev;
        key_node* nextknode;
        key_node* prevknode;
        int key;
        void* value;
};

class node{
    public:
        //Constructors
        node(int p);
        node(tree_pointer_node* begin,tree_pointer_node* end,int size, int p);

        //Main Utility Functions
        key_node* insert_key(int key,void* value);
        key_node* delete_key(int key, bool right_ptr);
        key_node* search_key(int key);
        bool check_min();

        //Some extra utilities
        node* get_subtree(int key);
        int isFull();
        void print_node();
        key_node* get_middle();
        tree_pointer_node* getTreePtr(node* n);

        //Getters
        tree_pointer_node* getStart();
        tree_pointer_node* getLast();
        bool getLeaf();
        int getSize();
        int getDepth();
        node* getNext();
        node* getPrev();
        bool getRoot();
        int getMin();
        
        //Setters
        void setNext(node* n);
        void setPrev(node* n);
        void setLeaf(bool b);
        void setDepth(int d);
        void setRoot(bool b);
        
    private:
        tree_pointer_node* begin;
        tree_pointer_node* end;
        int size;
        int p;
        int min;
        int depth;
        node* next;
        node* prev;
        bool leaf;
        bool root;
        
};