#include <iostream>
#include <climits>
using namespace std;

int main(){
    int matrix[20][2] = {
        {30,35},
        {35,15},
        {15, 5},
        {5, 10},
        {10,20},
        {20,25},
        {25,30},
        {30,75},
        {75,60},
        {60,55},
        {55,10},
        {10, 5},
        { 5, 5},
        { 5,15},
        {15,25},
        {25,30},
        {30,35},
        {35,40},
        {40,45},
        {45,50}
    };
    int len;
    cin >> len;
    int dp[len + 1][len + 1];
    int con[len +1][len + 1];
    for(int i = 0; i < len + 1; ++i){
        for(int j = 0; j < len + 1; ++j){
            dp[i][j] = 0;
            con[i][j] = 0;
        }
    }
    for(int i = 2; i <= len; ++i){
        for(int j = 0; j <= len - i;  ++j){
            dp[j][i] = INT_MAX;
            for(int k = 1; k <= i - 1; ++k){
                if(dp[j][k] + dp[j + k][i - k] + matrix[j][0] * matrix[j + k][0] * matrix[i + j - 1][1] < dp[j][i]){
                    dp[j][i] = dp[j][k] + dp[j + k][i - k] + matrix[j][0] * matrix[j + k][0] * matrix[j + i - 1][1];
                    con[i][j] = k;
                }
            }
        }
    }
    cout << dp[0][len] << endl; 
    return 0;
}