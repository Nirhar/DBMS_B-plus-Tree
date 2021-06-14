#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include<time.h>
#include "../../include/b+tree.hpp"
using namespace std;


b_plus_tree* b;

void printRS(int a,int c){
    ResultSet* rs=b->range(a,c);
    if(rs==NULL)exit(0);
    char* s;
    cout<<"Leaf connections: ";
    while((s=(char*)rs->next())!=NULL){
        cout<<s<<" ";
        
    }
    cout<<endl;
}

int main(){
    b=new b_plus_tree(5);
     
    cout<<"----------INSERTION---------\n\n";
    cout<<endl;
    cout<<"100 random values are entered into a b-plus-tree of p=5.\n";
    cout<<"The values in the order of insertion are:\n";
    vector<int> ins;
    srand(time(0));
    for(int i=0;i<100;i++){
        int j=rand()%10000;
        if(b->search_key(j)!=NULL){continue;}// If randomly generated key is already present ignore it
        ins.push_back(j);
        cout<<j<<" ";
        char* buf=(char*)malloc(sizeof(char)*6);
        sprintf(buf,"H%d",j);
        b->insert_key(j,(void*)buf);
    }
    cout<<endl;
    cout<<"Height of tree after insert = "<<b->get_depth()<<"\n\n";

    cout<<"---------RANGE QUERIES-----\n\n";
    cout<<"Here I show some random range queries done on the keys\n";
    int r=rand()%90;
    //JDBC style resultset object
    int x=ins[r]<ins[r+10]?ins[r]:ins[r+10];
    int y=ins[r]>=ins[r+10]?ins[r]:ins[r+10];
    cout<<"Query 1: Give me all metadata between "<<x<<" and "<<y<<"\n";
    ResultSet* rs=b->range(ins[r],ins[r+10]);
    char* s;
    if(rs!=NULL){
        // cout<<"Leaf connections: ";
        while((s=(char*)rs->next())!=NULL){
            cout<<s<<", ";
        }
        cout<<endl<<endl;
    }
    r=rand()%85;
    x=ins[r]<ins[r+15]?ins[r]:ins[r+15];
    y=ins[r]>=ins[r+15]?ins[r]:ins[r+15];
    //JDBC style resultset object
    cout<<"Query 2: Give me all metadata between "<<x<<" and "<<y<<"\n";
    rs=b->range(ins[r],ins[r+15]);
    if(rs!=NULL){
        // cout<<"Leaf connections: ";
        while((s=(char*)rs->next())!=NULL){
            cout<<s<<", ";
        }
        cout<<endl<<endl;
    }
    

    cout<<"---------DELETION----------\n\n";
    cout<<"Now all the inserted values are deleted in the same order as they were inserted\n";

    for(int i:ins){
        bool presbef=b->search_key(i);
        bool t=b->delete_key(i);
        bool pres=b->search_key(i);
    }
    cout<<"Height of tree after deletion = "<<b->get_depth()<<"\n";
    cout<<"-------------------\n\n";
   
}