#include <fstream>
#include <iostream>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <string>
using namespace std;
int main(int argc, char* argv[]){
    int N, i = 0;
    srand(static_cast<unsigned int>(time(0)));
    N = atoi(argv[1]);
    ofstream output;
    output.open("sortData", ios::trunc);
    output << N;
    output << "\n";
    while(i < N){
        int x = rand() % 1000;
        output << x << " ";
        ++i;
    }
    return 0;
}