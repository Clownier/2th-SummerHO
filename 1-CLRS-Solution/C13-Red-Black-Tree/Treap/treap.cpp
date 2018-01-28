#include <iostream>
#include <stack>
#include <random>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
using namespace std;
static int nnn = 0;
const bool RED = 1;
const bool BLACK = 0;
struct node{
    int elm;
    int priority;
    node* left;
    node* right;
};
void bfsOut(node* ptr, ofstream &dotfile);
class Treap{
    private:
        node* root;
        void initial(int x, int y);
        void left_rotate(node* x);
        void right_rotate(node* x);
        void _Treap_insert_balcace(node* x, stack<node*> route);
    public:
        Treap() {root = NULL;}
        //Treap(int x, int y) {initial(x,y);}
        void insert(int elm);
        void erase(int elm);
        void find(int elm);
        node* getRoot(){return root;}
        bool empty();
};

void Treap::insert(int x){
    int priority = rand() % 100;
    stack<node *> route;
    node* iter = root;
    node* father = NULL;
    while(iter != NULL){
        father = iter;
        route.push(father);
        if(iter->elm < x){
            iter = iter -> right;
        }else{
            iter = iter -> left;
        }
    }
    node* newNode = new node [1];
    newNode->elm = x;
    newNode->priority = priority;
    newNode->left = NULL;
    newNode->right = NULL;
    if(father == NULL){
        root = newNode;
    }else if(father->elm < x){
        father->right = newNode;
    }else{
        father->left = newNode;
    }
    _Treap_insert_balcace(newNode, route);
}

void Treap::_Treap_insert_balcace(node* x, stack<node*> route){
    node* father = NULL;
    node* grandpa = NULL;
    while(x != root){
        father = route.top();
        if(x->priority >= father->priority) break;
        route.pop();
        if(father->left == x){
            right_rotate(father);
        }else{
            left_rotate(father);
        }
        if(!route.empty()){
            grandpa = route.top();
            if(grandpa -> left == father){
                grandpa ->left = x;
            }else{
                grandpa ->right= x;
            }
        }else
            root = x;
    }
}

void Treap::right_rotate(node* x){
    node* newx = x->left;
    x->left = newx->right;
    newx->right = x;
}

void Treap::left_rotate(node* x){
    node* newx = x->right;
    x->right = newx->left;
    newx->left = x;
}

void Treap::erase(int x){
    node* iter = root;
    node* father= NULL;
    stack<node*> route;
    while(iter != NULL && iter->elm != x){
        father = iter;
        route.push(father);
        if(iter->elm < x){
            iter = iter->right;
        }else{
            iter = iter->left;
        }
    }
    int i = 0;
    if(!route.empty()){
        father = route.top();
        route.pop();
    }
    while(iter->left != NULL && iter->right != NULL){
        if(father == NULL){
            node* l = iter->left; 
            node* r = iter->right;
            if(l->priority > r->priority){
                left_rotate(iter);
                root = r;
                father= r;
            }else{
                right_rotate(iter);
                root = l;
                father = l;
            }
        }else if(father->left == iter){
            node* l = iter->left; 
            node* r = iter->right;
            if(l->priority > r->priority){
                left_rotate(iter);
                father->left = r;
                father= r;
            }else{
                right_rotate(iter);
                father->left = l;
                father = l;
            }
        }else{
            node* l = iter->left; 
            node* r = iter->right;
            if(l->priority > r->priority){
               left_rotate(iter);
               father->right = r;
               father = r;
            }else{
                right_rotate(iter);
                father->right = l;
                father = l;
            }
        }
    }
    if(father == NULL){
        if(iter->left == NULL){
            root = iter->right;
            delete iter;
        }else{
            root = iter->left;
            delete iter;
        }
    }else if(father->left == iter){
        if(iter->left == NULL){
            father->left = iter->right;
            delete iter;
        }else{
            father->left = iter->left;
            delete iter;
        }
    }else{
        if(iter->left == NULL){
            father->right = iter->right;
            delete iter;
        }else{
            father->right = iter->left;
            delete iter;
        }
    }
}

int number = 10000;
int main(){
    Treap T;
    int M[10000] = {0};
    for(int i = 0; i < 100; ++i){
        int ram = rand() % 150;
        while(M[ram] == 1){
            ram = rand() % 150;
        }
        M[ram] = 1;
        T.insert(ram);
    }
    for(int i = 0; i < 99; ++i){
        int ram = rand() % 150;
        while(M[ram] == 0){
            ram = rand() % 150;
        }
        M[ram] = 0;
        cout << ram << endl;
        T.erase(ram);
    }
    string kk = "xx";
    ofstream filenn;
    filenn.open(kk);
    bfsOut(T.getRoot(), filenn);
    return 0;
}

void bfsOut(node* ptr, ofstream &dotfile){
    dotfile << "graph x{" << "\n";
    dotfile << "node [shape = \"circle\"]\n";
    dotfile << "edge [style = \"bold\"]\n";
    int* memory;
    memory = new int [number + 1];
    for(int i = 0; i < number + 1; ++i){
        memory[i] = 0; 
    }
    queue<node*> Q; 
    dotfile << ptr -> elm << "[label = \"" << 
        ptr -> elm << ':' << ptr -> priority <<"\"]" << "\n";
    memory[ptr->elm] = 1;
    Q.push(ptr);
    int floor = 0;
    while(!Q.empty()){
        node* current = Q.front();
        Q.pop();
        if(current -> left){
            if(memory[(current->left)->elm] == 0){
                dotfile << (current->left) ->elm << "[label = \"" << 
                    (current->left) -> elm << ':' << (current -> left) -> priority <<"\"]" << "\n";
            }
            memory[(current->left)->elm] = 1;
            dotfile << current->elm << " -- " << (current -> left) -> elm << 
                "[color = blue]" << "\n";
            Q.push(current -> left);
        }
        if(current -> right){
            if(memory[(current->right)->elm] == 0){
                dotfile << (current->right) ->elm << "[label = \"" << 
                    (current->right) -> elm << ':' << (current -> right) -> priority <<"\"]" << "\n";
            }
            memory[(current->right)->elm] = 1;
            dotfile << current->elm << " -- " << (current -> right) -> elm << 
                "[color = red]" << "\n";
            Q.push(current -> right);
        }
    }
    dotfile << "}"  << "\n";
}