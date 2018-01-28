#include <iostream>
using namespace std;
int main(){
    int len;
    cin >> len;
    int dp[len + 1];
    dp[0] = 0;
    dp[1] = 1;
    for(int i = 2; i <= len; ++i){
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    cout << dp[len] << endl;
    return 0;
}