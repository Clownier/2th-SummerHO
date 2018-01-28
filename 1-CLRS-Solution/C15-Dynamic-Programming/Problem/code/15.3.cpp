#include <iostream>
#include <climits>
#include <cmath>
using namespace std;
const int N = 7;
int Graph[N][2] = {
    {1,7},
    {2,1},
    {3,4},
    {6,5},
    {7,2},
    {8,6},
    {9,3}
};
double dis(int* a, int* b){
    int x1 = a[0];
    int y1 = a[1];
    int x2 = b[0];
    int y2 = b[1];
    double res = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
    return res;
}
int main(){
    double dp[N][N];
    dp[2][0] = dis(Graph[2], Graph[0]) + dis(Graph[1], Graph[2]) + dis(Graph[0], Graph[1]);
    cout << dp[2][0] << endl;
    for(int i = 3; i < N; ++i){
        for(int j = 0; j < i - 2; ++j){
            dp[i][j] = dp[i - 1][j] - dis(Graph[i-1],Graph[j]) + dis(Graph[i-1],Graph[i]) + dis(Graph[j],Graph[i]);
        }
        dp[i][i-2] = 1e5;
        double temp;
        for(int j = 0; j < i - 2; ++j){
            temp = dp[i - 1][j] - dis(Graph[i-1],Graph[i-2]) + dis(Graph[i-1],Graph[i]) + dis(Graph[i-2],Graph[i]);
            dp[i][i-2] = min(temp, dp[i][i-2]);
        }
    }
    double temp, res = 1e5;
    for(int i = 0; i < N - 2; ++i){
        temp = dp[N - 1][i];
        res = min(temp, res);
    }
    cout << res << endl;
    return 0;
}