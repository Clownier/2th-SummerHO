#include <iostream>
#include <climits>
using namespace std;
// optimal binary search tree
int main(){
    int length;
    cin >> length;
    double probability[length];
    int element[length];
    for(int i = 0; i < length; ++i){
        cin >> probability[i];
    }
    double w[length + 1][length + 1];
    for(int i = 1; i <= length; ++i){
        for(int j = i; j <= length; ++j){
            if(i == j){
                w[i][j] = probability[j - 1];
            }
            else{
                w[i][j] = w[i][j - 1] + probability[j - 1];                
            }
        }
    }
    double dp[length + 1][length + 1];
    int noderoot[length + 1][length + 1];
    double temp;
    for(int right = 1; right <= length; ++right){
        for(int left = right; left >= 1; --left){
            if(left + 1 >= right){
                if(left == right){
                    dp[left][right] = w[left][right];
                    noderoot[left][right] = left;
                }
                if(left + 1 == right){
                    if(dp[left][left] < dp[right][right]){
                        dp[left][right] = dp[left][left] + w[left][right];
                        noderoot[left][right] = right;
                    }
                    else{
                        dp[left][right] = dp[right][right] + w[left][right];
                        noderoot[left][right] = left;
                    }
                }
            }
            else{
                dp[left][right] = INT_MAX;
                for(int root = left; root <= right; ++root){
                    temp = dp[left][root - 1] + dp[root + 1][right] + w[left][right];
                    if(temp < dp[left][right]){
                        dp[left][right] = temp;
                        noderoot[left][right] = root;
                    } 
                }
            }
        }
    }
    cout << dp[1][length];
    return 0;
}

void coutTree(int* noderoot, int left, int right){
    int root = noderoot[left][right];
    cout << root;
    if(root - 1 >= left)
        cout << coutTree(noderoot, left, root - 1) << " is the left children of " << root << endl;
    if(root + 1 <= right)
        cout << coutTree(noderoot, root + 1, right) << " is the right children of " << root << endl;
}