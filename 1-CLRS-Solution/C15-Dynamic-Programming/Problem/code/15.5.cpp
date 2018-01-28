#include <iostream>
#include <string>
#include <map>
#include <climits>
#include <vector>
using namespace std;

int main(){
    string x;
    string y;
    map<string, int> cost;
    vector<string> operation = {
        "insert",
        "copy",
        "delete",
        "replace",
        "twiddle",
        "kill"
    };
    for(int i = 0; i < 6; ++i){
        int a;
        cin >> a;
        cost[operation[i]] = a;
    }
    cin >> x;
    cin >> y;
    int temp;
    int dp[x.length() + 1][y.length() + 1];
    for(int i = 0; i <= x.length(); ++i){
        for(int j = 0; j <= y.length(); ++j){
            dp[i][j] = INT_MAX;
            if(i == 0){
                dp[i][j] = j * cost[string("insert")];
            }
            else if(j == 0){
                dp[i][j] = i * cost[string("delete")];
            }
            else{
                if(x[i] == y[j]){
                    temp = dp[i-1][j-1] + cost[string("copy")];
                    dp[i][j] = min(temp, dp[i][j]);
                }
                if(x[i-1] = y[j] && y[j-1] == x[i]){
                    temp = dp[i-2][j-2] + cost[string("twiddle")];
                    dp[i][j] = min(temp, dp[i][j]);
                }
                temp = dp[i][j-1] + cost[string("insert")];
                dp[i][j] = min(temp, dp[i][j]);
                temp = dp[i-1][j-1] + cost[string("replace")];
                dp[i][j] = min(temp, dp[i][j]);
                temp = dp[i-1][j] + cost[string("delete")];
                dp[i][j] = min(temp, dp[i][j]);
                if(i == x.length()-1 && j == y.length()-1){
                    for(int k = 0; k < x.length() - 1; ++k){
                        temp = dp[k][y.length()-1] + cost[string("kill")];
                        dp[i][j] = min(temp, dp[i][j]);
                    }
                }
            }
        }
    }
    cout << dp[x.length()][y.length()];
    return 0;
}