#define PARENT(i) (i) / 2
#define LEFT(i) ((i) * 2 + 1)
#define RIGHT(i) ((i) * 2 + 2)
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

typedef int heapElm;
struct heap{
    unsigned int length;
    unsigned int heapSize;
    heapElm* A;
};

void maxHeapPify(heap &, int);
void buildHeap(heap &);
void heapSort(heap &);
int main(int argc, char* argv[]){
    heap HH;
    ifstream input;
    input.open(string(argv[1]));
    input >> HH.length;
    HH.A = new heapElm [HH.length];
    for(int i = 0; i < HH.length; ++i){
        input >> HH.A[i];
    } 
    clock_t beg = clock();
    heapSort(HH);
    clock_t end = clock();
    for(int i = 0; i < HH.length; ++i){
        cout << HH.A[i] << " ";
        if(i % 20 == 19){
            cout << "\n";
        }
    }
    cout << endl;
    cout << "using heap Sort " << HH.length << " num" << endl;;
    cout << "time :" << end - beg  <<" ms" << endl;
    return 0;
}

void maxHeapPify(heap &H, int i){
    int left = LEFT(i);
    int right= RIGHT(i);
    int largest = i;
    if(H.heapSize > left && H.A[i] < H.A[left]){
        largest = left;
    }
    if(H.heapSize > right && H.A[largest] < H.A[right]){
        largest = right;
    }
    if(largest != i){
        int temp = H.A[i];
        H.A[i] = H.A[largest];
        H.A[largest] = temp;
        maxHeapPify(H, largest);
    }
}
void buildHeap(heap &H){
    H.heapSize = H.length;
    for(int i = H.heapSize / 2 - 1; i >= 0; --i){
        maxHeapPify(H, i);
    }
}
void heapSort(heap &H){
    buildHeap(H);
    while(H.heapSize > 0){
        int max = H.A[0];
        H.A[0] = H.A[H.heapSize - 1];
        H.A[H.heapSize - 1] = max;
        --H.heapSize;
        maxHeapPify(H, 0);
    }
}