#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <climits>
#include <ctime>
using namespace std;
int main(){
    clock_t begin = clock();
    int i = 0;
    while(1){
        //cout << i << endl;
        i++;
        if(i == 100000000){
            break;
        }
    }
    clock_t end = clock();
    cout << end - begin << endl;
    return 0;
}