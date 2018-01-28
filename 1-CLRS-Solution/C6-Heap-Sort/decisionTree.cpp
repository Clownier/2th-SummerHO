#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>
#include <stack>
#include <vector>
#include <climits>
#include <queue>
using namespace std;

int N;
static int number = 0;
static int compare_num = 0;
struct node{
    int in;
    int out;
    int flag;
    vector<int> order;
    node *left;
    node *right;
};
node* root;
int* A;
int* indexArr;
vector<vector<int>> data;
void allocSpace(int size);
void bfsOut(node* ptr, ofstream &dotfile);
void getAllperm(int* A, int start, int len);
void insertSort(vector<int> thisPrem, int length, node* root, int num);

void SortMerge(int* A, int p, int r, node* root);
void mergeSort(int* thisPrem, int p, int r, node** current);
void merge(int* thisPrem, int p, int q, int r, node** current);

int main(int argc, char* argv[]){
    N = atoi(argv[1]);
    string filename = argv[2];
    allocSpace(N);
    getAllperm(A, 0, N);
    int sum = 0;
    ofstream dotfile;
    dotfile.open(filename);
    for(int i = 0; i < data.size(); ++i){
        node* rootPtr = root;
        int* elmThisPerm = new int [N]; 
        for(int j = 0; j < N; ++j){
            indexArr[data[i][j]] = j;
            elmThisPerm[j] = data[i][j];
        }
        if(argv[3][0] == '1'){
            insertSort(data[i], N, rootPtr, i);
        }else if(argv[3][0] == '2'){
            SortMerge(elmThisPerm, 0, N, rootPtr);
        }
        sum += compare_num;
        compare_num = 0;
    }
    node* ptr = root;
    cout << "begin generate dot file" << endl;
    bfsOut(ptr, dotfile);
    dotfile.close();
    cout << "average: " << (sum * 1.0) / data.size() << endl;
}
void allocSpace(int size){
    A = new int [size];
    for(int i = 0; i < size; ++i){
        A[i] = i + 1;
    }
    indexArr = new int [size];
    root = new node [1];
    root -> left = NULL;
    root -> right = NULL;
}

void getAllperm(int* A, int start, int len){
    if(start == len - 1){
        vector<int> oneperm;
        for(int i = 0; i < len; ++i){
            oneperm.push_back(A[i]);
        }
        if(find(data.begin(), data.end(), oneperm) == data.end())
            data.push_back(oneperm);
    }else{
        for(int i = 0; i < len; ++i){
            swap(A[start], A[i]);
            getAllperm(A, start + 1, len);
            swap(A[start], A[i]);
        }
    }
}

void insertSort(vector<int> thisPrem, int length, node* root, int num){
    int key = 0, j = 0;
    node *current = root;
    current -> flag = 0;
    for(int i = 1; i < length; ++i){
        key = thisPrem[i];
        j = i - 1;
        while(j >= 0){
            current->in = indexArr[thisPrem[j]];
            current->out = indexArr[key];
            if(thisPrem[j] < key){
                if(!current->left){
                    current ->left = new node [1];
                    current = current -> left;
                    number++;
                    current->flag = number;
                    current ->left = NULL;
                    current ->right = NULL;
                }else
                    current = current->left;
                compare_num++;
                thisPrem[j + 1] = thisPrem[j]; 
                --j;
            }else{
                if(!current->right){
                    current->right = new node [1];
                    current = current->right;
                    number++;
                    current->flag = number;
                    current ->left = NULL;
                    current ->right = NULL;
                }else
                    current = current->right;
                compare_num++;
                break;
            }
        }
        thisPrem[j + 1] = key;
    }
    current->in = INT_MAX;
    current->out = num;
    for(int i = 0; i < N; ++i){
        current->order.push_back(indexArr[A[i]]);
    }
}

void SortMerge(int* thisPrem, int p, int r, node* root){
    node *current = root;
    current -> flag = 0;
    mergeSort(thisPrem, p, r, &current);
    current->in = INT_MAX;
    current->out = 0;
    for(int i = 0; i < N; ++i){
        current->order.push_back(indexArr[thisPrem[i]]);
    }
}
void mergeSort(int* thisPrem, int p, int r, node** current){
    if(p < r - 1){
        int q = (p + r - 1) / 2;
        mergeSort(thisPrem, p, q + 1, current);
        mergeSort(thisPrem, q + 1, r, current);
        merge(thisPrem, p, q, r, current);
    }
}
void merge(int* thisPrem, int p, int q, int r, node** ptr_node){
    int n1 = q + 1 - p;
    int n2 = r - q - 1;
    vector<int> L(n1, 0);
    vector<int> R(n2, 0);
    for(int i = 0; i < n1; ++i){
        L[i] = thisPrem[p + i];
    }
    for(int i = 0; i < n2; ++i){
        R[i] = thisPrem[q + 1 + i];
    }
    int i = 0, j = 0;
    for(int k = p; k < r;){
        compare_num++;
        if(i < n1 && j < n2 && L[i] < R[j]){
            (*ptr_node)->in = indexArr[L[i]];
            (*ptr_node)->out = indexArr[R[j]];
            if(!(*ptr_node)->left){
                (*ptr_node)->left = new node [1];
                number++;
                (*ptr_node) = (*ptr_node)->left;
                (*ptr_node)->flag = number;
                (*ptr_node)->left = NULL;
                (*ptr_node)->right = NULL;
            }else
                (*ptr_node) = (*ptr_node)->left;
            thisPrem[k] = R[j];
            k++;
            j++;
        }else if(i < n1 && j < n2){
            (*ptr_node)->in = indexArr[L[i]];
            (*ptr_node)->out = indexArr[R[j]];
            if(!(*ptr_node)->right){
                (*ptr_node)->right = new node [1];
                number++;
                (*ptr_node) = (*ptr_node)->right;
                (*ptr_node)->flag = number;
                (*ptr_node)->left = NULL;
                (*ptr_node)->right = NULL;
            }else
                (*ptr_node) = (*ptr_node)->right;
            thisPrem[k] = L[i];
            k++;
            i++;
        }else{
            while(i < n1){
                thisPrem[k] = L[i];
                i++;
                k++;
            }
            while(j < n2){
                thisPrem[k] = R[j];
                j++;
                k++;
            } 
        }
    }
}

void bfsOut(node* ptr, ofstream &dotfile){
    dotfile << "graph x{" << "\n";
    int* memory;
    memory = new int [number + 1];
    for(int i = 0; i < number + 1; ++i){
        memory[i] = 0; 
    }
    queue<node*> Q; 
    dotfile << ptr -> flag << "[label = \"" << 
        ptr -> in << ':' << ptr -> out <<"\"]" << "\n";
    memory[ptr->flag] = 1;
    Q.push(ptr);
    int floor = 0;
    while(!Q.empty()){
        node* current = Q.front();
        Q.pop();
        if(current -> left){
            if(memory[(current->left)->flag] == 0){
                if((current->left)->in == INT_MAX){
                    dotfile << (current->left) ->flag << "[label = \"";
                    for(int k = 0; k < (current->left)->order.size(); ++k){
                        dotfile << (current->left)->order[k];
                    }
                    dotfile  <<"\"]" << "\n";
                }else{
                    dotfile << (current->left) ->flag << "[label = \"" << 
                        (current->left) -> in << ':' << (current -> left) -> out <<"\"]" << "\n";
                }
            }
            memory[(current->left)->flag] = 1;
            dotfile << current->flag << " -- " << (current -> left) -> flag << 
                "[color = blue]" << "\n";
            Q.push(current -> left);
        }
        if(current -> right){
            if(memory[(current->right)->flag] == 0){
                if((current->right)->in == INT_MAX){
                    dotfile << (current->right) ->flag << "[label = \"";
                    for(int k = 0; k < (current->right)->order.size(); ++k){
                        dotfile << (current->right)->order[k];
                    }
                    dotfile  <<"\"]" << "\n";
                }else{
                    dotfile << (current->right) ->flag << "[label = \"" << 
                        (current->right) -> in << ':' << (current -> right) -> out <<"\"]" << "\n";
                }
            }
            memory[(current->right)->flag] = 1;
            dotfile << current->flag << " -- " << (current -> right) -> flag << 
                "[color = red]" << "\n";
            Q.push(current -> right);
        }
    }
    dotfile << "}"  << "\n";
}