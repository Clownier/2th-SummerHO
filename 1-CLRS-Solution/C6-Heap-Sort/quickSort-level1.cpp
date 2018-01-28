#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include <cmath>
#include <ctime>
#include <cstdlib>
using namespace std;
#define SWAP(x, y) (y) = (x) + (y); (x) = (y) - (x); (y) = (y) - (x);

int parition(int* A, int p, int r);
void quickSort(int *A, int p, int r);
int main(){
    int A[] = {5, 8, 2, 45, 7, 48, 9};
    quickSort(A, 0, 7);
    for(int i = 0; i < 7; ++i){
        cout << A[i] << " ";
    }
    cout << endl;
}

void quickSort(int* A, int p, int r){
    if(p < r){
        int q = parition(A, p, r);
        quickSort(A, p, q);
        quickSort(A, q + 1, r);
    }
}
int parition(int* A, int p, int r){
    int flag = A[r - 1];
    int i = p - 1;
    int j = p;
    while(j < r - 1){
        if(A[j] <= flag){
            i++;
            if(i != j){
                SWAP(A[i], A[j]);
            }            
        }
        j++;
    }
    i++;
    if(i != r - 1){
        SWAP(A[i], A[r - 1]);
    }
    return i;
}