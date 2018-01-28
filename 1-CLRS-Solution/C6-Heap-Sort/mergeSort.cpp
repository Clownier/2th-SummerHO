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
void mergeSort(int* thisPrem, int p, int r);
void merge(int* thisPrem, int p, int q, int r);
int main(){
    int* thisPrem;
    thisPrem = new int [4];
    for(int i = 0; i < 4; ++i){
        thisPrem[i] = rand() % 4;
    }
    for(int i = 0; i < 4; ++i){
        cout << thisPrem[i] << " ";
        if(i % 20 == 19)
        cout << endl;
    }
    cout << endl;
    mergeSort(thisPrem, 0, 4);
    for(int i = 0; i < 4; ++i){
        cout << thisPrem[i] << " ";
        if(i % 20 == 19)
        cout << endl;
    }
}
void mergeSort(int* thisPrem, int p, int r){
    if(p < r - 1){
        int q = (p + r - 1) / 2;
        mergeSort(thisPrem, p, q + 1);
        mergeSort(thisPrem, q + 1, r);
        merge(thisPrem, p, q, r);
    }
}
void merge(int* thisPrem, int p, int q, int r){
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
        if(i < n1 && j < n2 && L[i] < R[j]){
            thisPrem[k] = R[j];
            k++;
            j++;
        }else if(i < n1 && j < n2){
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