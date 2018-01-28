#include <iostream>
#include <string>
using namespace std;
int main(){
    int price[25] = {1,5,8,9,10,17,17,20,24,30,35,
                    39,43,48,53,57,60,63,66,72,76,
                    84,88,95,100};
    int len;
    cin >> len;
    int dp[len + 1];
    int con[len+ 1];
    for(int i = 0; i <= len; ++i){
        dp[i] = 0;
        con[i] = 0;
        for(int j = 0; j < i; ++j){
            if(dp[j] + price[i - j - 1] > dp[i]){
                dp[i] = dp[j] + price[i - j - 1];
                con[i] = j;
            }
        }
    }
    cout << dp[len] << endl;
    for(int i = len; i > 0;){
        cout << price[i - con[i] - 1] << " ";
        i = con[i];
    }
    return 0;
}