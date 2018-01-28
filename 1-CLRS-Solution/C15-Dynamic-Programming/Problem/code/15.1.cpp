#include <iostream>
#include <climits>
using namespace std;

const int N = 6;
int G[6][6] = {
    {0,5,2,0,0,0},
    {0,0,2,6,0,0},
    {0,0,0,7,4,2},
    {0,0,0,0,-1,1},
    {0,0,0,0,0,-2},
    {0,0,0,0,0,0}
};
int dp[N];

int longestPath(int s, int t){
    if(dp[s] != INT_MIN) return dp[s];
    int temp;
    int res = INT_MIN;
    if(s == t)
        return 0;
    for(int i = 0; i < N; ++i){
        if(G[s][i] != 0){
            temp = longestPath(i, t) + G[s][i];
            res = max(temp, res);
        }
    }
    dp[s] = res;
    return res;
}

int main(){
    for(int i = 0; i < N; ++i){
        dp[i] = INT_MIN;
    }
    cout << longestPath(1, 5) << endl;;
    return 0;
}