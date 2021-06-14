#include "../include/b+tree.hpp"
#include <stack>

b_plus_tree::b_plus_tree(int p){
    this->p=p;
    this->depth=0;
    this->num_keys=0;
    this->root=new node(p);
    this->root->setLeaf(true);
}

int b_plus_tree::get_depth(){
    return this->depth;
}

int b_plus_tree::get_num_keys(){
    return this->num_keys;
}

void b_plus_tree::insert_key(int key,void* value){ 
    std::stack<node*> s;
    s.push(this->root);
    while(1){
        node* top=s.top();
      
        if(top->getLeaf()){
            top->insert_key(key,value);
            break;
        }else{
            node* subtree=top->get_subtree(key);
            s.push(subtree);
        }
    }
    
    while(!s.empty()){
        node* top=s.top();
        s.pop();
        if(top->isFull()){
            key_node* middle=top->get_middle();

            //Preparing the left node at split
            int left_size=(top->getSize() %2==0)?(top->getSize()/2)+1:(top->getSize()+1)/2;
            node* Left=new node(top->getStart(),middle->prev,left_size-1,this->p);
            middle->prev->next=NULL;
            middle->prevknode->nextknode=NULL;

            //Preparing the Right node at split
            key_node* rightBegin=middle;
            if(!top->getLeaf()){
                rightBegin=middle->nextknode;
                rightBegin->prev->prev=NULL;
                middle->prevknode=NULL;
                middle->prev=NULL;
                middle->nextknode=NULL;
                middle->next=NULL;
            }else{
                tree_pointer_node* tptr=new tree_pointer_node();
                tptr->next=rightBegin;
                rightBegin->prev=tptr;
                rightBegin->prevknode=NULL;
            }
            int off=(rightBegin==middle)?1:0;   
            node* Right=new node(rightBegin->prev,top->getLast(),top->getSize()-left_size+off,this->p);
            if(top->getLeaf()){
                Left->setNext(Right);
                Left->setPrev(top->getPrev());
                if(top->getPrev()!=NULL)top->getPrev()->setNext(Left);
                Right->setPrev(Left);
                Right->setNext(top->getNext());
                if(top->getNext()!=NULL)top->getNext()->setPrev(Right);
                Left->setLeaf(true);
                Right->setLeaf(true);
            }

            if(!s.empty()){
                key_node* i=s.top()->insert_key(middle->key,middle->value);
                i->prev->ptr=Left;
                i->next->ptr=Right;
            }else{
                node* n=new node(this->p);
                key_node* i=n->insert_key(middle->key,middle->value);
                i->prev->ptr=Left;
                i->next->ptr=Right;
                this->root=n;
                this->depth++;
            }
        }else{
            break;
        }
    }
    
}

void* b_plus_tree::search_key(int key){
    node* t=this->root;
    
    while(!t->getLeaf()){
        t=t->get_subtree(key);
    }
    key_node* v=t->search_key(key);
    if(v==NULL)return NULL;
    return v->value;
}

node* b_plus_tree::getLeftNeighbhor(node* child, node*parent){
    tree_pointer_node* temp=parent->getStart();

    while(temp!=NULL && temp->ptr!=child){
        temp=temp->next->next;
    }

    if(temp->prev==NULL)return NULL;
    return temp->prev->prev->ptr;
}

node* b_plus_tree::getRightNeighbhor(node* child,node* parent){
    tree_pointer_node* temp=parent->getStart();

    while(temp!=NULL && temp->ptr!=child){
        temp=temp->next->next;
    }

    if(temp->next==NULL)return NULL;
    return temp->next->next->ptr;
}

void b_plus_tree::updateWithInorderSuccessor(node* n,int key){
    key_node* temp=n->getStart()->next;
    while(temp!=NULL){
        if(temp->key==key)break;
        temp=temp->nextknode;
    }
    if(temp==NULL)return;
    node* t=temp->next->ptr;
    while(t!=NULL && !t->getLeaf())t=t->getStart()->ptr;
    if(t==NULL)return;
    temp->key=t->getStart()->next->key;
    temp->value=t->getStart()->next->value;
}

node* b_plus_tree::merge_nodes(key_node* root, bool incl_root){
    node* left=root->prev->ptr;
    node* right=root->next->ptr;
    node* n=new node(this->p);
    key_node* kn=left->getStart()->next;
    n->getLast()->ptr=left->getStart()->ptr;
    while(kn!=NULL){
        n->insert_key(kn->key,kn->value);
        n->getLast()->ptr=kn->next->ptr;
        kn=kn->nextknode;
    }
    if(incl_root){
        n->insert_key(root->key,root->value);
        n->getLast()->ptr=right->getStart()->ptr;
    }
    kn=right->getStart()->next;
    while(kn!=NULL){
        n->insert_key(kn->key,kn->value);
        n->getLast()->ptr=kn->next->ptr;
        kn=kn->nextknode;
    }

    n->setNext(right->getNext());
    if(right->getNext()!=NULL)right->getNext()->setPrev(n);

    n->setPrev(left->getPrev());
    if(left->getPrev()!=NULL)left->getPrev()->setNext(n);
    
    return n;    
}

bool b_plus_tree::delete_key(int key){
    if(this->search_key(key)==NULL)return false;
    std::stack<node*> s;
    s.push(this->root);  
    while(1){
        node* top=s.top();
        if(top->getLeaf()){
            break;
        }else{
            node* subtree=top->get_subtree(key);
            s.push(subtree);
        }
    }
    key_node* delnode=s.top()->delete_key(key,true); 
    if(s.size()==1)return true; //if only root node is present we can quit
    
    while(!s.empty()){
        node* top=s.top();
        s.pop();
        if(s.empty()){
            this->updateWithInorderSuccessor(top,key);
            break;// Root node is allowed to underflow
        }

        //Checking if there is underflow
        if(top->getSize()<top->getMin()){
            node* parent=s.top();
            node* leftNeighbhor=this->getLeftNeighbhor(top,parent);
            node* rightNeighbhor=this->getRightNeighbhor(top,parent);
            tree_pointer_node* tptr=parent->getTreePtr(top);

            if(leftNeighbhor!=NULL && leftNeighbhor->getSize()>=leftNeighbhor->getMin()+1){
                //Borrow from left neighbhor
                key_node* last=tptr->prev->prev->ptr->getLast()->prev;
                last=leftNeighbhor->delete_key(last->key,true);
                key_node* left=tptr->prev;

                // top->insert_key(left->key,left->value);
                // left->key=last->key;
                // left->value=left->value;
                // this->updateWithInorderSuccessor(top,key);
                if(left->key==key){
                    left=last;
                }else if(top->getStart()->next!=NULL && left->key==top->getStart()->next->key){
                    left=last;
                }
                
                top->insert_key(left->key,left->value);
                top->getStart()->next->next->ptr=top->getStart()->ptr;
                top->getStart()->ptr=last->next->ptr;
                tptr->prev->key=last->key;
                tptr->prev->value=last->value;

                this->updateWithInorderSuccessor(top,left->key);

            }else if(rightNeighbhor!=NULL && rightNeighbhor->getSize()>=rightNeighbhor->getMin()+1){
                //Borrow from Right neighbhor
                key_node* first=tptr->next->next->ptr->getStart()->next;
                first=rightNeighbhor->delete_key(first->key,false);
                key_node* right=tptr->next;
                top->insert_key(right->key,right->value);
                top->getLast()->ptr=first->prev->ptr;

                // if(right->key==first->key)first=rightNeighbhor->getStart()->next;
                // first=tptr->next->next->ptr->getStart()->next;
                // tptr->next->key=first->key;
                // tptr->next->value=first->value;
                this->updateWithInorderSuccessor(parent,right->key);

            }else{
                //Merge with sibling
                if(leftNeighbhor!=NULL){
                    // bool incl_root=(tptr->prev->key!=key);
                    bool incl_root=true;
                    if(top->getStart()->next!=NULL){
                        incl_root=(tptr->prev->key!=top->getStart()->next->key);
                    }
                    
                    node* new_node=this->merge_nodes(tptr->prev,incl_root);
                    new_node->setLeaf(top->getLeaf());
                    parent->delete_key(tptr->prev->key,false);
                    if(new_node->search_key(key)!=NULL){
                        this->updateWithInorderSuccessor(new_node,key);
                        if(new_node->search_key(key)!=NULL){
                            new_node->delete_key(key,true);
                        }
                    }
                    
                    tptr->ptr=new_node;

                }else if(rightNeighbhor!=NULL){
                    bool incl_root=(tptr->next->key!=rightNeighbhor->getStart()->next->key);
                    node* new_node=this->merge_nodes(tptr->next,incl_root);
                    new_node->setLeaf(top->getLeaf());

                    parent->delete_key(tptr->next->key,true);
                    tptr->ptr=new_node;
                }
                //If root ended up with no keys, then decrease height of tree and change the root to merged node
                if(parent->getSize()==0 && parent==this->root){
                    this->root=parent->getStart()->ptr;
                    this->depth--;
                    break;
                }
            } 


        }

        //If the key is occuring still occurs in the internal node, replacing it with its inorder succesor
        this->updateWithInorderSuccessor(top,key);
    }
    return true;
}

ResultSet* b_plus_tree::range(int a,int b){

    node* t=this->root;
    while(!t->getLeaf()){
        t=t->get_subtree(a);
    }
    key_node* k=t->search_key(a);
    if(k==NULL)return NULL;

    ResultSet* rs=new ResultSet();
    rs->setStart(t,k);
    rs->setEnd(b);
    return rs;
}

ResultSet::ResultSet(){
    this->curNode=NULL;
    this->curKey=NULL;
}

void ResultSet::setStart(node* n,key_node* k){
    this->curNode=n;
    this->curKey=k;
}

void ResultSet::setEnd(int b){
    this->end=b;
}

void* ResultSet::next(){
    if(this->curKey==NULL)return NULL;
    int key=this->curKey->key;
    void* value=this->curKey->value;

    if(key==this->end){
        this->curKey=NULL;
        this->curNode=NULL;
    }else{
        this->curKey=this->curKey->nextknode;
        if(this->curKey==NULL){
            this->curNode=this->curNode->getNext();
            if(this->curNode!=NULL)this->curKey=this->curNode->getStart()->next;
        }
    }
    return value;
}

void* ResultSet::current(){
    if(this->curKey==NULL)return NULL;
    return this->curKey->value;
}

// void* ResultSet::prev(){

// }

