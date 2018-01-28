#include <iostream>
#include <climits>
#include <cmath>
using namespace std;
int main(){
    const int N = 7;
    int dp[N];
    int M = 30;
    int cost[N][N];
    int G[N];
    for(int i = 0; i < N; ++i){
        cin >> G[i];
    }
    int temp;
    for(int i = 0; i < N; ++i){
        for(int j = i; j < N; ++j){
            if(i == j) 
                cost[i][j] = G[j];
            else 
                cost[i][j] = cost[i][j - 1] + G[j] + 1;
        }
    }
    int last;
    for(int i = 0; i < N; ++i){
        dp[i] = INT_MAX;
        for(int j = 0; j <= i; ++j){
            if(cost[j][i] > M){
                temp = INT_MAX;
                continue;
            }
            else if(i == N - 1){
                temp = j == 0?  0 :  dp[j - 1];
            }
            else if(j == 0){
                temp = M - cost[j][i];
            }
            else{
                temp = dp[j - 1] + M - cost[j][i];
            }
            if(temp < dp[i]){
                dp[i] = temp;
                last = j;
            }
        }
    }
    cout << dp[N - 1] << endl;
    return 0;
}