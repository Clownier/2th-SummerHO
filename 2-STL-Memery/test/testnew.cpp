#include <iostream>
#include <memory>
#include <array>
#include <ctime>
#include <climits>
#include <cstdlib>
using namespace std;
int main(){
    int x = 0;
    int y = 0;
    try{
        int *a;
        a = new int [INT_MAX];
    }
    catch(bad_alloc e){
        cout << x << " " << y << endl;
        cout << e.what() << endl;
    }
    return  0;
}