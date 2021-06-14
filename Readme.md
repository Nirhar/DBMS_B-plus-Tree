Readme

This Document is organised as follows:
    1. Instructions on how to run the testfiles
    2. Documentation of B+ Tree class
    3. Documentation of ResultSet class

############################### 1. INSTRUCTIONS ##############################

To compile the file and build the executables enter:
    make all

To remove all the executables, type
    make clean

[In windows make may be replaced with mingw32-make command] 
Now that the executables are built, go to folder test/build and run
    ./test_tree

The program ./test_tree randomly inserts 100-key-value pairs into a b-plus-tree of 
order 5 and does some random range queries on it. After that it deletes all the key-value pairs to verify if the height of the tree becomes 0

Note: The Meta data given here is the value of the key prepended with a "H"

have a look at the sample runs directory to see screenshots of some runs

############################### 2. B+ TREE CLASS #############################

To include the class copy files from the include folder to your project, and after 
building the project copy the b+tree.o object file to your project

The B+ tree class has the following functions:

b_plus_tree(int p);
    It is the constructor, p is the max number of tree pointers in a node
        
void insert_key(int key, void* value);
    Used to insert keys-value pairs into b+ tree
    value is void* type and can be used to point to any metadata

void* search_key(int key);
    Gives the metadata associated with the given key
    returns NULL if key not found

ResultSet* range(int start, int end);
    Gives a ResultSet object that contains the given range of key-values
    returns NULL if start not found
    [Look at ResultSet to navigate the ResultSet object]

bool delete_key(int key);
    Deletes the given key and associated meta-data from the tree
    returns NULL if key not found

int get_depth();
    return the depth of the key

int get_num_keys();
    returns the number of keys present in the tree

node* root
    A pointer to the root of the tree;


############################### 3. RESULTSET CLASS #############################

To include the class copy files from the include folder to your project, and after 
building the project copy the b+tree.o object file to your project

ResultSet();
    Constructor to initiate a result set

void setStart(node* n,key_node* k);
    Set the start pointer of the resultset, with the given key in the given node

void setEnd(int b);
    Sets the ending key of the resultSet

void* next();
    returns the current key's metadata and moves the pointer to the next key
    returns NULL if end of set is reached
        
void* current();
    returns the current key's metadata

        
