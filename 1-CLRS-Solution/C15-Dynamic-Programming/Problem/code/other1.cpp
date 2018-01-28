#include <iostream>
#include <climits>
#include <vector>
#include <string>
using namespace std;
//
int main(){
    int k,m,n;
    cin >> k >> m >> n;
    int M[m][3];
    int N[n];
    for(int i = 0; i < m; ++i){
        cin >> M[i][0] >> M[i][1] >> M[i][2];
    } 
    for(int i = 0; i < n; ++i){
        cin >> N[i];
    }
    int dp[k+1][n][n];
    pair<int,int> con[k+1][n][n];
    for(int i = 0; i < k + 1; ++i){
        for(int j = 0; j < n; ++j){
            for(int s = 0; s < n; ++s){
                dp[i][j][s] = INT_MAX;
                con[i][j][s] = {-2, -2};
            }
        }
    }
    int temp;
    for(int beg = n - 1; beg >= 0; --beg){
        for(int end = beg; end < n; ++end){
            if(beg == end){
                dp[N[beg]][beg][end] = 0;
                con[N[beg]][beg][end] = {-1, 0};
            }
            else{
                for(int color = 1; color <= k; ++color){
                    for(int change = 0; change < m; ++change){
                        if(M[change][0] == color){
                            int firstC = M[change][1];
                            int secondC= M[change][2];
                            for(int shit = beg; shit < end; ++shit){
                                if(dp[firstC][beg][shit] == INT_MAX || dp[secondC][shit+1][end] == INT_MAX)
                                    temp = INT_MAX;
                                else
                                    temp = max(dp[firstC][beg][shit], dp[secondC][shit+1][end]) + 1;
                                if(temp < dp[color][beg][end]){
                                    dp[color][beg][end] = temp;
                                    con[color][beg][end] = {change, shit};
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vector<pair<int,pair<int, int>>> Q;
    vector<pair<int,pair<int, int>>> ne;
    Q.push_back({1, {0, n-1}});
    while(dp[1][0][n-1] >= 0){
        if(dp[1][0][n-1] == INT_MAX){
            cout << "Naive" << endl;
            return 0;
        }
        for(int i = 0; i < Q.size(); ++i){
            int beg = Q[i].second.first;
            int end = Q[i].second.second;
            if(Q[i].first == -1){
                cout << ",";
                continue;
            }
            else if(Q[i].first == -2){
                cout << " ";
                continue;
            }
            else
                cout << Q[i].first;
            if(beg != end){
                int first = con[Q[i].first][beg][end].first;
                int second= con[Q[i].first][beg][end].second;
                ne.push_back({M[first][1], {beg, second}});
                ne.push_back({-2,{-1,-1}});
                ne.push_back({M[first][2], {second + 1, end}});
                ne.push_back({-1,{-1,-1}});
            }
            else{
                ne.push_back({Q[i].first, {beg, end}});
                ne.push_back({-1,{-1,-1}});
            }
        }
        Q.clear();
        Q = ne;
        ne.clear();
        cout << endl;
        dp[1][0][n-1] -= 1;
    }
    return 0;
}