#include <iostream>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
using namespace std;
static int nnn = 0;
const bool RED = 1;
const bool BLACK = 0;
struct statellite{
    int x;
};
struct nodeElm{
    int key;
    statellite otherData;
};
struct node{
    nodeElm elm;
    bool color; 
    node* left;
    node* right;
    node* p;
    node(nodeElm x) {elm = x; color = 1; left = NULL; right = NULL; p = NULL;}
};
class RBT{
    private:
        node* root;
        node* nil;
        void _Rb_tree_insert_rebalance(node* nobalancenode);
        void _Rb_tree_leftrotate(node* x);
        void _Rb_tree_rightrotate(node* x);
        void _transplace(node* u, node* v);
        node* _Rb_tree_find_right_successer(node* x);
        void _Rb_tree_erase_rebalance(node* x);
    public:
        RBT() {nodeElm x; x.key = -1; nil = new node(x); root = nil; nil->p = root; nil->color = BLACK;}
        void insert(nodeElm elm);
        void erase(nodeElm elm);
        void find(nodeElm elm);
        node* getRoot(){return root;}
        bool empty();
};
void RBT::insert(nodeElm elm){
    node* iter = root;
    node* father = nil;
    node* newNode = new node(elm);
    while(iter != nil){
        father = iter;
        if((iter->elm).key > elm.key){
            iter = iter -> left;// small insert left
        }else{
            iter = iter -> right;// big insert right
        }
    }
    newNode ->p = father;
    if(father == nil){
        root = newNode;
    }else if((father->elm).key > elm.key ){
        father ->left = newNode;
    }else{
        father ->right = newNode;
    }
    newNode -> left = nil;
    newNode -> right = nil;
    newNode -> color = RED;
    _Rb_tree_insert_rebalance(newNode);
}
void RBT::_Rb_tree_leftrotate(node* x){
    node* y = x->right;
    x->right = y->left;
    if(y ->left != nil){
        (y->left)->p = x;
    }
    y->p = x->p;
    if(x->p == nil){
        root = y;
    }else if(x == (x->p)->left){
        (x->p)->left = y;
    }else{
        (x->p)->right= y;
    }
    y->left = x;
    x->p = y;
}
void RBT::_Rb_tree_rightrotate(node* x){
    node* y = x -> left;
    x->left = y->right;
    if(y->right != nil){
        (y->right)->p = x;
    }
    y->p = x->p;
    if(x->p == nil){
        root = y;
    }else if(x == (x->p)->left){
        (x->p)->left = y;
    }else{
        (x->p)->right = y;
    }
    y->right = x;
    x->p = y;
}
void RBT::_Rb_tree_insert_rebalance(node* nobalancenode){
    node* current = nobalancenode;
    while((current ->p)->color == RED){
        if(current->p == ((current->p)->p)->left){
            node* uncle =((current->p)->p)->right;
            node* grandpa=(current->p)->p;
            node* father=(current)->p;
            if(uncle->color == RED){
                father->color = BLACK;
                uncle->color = BLACK;
                grandpa->color=RED;
                current = grandpa;
            }else{
                if(father->right == current){
                    _Rb_tree_leftrotate(father);
                    current = father;
                    father = current->p;
                }
                father->color = BLACK;
                grandpa->color= RED;
                _Rb_tree_rightrotate(grandpa);
            }
        }else{
            node* uncle = ((current->p)->p)->left;
            node* grandpa=(current->p)->p;
            node* father=(current)->p;
            if(uncle->color == RED){
                father->color = BLACK;
                uncle ->color = BLACK;
                grandpa->color= RED;
                current = grandpa;
            }else{
                if(current == father->left){
                    current = father;
                    _Rb_tree_rightrotate(current);
                    father = current ->p;
                }
                father->color = BLACK;
                grandpa->color =RED;
                _Rb_tree_leftrotate(grandpa);
            }
        }
    }
    root->color = BLACK;
}
void RBT::erase(nodeElm elm){
    node* iter = root;
    while(iter != nil && elm.key != (iter->elm).key){
        if((iter->elm).key > elm.key){
            iter = iter->left;
        }else{
            iter = iter->right;
        }
    }
    node* x;
    node* z = iter;
    node* y = z;
    bool y_old_color = z->color;
    if(z->left == nil){
        x = y->right;
        _transplace(z, y->right);
    }else if(z->right == nil){
        x = y->left;
        _transplace(z, y->left);
    }else{
        y = _Rb_tree_find_right_successer(z);
        y_old_color = y->color;
        x = y->right;
        _transplace(y, y->right);
        _transplace(z, y);
        y->left = z ->left;
        (y->left)->p  = y;
        y->right = z->right;
        (y->right)->p = y;
        y->color = z->color;
    }
    if(y_old_color == BLACK){
        //cout << "reBalance" << (x->elm).key << endl;
        _Rb_tree_erase_rebalance(x);
    }
}
void RBT::_Rb_tree_erase_rebalance(node* x){
    while(x != root && x->color == BLACK){
        if(x == (x->p)->left){
            node* sibling = (x->p)->right;
            if(sibling->color == RED){
                (x->p)->color = RED;
                sibling->color = BLACK;
                _Rb_tree_leftrotate(x->p);
            }else if((sibling->left)->color == BLACK && (sibling->right)->color == BLACK){
                sibling-> color = RED;
                x = x->p;
            }else if((sibling->left)->color == RED && (sibling->right)->color == BLACK){
                (sibling->left)->color = BLACK;
                sibling->color = RED;
                _Rb_tree_rightrotate(sibling);
            }else if((sibling->right)->color == RED){
                sibling->color = (x->p)->color;
                (sibling->right)->color = BLACK;
                (x->p)->color = BLACK;
                _Rb_tree_leftrotate(x->p);
                x = root;
            }
        }else{
            node* sibling = (x->p)->left;
            if(sibling->color == RED){
                (x->p)->color = RED;
                sibling->color = BLACK;
                _Rb_tree_rightrotate(x->p);
            }else if((sibling->left)->color == BLACK && (sibling->right)->color == BLACK){
                sibling->color = RED;
                x = x->p;
            }else if((sibling->right)->color == RED && (sibling->left)->color == BLACK){
                sibling->color = RED;
                (sibling->right)->color = BLACK;
                _Rb_tree_leftrotate(sibling);
            }else if((sibling->left)->color == RED){
                (sibling->left)->color = BLACK;
                sibling->color = (x->p)->color;
                (x->p)->color = BLACK;
                _Rb_tree_rightrotate(x->p);
                x = root;
            }
        }
    }
    x->color = BLACK;
}
node* RBT::_Rb_tree_find_right_successer(node* x){
    node* iter = x->right;
    while(iter != nil && iter->left != nil){
        iter = iter->left;
    }   
    return iter;
}
void RBT::_transplace(node* u, node* v){
    if(u->p == nil){
        root = v;
    }else if(u == (u->p)->left){
        (u->p)->left = v;
    }else{
        (u->p)->right = v;
    }
    v->p = u->p;
}
void bfsOut(node* ptr, ofstream &dotfile);
const int number = 10000;
int main(){
    vector<int> v(10001, 0);
    RBT tree;
    string kk = "xx";
    for(int i = 1; i < 1000; ++i){
        nodeElm x;
        int ram = rand() % 1000;
        while(v[ram] == 1){
            ram = rand() % 1000;
        }
        x.key = ram;
        v[ram] = 1;
        tree.insert(x);
    }
    for(int i = 1; i < 970; ++i){
        nodeElm x;
        int ram = rand() % 1000;
        while(v[ram] != 1){
            ram = rand() % 1000;
        }
        v[ram] = 0;
        x.key = ram;
        cout << x.key << endl;
        tree.erase(x);
    }
    ofstream filenn;
    filenn.open(kk);
    bfsOut(tree.getRoot(), filenn);
    return 0;
}
void bfsOut(node* ptr, ofstream &dotfile){
    dotfile << "graph x{" << "\n";
    dotfile << "node [shape = \"circle\"]\n";
    dotfile << "edge [style = \"bold\"]\n";
    string red = "red";
    string black = "black";
    int* memory;
    memory = new int [number + 1];
    for(int i = 0; i < number + 1; ++i){
        memory[i] = 0; 
    }
    queue<node*> Q; 
    if((ptr->color) == RED){
        dotfile << (ptr->elm).key << " [label = \"" << (ptr->elm).key << "\"," <<
        "style = \"filled\", " << "fontcolor = \"white\", "
        << "fillcolor=\"red\"]" 
        << "\n";
    }else{
        dotfile << (ptr->elm).key << " [label = \"" << (ptr->elm).key << "\"," <<
        "style = \"filled\", " << "fontcolor = \"white\", " << "fillcolor=\"black\"]" 
        << "\n";
    }
    memory[(ptr->elm).key] = 1;
    Q.push(ptr);
    int floor = 0;
    while(!Q.empty()){
        node* current = Q.front();
        Q.pop();
        if((current->elm).key == -1){
            continue;
        }
        if(current -> left && ((current->left)->elm).key != -1){
            if(memory[((current->left)->elm).key] == 0){
                if(((current->left) ->color) == RED)
                dotfile << ((current->left) ->elm).key << " [label = \"" << 
                    ((current->left) ->elm).key  <<"\"" <<
                     "style = \"filled\", " << "fontcolor = \"white\", " << "fillcolor=\"red\"]" 
                    << "\n";
                else
                dotfile << ((current->left) ->elm).key << " [label = \"" << 
                    ((current->left) ->elm).key  <<"\"" <<
                     "style = \"filled\", " << "fontcolor = \"white\", " << "fillcolor=\"black\"]" 
                    << "\n";
            }
            memory[((current->left)->elm).key] = 1;
            dotfile << (current->elm).key << " -- " << ((current -> left)->elm).key << 
                "[color = red]" << "\n";
            Q.push(current -> left);
        }
        if(current -> right && ((current->right)->elm).key != -1){
            if(memory[((current->right)->elm).key] == 0){
                if(((current->right) ->color) == RED)
                dotfile << ((current->right) ->elm).key << " [label = \"" << 
                    ((current->right) ->elm).key <<"\"" <<
                    "style = \"filled\", " << "fontcolor = \"white\", " << "fillcolor=\"red\"]" 
                    << "\n";
                else
                dotfile << ((current->right) ->elm).key << " [label = \"" << 
                    ((current->right) ->elm).key <<"\"" <<
                    "style = \"filled\", " << "fontcolor = \"white\", " << "fillcolor=\"black\"]" 
                    << "\n";
            }
            memory[((current->right)->elm).key] = 1;
            dotfile << (current->elm).key << " -- " << ((current->right)->elm).key << 
                "[color = blue]" << "\n";
            Q.push(current->right);
        }
    }
    dotfile << "}"  << "\n";
}