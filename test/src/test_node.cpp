#include <iostream>
#include <string>
#include "../../include/node.hpp"

using namespace std;
int main(){
    node n=node(5);
    int j;
    n.insert_key(1,NULL);
    n.print_node();
    n.insert_key(2,NULL);
    n.print_node();
    n.insert_key(3,NULL);
    n.print_node();
    n.insert_key(4,NULL);
    n.print_node();
    n.insert_key(5,NULL);
    n.print_node();
    n.insert_key(3,NULL);
    n.print_node();
    

    n.delete_key(1,false);
    n.print_node();
    n.delete_key(1,true);
    n.print_node();
    n.delete_key(5,false);
    n.print_node();
    n.delete_key(3,true);
    n.print_node();
    n.delete_key(4,false);
    n.print_node();
    
}