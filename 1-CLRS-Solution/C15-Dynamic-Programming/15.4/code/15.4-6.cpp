#include <iostream>
#include <string>
using namespace std;
string insert(string s, char x);
int main(){
    string s;
    cin >> s;
    string res;
    for(int i = 0; i < s.length(); ++i){
        if(s[i] >= res[res.length() - 1]){
            res.push_back(s[i]);
        }
        else{
            res = insert(res, s[i]);
        }
    }
    cout << res << endl;
    return 0;
}

string insert(string s, char x){
    if(s.empty()){
        s.push_back(x);
        return s;
    }
    int beg = 0;
    int end = s.length() - 1;
    int mid = s.length() / 2;
    while(beg <= end){
        if(s[mid] > x){
            end = mid - 1;
        }
        else{
            beg = mid + 1;
        }
        mid = (beg + end) / 2;
    }
    s[beg] = x;
    return s;
}