#include <iostream>
using namespace std;
using value_type = int;
template<class T>
typename T value_type
foo(){
    return value_type(0);
}

int main(){
    int x = 9;
    cout << foo() << endl;
    return 0;
}