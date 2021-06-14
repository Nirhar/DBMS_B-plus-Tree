#include "../include/node.hpp"

//Constructors
tree_pointer_node::tree_pointer_node(){
    this->ptr=NULL;
    this->next=NULL;
    this->prev=NULL;
}
key_node::key_node(int key, void* value){
    this->key=key;
    this->value=value;
    this->next=NULL;
    this->prev=NULL;
    this->prevknode=NULL;
    this->nextknode=NULL;
}
node::node(int p){
    this->p=p;
    this->size=0;
    tree_pointer_node* tptr=new tree_pointer_node();
    this->begin=tptr;
    this->end=tptr;
    this->depth=0;
    this->leaf=false;
    this->prev=NULL;
    this->next=NULL;
    this->min=(this->p%2)?(this->p/2)-1:((this->p+1)/2)-1;
}
node::node(tree_pointer_node* begin, tree_pointer_node* end, int size, int p){
    this->p=p;
    this->size=size;
    this->begin=begin;
    this->end=end;
    this->depth=0;
    this->leaf=false;
    this->root=true;
    this->prev=NULL;
    this->next=NULL;
    this->min=(this->p%2)?(this->p/2)-1:((this->p+1)/2)-1;

    this->begin->prev=NULL;
    if(this->begin->next!=NULL)this->begin->next->prevknode=NULL;
    this->end->next=NULL;
    if(this->end->prev)this->end->prev->nextknode=NULL;


}


//Function to check if node is full
int node::isFull(){
    return node::size>node::p-1;
}

//Function to insert into a node and returns pointer to where the node was inserted
key_node* node::insert_key(int k,void* value){
    if(this->size>=(this->p))return NULL;
    key_node* kn=new key_node(k,value);
    tree_pointer_node* tptr=new tree_pointer_node();
    kn->next=tptr;
    tptr->prev=kn;
    
    key_node* temp=this->begin->next;
    key_node* temp2=NULL;
    while(temp!=NULL){
        temp2=temp;
        if(temp->key>=k)break;
        temp=temp->nextknode;
    }

    if(temp2==NULL){
        this->begin->next=kn;
        kn->prev=this->begin;
        this->end=kn->next;
    }else if(temp==NULL){
        temp2->nextknode=kn;
        kn->prevknode=temp2;
        temp2->next->next=kn;
        kn->prev=temp2->next;

        this->end=kn->next;
    }else{
        key_node* pr=temp->prevknode;
        kn->prev=temp2->prev;
        kn->nextknode=temp2;
        kn->next->next=temp2;

        temp2->prev=kn->next;
        temp2->prevknode=kn;
        kn->prev->next=kn;
        
        if(pr!=NULL){
            kn->prevknode=pr;
            kn->prevknode->nextknode=kn;

        }else{
            this->begin=kn->prev;
        }    
    }
    this->size++;
    return kn;
}


//Function to search for a key in a node
key_node* node::search_key(int key){
    key_node* temp=this->begin->next;
    while(temp!=NULL){
        if(temp->key==key)break;
        temp=temp->nextknode;
    }
    if(temp==NULL)return NULL;
    return temp;
}

key_node* node::delete_key(int key,bool right_ptr){
    // if(!this->check_min())return NULL;
    key_node* temp=this->begin->next;

    while(temp!=NULL){
        if(temp->key==key)break;
        temp=temp->nextknode;
    }
    if(temp==NULL)return NULL;
    bool noRight=true,noLeft=true;
    if(temp->prevknode!=NULL){
        temp->prevknode->nextknode=temp->nextknode;
        temp->prevknode=NULL;
        noLeft=false;
    }else{
        if(!right_ptr)this->begin=temp->next;
    }
    if(temp->nextknode!=NULL){
        temp->nextknode->prevknode=temp->prev->prev;
        temp->nextknode=NULL;
        noRight=false;
    }else{
        if(right_ptr)this->end=temp->prev;
    }
    
    if(right_ptr){
        temp->prev->next=temp->next->next;
        if(!noRight)temp->next->next->prev=temp->prev;
        temp->prev=NULL;
        temp->next->next=NULL;
    }else{
        temp->next->prev=temp->prev->prev;
        if(!noLeft)temp->prev->prev->next=temp->next;
        temp->next=NULL;
        temp->prev->prev=NULL;
    }
    this->size--;
    return temp;

}

bool node::check_min(){
    return this->size>this->min;
}

//Function to print the keys of the node in order of storage
void node::print_node(){
    key_node* temp=this->begin->next;
    std::cout<<"Node contains the keys: ";
    while(temp!=NULL){
        std::cout<<temp->key<<", ";
        temp=temp->nextknode;
    }
    std::cout<<std::endl;
}

//Function to get the middle key of the node
key_node* node::get_middle(){
    key_node* temp=this->begin->next;
    int middle=(this->size%2==0)?(this->size/2)+1:(this->size+1)/2;
    for(int i=1;i<middle;i++)temp=temp->nextknode;
    return temp;
}

tree_pointer_node* node::getTreePtr(node* n){
    tree_pointer_node* temp=this->getStart();
    while(temp!=NULL && temp->ptr!=n){
        temp=temp->next->next;
    }
    return temp;
}

tree_pointer_node* node::getStart(){
    return this->begin;
}

tree_pointer_node* node::getLast(){
    return this->end;
}

bool node::getLeaf(){
    return this->leaf;
}
int node::getSize(){
    return this->size;
}

int node::getDepth(){
    return this->depth;
}
node* node::getNext(){
    return this->next;
}
bool node::getRoot(){
    return this->root;
}

node* node::get_subtree(int key){
    key_node* temp=this->begin->next;
    if(temp==NULL)return NULL;
    key_node* temp2;
    while(temp!=NULL){
        temp2=temp;
        if(temp->key>key)break;
        temp=temp->nextknode;
    }
    if(temp==NULL)return temp2->next->ptr;
    return temp->prev->ptr;
}
int node::getMin(){
    return this->min;
}
node* node::getPrev(){
    return this->prev;
}


void node::setPrev(node* n){
    this->prev=n;
}

void node::setNext(node* n){
    this->next=n;
}

void node::setLeaf(bool b){
    this->leaf=b;
}
void node::setRoot(bool b){
    this->root = b;
}