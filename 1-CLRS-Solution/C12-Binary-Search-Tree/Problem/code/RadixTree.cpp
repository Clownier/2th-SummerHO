#include <iostream>
#include <string>
using namespace std;
struct node{
    string key;
    bool flag;
    node* left;
    node* right;
};

class RadixTree{
    private:
        node* root;
        void preorder(node* p);
    public:
        RadixTree() {root = new node [1]; root->left=NULL; root->right=NULL; root->flag=false; root->key="";};
        void insert(string key);
        void preorderTravel();
        node* getRoot(){return root;}
};
void RadixTree::preorderTravel(){
    preorder(root);
}
void RadixTree::preorder(node* p){
    if(!p) return;
    if(p->flag==true){
        cout << p->key << endl;
    }
    preorder(p->left);
    preorder(p->right);
}
void RadixTree::insert(string key){
    node* p = root;
    for(int i = 0; i < key.length(); ++i){
        if(key[i] == '1'){
            if(!p->right){
                p->right = new node [1];
                p->right->key = key.substr(0, i + 1);
                p->right->flag = false;
                p->right->left =NULL;
                p->right->right=NULL;
                p = p->right;
            }
            else{
                p = p->right;
            }
        }
        else{
            if(!p->left){
                p->left = new node [1];
                p->left->key = key.substr(0, i + 1);
                p->left->flag = false;
                p->left->left =NULL;
                p->left->right=NULL;
                p = p->left;
            }
            else{
                p = p->left;
            }
        }
    }
    p->flag = true;
}
int main(){
    RadixTree radix;    
    radix.insert(string("011"));
    radix.insert(string("10"));
    radix.insert(string("0"));
    radix.insert(string("100"));
    radix.insert(string("1011"));
    radix.preorderTravel();
    return 0;
}
