#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
string lcs(string s1, string s2);
int main(){
    string s1;
    string s2;
    cin >> s1;
    s2 = s1;
    sort(s1.begin(), s1.end());
    string s = lcs(s1, s2);
    for(int i = s.length() - 1; i >= 0; --i){
        cout << s[i];
    }
    return 0;
}

string lcs(string s1, string s2){
    int z[s1.length() + 1][s2.length() + 1];
    int con[s1.length() + 1][s2.length() + 1];
    for(int i = 0; i <= s1.length(); ++i){
        for(int j = 0; j <= s2.length(); ++j){
            if(i == 0 || j == 0){
                z[i][j] = 0;
            }
            else if(s1[i - 1] == s2[j - 1]){
                z[i][j] = z[i - 1][j - 1] + 1;
                con[i][j] = 1;
            }
            else{
                if(z[i - 1][j] > z[i][j - 1]){
                    z[i][j] = z[i - 1][j];
                    con[i][j] = -1;
                }
                else{
                    z[i][j] = z[i][j - 1];
                    con[i][j] = -2;
                }
            }
        }
    }
    int i = s1.length();
    int j = s2.length();
    string s;
    while(i > 0 && j > 0){
        if(con[i][j] == 1){
            s.push_back(s1[i - 1]);
            i = i - 1;
            j = j - 1;
        }
        else if(con[i][j] == -1)
            i = i - 1;
        else if(con[i][j] == -2)
            j = j - 1;
    }
    return s;
}