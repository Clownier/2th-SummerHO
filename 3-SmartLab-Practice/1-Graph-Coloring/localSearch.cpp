#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
using namespace std;

int N = 0;
int K = 0;

int **E;
int *S;
int **M;
int *Color;
int conficting;

void InitialGraphColor();
void Readfile(char filename[]);
void InitialConfictingTable();
void InitialConficting();
vector<int> cacluteConfitingPairs();
vector<int> FindBestMove(vector<int> ConfictingPairs);
void doMove(vector<int> Move);
vector<int> getNeighbor(int point);

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Usage: exe filename" << endl;
        return 1;
    }
    K = atoi(argv[2]);
    int num = 0;
    Color = new int [K] {0};
    Readfile(argv[1]);
    InitialGraphColor();
    InitialConfictingTable();
    InitialConficting();
    int before = conficting;
    vector<int> move(3, 0);
    vector<int> confictingPairs;
    while(1){
        confictingPairs = cacluteConfitingPairs();
        move = FindBestMove(confictingPairs);
        if(move[2] == 0) break;
        doMove(move);
        num += 1;
    }
    cout << "result color : ";
    for(int i = 0; i < N; ++i){
        cout << S[i] << " ";
        if(i % 10 == 9)
            cout << endl;
    }
    cout << endl;
    cout << "before iter the result is: " << before << endl;
    cout << "after " << num << " iter rechive the local best result: " << conficting << endl;
    cout << "average improve is: " << ((before - conficting) * 1.0)/ num << endl;
    InitialConficting();
    cout << "check the result  " << conficting / 2 << endl;
    return 0;
}
void Readfile(char filename[]){
    ifstream input (filename);
    int x, y;
    string line;
    while(!input.eof()){
        getline(input, line);
        if(line[0] != 'p' && line[0] != 'e'){
            continue;
        }
        if(line[0] == 'p'){
            N = stoi(line.substr(line.find_first_of("0123456789"), 
                line.find_last_of(" ") - line.find_first_of("0123456789")));
            S = new int [N] {0};
            E = new int *[N];
            for(int i = 0; i < N; ++i){
                E[i] = new int [N] {0};
            }
            M = new int *[N];
            for(int i = 0; i < N; ++i){
                M[i] = new int [K] {0};
            }
        }
        if(line[0] == 'e'){
            x = stoi(line.substr(line.find_first_of("0123456789"), 
                line.find_last_of(" ") - line.find_first_of("0123456789")));
            y = stoi(line.substr(line.find_last_of(" ") + 1));
            E[x - 1][y - 1] = 1;
            E[y - 1][x - 1] = 1;
        }
    }
}
void InitialGraphColor(){
    for(int i = 0; i < N; ++i){
        S[i] = rand() % K;
    }
}

void InitialConfictingTable(){
    for(int i = 0; i < N; ++i){
        vector<int> neighbor = getNeighbor(i);
        for(int k = 0; k < K; ++k){
            for(int j = 0; j < neighbor.size(); ++j){
                if(S[neighbor[j]] == k)
                    M[i][k] += 1;
            }
        }
    }
}

void InitialConficting(){
    for(int i = 0; i < N; ++i){
        for(int j = 0; j < N; ++j){
            if(E[i][j] == 1 && S[i] == S[j])
                conficting += 1;
        }
    }
}

void doMove(vector<int> Move){
    int point  = Move[0];
    int color  = Move[1];
    int bestDes= Move[2];
    vector<int> neighbor = getNeighbor(point);
    for(int i = 0; i < neighbor.size(); ++i){
        M[neighbor[i]][S[point]] -= 1;
        M[neighbor[i]][color] += 1;
    }
    S[point] = color;
    conficting -= bestDes;
    int x = 0;
    for(int i = 0; i < N; ++i){
        for(int j = 0; j < N; ++j){
            if(E[i][j] == 1 && S[i] == S[j])
                x += 1;
        }
    }
    if(x == conficting) {
        cout << "true" << endl;
    }
    else{
        cout << "false" << endl;
    }
}

vector<int> cacluteConfitingPairs(){
    vector<int> pairs;
    for(int i = 0; i < N; ++i){
        if(M[i][S[i]] != 0){
            pairs.push_back(i);
        }
    }
    return pairs;
}
vector<int> FindBestMove(vector<int> ConfictingPairs){
    int bestDes = 0, thisDes = 0;
    vector<int> move(3, 0);
    int oldColor = 0;
    int des = 0, inc = 0;
    for(int i = 0; i < ConfictingPairs.size(); ++i){
        oldColor = S[ConfictingPairs[i]];
        for(int k = 0; k < K; ++k){
            thisDes = 2 *(M[ConfictingPairs[i]][oldColor] - M[ConfictingPairs[i]][k]);
            if(thisDes > bestDes){
                bestDes = thisDes;
                move = {ConfictingPairs[i], k, bestDes};
            }
        }
    }
    return move;
}

vector<int> getNeighbor(int point){
    vector<int> res;
    for(int i = 0; i < N; ++i){
        if(E[point][i] == 1)
            res.push_back(i);
    }
    return res;
}


// bool IsBestResult(int ConfictingTable[][K], int S[]){
//     for(int i = 0; i < N; ++i){
//         if(ConfictingTable[i][S[i]] > 0)
//             return false;
//     }
//     return true;
// }
