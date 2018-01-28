#include <iostream>
#include <string>
#include <climits>
#include <climits>
#define PARENT(i) (i) / 2
#define LEFT(i) ((i) * 2 + 1)
#define RIGHT(i) ((i) * 2 + 2)
using namespace std;
typedef int heapElm;
struct heap{
    unsigned int length;
    unsigned int heapSize;
    heapElm* A;
};
void maxHeapPify(heap &, int);
void buildHeap(heap &);

class priorityQueues{
    private:
        heap H;
    public:
        priorityQueues(int num, int *A){H.length = num; H.A = A;buildHeap(H);};
        void insert(heapElm x){
            H.heapSize++;
            H.A[H.heapSize - 1] = INT_MIN;
            increaseKey(H.heapSize - 1, x);
        };
        heapElm maxImun(){return H.A[0];};
        heapElm extractMax(){
            if(H.heapSize < 1) 
                cout << "overflow" << endl; 
            int maxElm = H.A[0];
            H.A[0] = H.A[H.heapSize - 1]; 
            H.heapSize--;
            maxHeapPify(H, 0);
            return maxElm;
        };
        void increaseKey(int i, int k){
            if(k < H.A[i])
                return;
            H.A[i] = k;
            while(i > 0 && H.A[PARENT(i)] < H.A[i]){
                int temp = H.A[i];
                H.A[i] = H.A[PARENT(i)];
                H.A[PARENT(i)] = temp;
                i = PARENT(i);
            }
        };
};

int main(){
    int A[30] = {0,1,2,3,4,5,6,7,11,9};
    priorityQueues Q(10, A);
    Q.insert(8);
    for(int i = 0; i < 11; ++i){
        cout << Q.extractMax() << endl;
    }
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