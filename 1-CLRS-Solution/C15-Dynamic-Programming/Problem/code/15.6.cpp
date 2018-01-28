#include <iostream>
#include <string>
#include <map>
#include <climits>
#include <vector>
using namespace std;

map<int, vector<int>> company = {
    {100,{50, 45, 67}},
    {50, {36, 43, 21, 29}},
    {45, {40, 41}},
    {67, {51, 55}},
    {36, {19, 16, 23, 24}},
    {43, {18, 11,  5, 13}},
    {21, {12, 20, 25,  8}},
    {29, {14, 17,  9,  7}},
    {40, { 4,  5}},
    {41, { 1,  3}},
    {51, {}},
    {55, {}},
    {19, {}},
    {16, {}},
    {23, {}},
    {24, {}},
    {18, {}},
    {11, {}},
    { 5, {}},
    {13, {}},
    {12, {}},
    {20, {}},
    {25, {}},
    { 8, {}},
    {14, {}},
    {17, {}},
    { 9, {}},
    { 7, {}},
    { 4, {}},
    { 5, {}},
    { 1, {}},
    { 3, {}}
};
const int N = 32;
map<pair<int,int>,int> dp;
int getDP(int root, int flag);
int main(){
    cout << max(getDP(100, 0), getDP(100, 1)) << endl;
    return 0;
}

int getDP(int root, int flag){
    if(dp.find({root, flag}) != dp.end()){
        return dp[{root, flag}];
    }
    if(company[root].empty()){
        if(flag == 0){
            dp[{root,0}] = 0;
        }
        else{
            dp[{root,1}] = root;
        }
    }
    else{
        dp[{root,flag}] = 0;
        if(flag == 0){
            for(int i = 0; i < company[root].size(); ++i){
                dp[{root, 0}] += max(getDP(company[root][i],0), getDP(company[root][i],1));
            }
        }
        else{
            for(int i = 0; i < company[root].size(); ++i){
                dp[{root, 1}] += getDP(company[root][i],0);
            }
            dp[{root, 1}] += root;
        }
    }
    return dp[{root,flag}];
}
