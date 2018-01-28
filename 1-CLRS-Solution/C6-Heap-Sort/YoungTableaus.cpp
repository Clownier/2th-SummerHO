#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include <cmath>
#include <ctime>
#include <cstdlib>
#define SWAP(x, y) (y) = (x) + (y); (x) = (y) - (x); (y) = (y) - (x);

using namespace std;
typedef int Elm;

class YoungTableaus{
    private:
        int M;
        int N;
        Elm **A;
        Elm left(int x, int y){return A[x][y - 1];}
        Elm upon(int x, int y){return A[x - 1][y];}
        void buildYT(int m, int n){
            for(int i = m - 1; i >= 0; --i){
                for(int j = n - 1; j >= 0; --j){
                    reBalance(i, j);
                }
            }
        }
        void reBalance(int x, int y){
            if(x == M - 1 && y == N - 1)
                return;
            else if(x == M - 1){
                if(A[x][y] < A[x][y + 1]){
                    SWAP(A[x][y], A[x][y + 1]);
                    reBalance(x, y + 1);
                }
                else
                    return; 
            }
            else if(y == N - 1){
                if(A[x][y] < A[x + 1][y]){
                    SWAP(A[x][y], A[x + 1][y]);
                    reBalance(x + 1, y);
                }
                else 
                    return;
            }
            else{
                if(A[x][y] >= A[x + 1][y] && A[x][y] >= A[x][y + 1]){
                    return;
                }else if(A[x][y] >= A[x + 1][y]){
                    SWAP(A[x][y], A[x][y + 1]);
                    reBalance(x, y + 1);
                }else if(A[x][y] >= A[x][y + 1]){
                    SWAP(A[x][y], A[x + 1][y]);
                    reBalance(x + 1, y);
                }else{
                    if(A[x + 1][y] >= A[x][y + 1]){
                        SWAP(A[x][y], A[x + 1][y]);
                        reBalance(x + 1, y);
                    }else{
                        SWAP(A[x][y], A[x][y + 1]);
                        reBalance(x, y + 1);
                    }
                }
            }
        }
    public:
        YoungTableaus() {
            M = 0; N = 0; 
            A = NULL;
        }
        YoungTableaus(int m, int n){
            M = m; N = n; 
            A = new Elm* [M];
            for(int i = 0; i < M; ++i){
                A[i] = new Elm [N];
            }
            for(int i = 0; i < M; ++i){
                for(int j = 0; j < N; ++j){
                    A[i][j] = INT_MIN;
                }
            }
        }
        YoungTableaus(int m, int n, Elm** a){
            M = m; N = n;
            A = a;
            buildYT(m, n);
        }
        Elm **getA(){return A;}
        
        void insert(Elm item){
            int col_last = N - 1, row_last = M - 1;
            if(A[row_last][col_last] != INT_MIN){
                cout << " no space" << endl;
                return;
            }
            A[row_last][col_last] = item;
            for(int i = 0, j = 0; i + j < M + N;){
                cout << row_last - i << " " <<  col_last - j << endl;
                if(col_last - j == 0 && row_last - i == 0){
                    break;
                }else if(row_last - i != 0 && upon(row_last - i, col_last - j) == INT_MIN){
                    SWAP(A[row_last - i - 1][col_last - j], A[row_last - i][col_last - j]);
                    i++;
                }else if(col_last - j != 0 && left(row_last - i, col_last - j) == INT_MIN){
                    SWAP(A[row_last - i][col_last - j - 1], A[row_last - i][col_last - j]);
                    j++;
                }else if(col_last - j != 0 && row_last - i != 0 &&
                    A[row_last - i][col_last - j] <= upon(row_last - i, col_last - j) &&
                    A[row_last - i][col_last - j] <= left(row_last - i, col_last - j)){
                    break;
                }else if(col_last - j == 0 && 
                    A[row_last - i][col_last - j] <= upon(row_last - i, col_last - j)){
                    break;
                }else if(row_last - i == 0 &&
                    A[row_last - i][col_last - j] <= left(row_last - i, col_last - j)){
                    break;
                }else if(row_last - i == 0 ||
                    A[row_last - i][col_last - j] <= upon(row_last - i, col_last - j)){
                    SWAP(A[row_last - i][col_last - j - 1], A[row_last - i][col_last - j]);
                    j++;
                }else if(col_last - j == 0 ||
                    A[row_last - i][col_last - j] <= left(row_last - i, col_last - j)){
                    SWAP(A[row_last - i - 1][col_last - j], A[row_last - i][col_last - j]);
                    i++;
                }else{
                    if(left(row_last - i, col_last - j) >= upon(row_last - i, col_last - j)){
                        SWAP(A[row_last - i - 1][col_last - j], A[row_last - i][col_last - j]);
                        i++;
                    }else{
                        SWAP(A[row_last - i][col_last - j - 1], A[row_last - i][col_last - j]);
                        j++;
                    }
                }
            }
        }

        Elm deleteMax(){
            Elm maxElm = A[0][0];
            A[0][0] = INT_MIN;
            reBalance(0, 0);
            return maxElm;
        }
};

int main(int argc, char *argv[]){
    int **A;
    int M, N, len;
    ifstream input;
    input.open(string(argv[1]));
    input >> len;
    M = sqrt(len) + 1;
    N = M;
    A = new Elm* [M];
    for(int i = 0; i < M; ++i){
        A[i] = new Elm [N] {INT_MIN};
    }
    for(int i = 0; i < len; ++i){
       input >>  A[i % N][i / N]; 
    }
    YoungTableaus YT(M, N, A);
    for(int i = 0; i < len; ++i){
        Elm a = YT.deleteMax();
        cout << a << " ";
        if(i % 20 == 19)
            cout << "\n";
    }
    cout << endl;
    return 0;
}

