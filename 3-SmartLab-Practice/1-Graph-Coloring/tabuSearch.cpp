#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <climits>
#include <ctime>
using namespace std;

int N = 0;
int K = 0;

int *S;
int *H;
int **E;
int **M;
int **MB;
int **T;
int **TB;
int *Color;
int conficting = 0;

void InitialGraphColor();
void InitialConficting();
void InitialConfictingTable();
void Readfile(char filename[]);
void doMove(vector<int> Move, int num, size_t tabuStep);

int degreeSubgraph(int node);
int getMaxColor(int* saturationDegreeList);
int saturationDegree(int node);

vector<int> getNeighbor(int point);
vector<int> cacluteConfitingPairs();
vector<int> FindBestMove(vector<int> ConfictingPairs, int hisBestCon, bool localBest, int num);

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Usage: exe filename colorNum" << endl;
        return 1;
    }
    clock_t begin = clock();
    int num = 0;
    K = atoi(argv[2]);
    Color = new int [K] {0};
    srand(static_cast<unsigned int>(time(0)));

    Readfile(argv[1]);
    InitialGraphColor();
    InitialConfictingTable();
    InitialConficting();
    int before = conficting;
    vector<int> move(4, 0);
    vector<int> confictingPairs;
    cout << "begin iter: " << endl;
    int memory = conficting;
    int hisBestCon = conficting;
    bool localBest = 0;
    while(num < INT32_MAX){
        confictingPairs = cacluteConfitingPairs();
        move = FindBestMove(confictingPairs, hisBestCon, localBest, num);
        doMove(move, num, confictingPairs.size());
        if(move[2] == 0){
            localBest = 1;
            cout << num << " local best " << conficting << endl;
        }else{
            localBest = 0;
        }
        if(conficting < hisBestCon) {
            hisBestCon = conficting;
            for(int i = 0; i < N; ++i){
                H[i] = S[i];
            }
            for(int i = 0; i < N; ++i){
                for(int j = 0; j < K; ++j){
                    MB[i][j] = M[i][j];
                }
            }
            for(int i = 0; i < N; ++i){
                for(int j = 0; j < K; ++j){
                    TB[i][j] = T[i][j];
                }
            }
        }
        if(conficting == 0) break;
        if(num % 10000 == 9999){
            if(conficting >= memory) {
                for(int i = 0; i < N; ++i){
                    S[i] = H[i];
                }
                for(int i = 0; i < N; ++i){
                    for(int j = 0; j < K; ++j){
                        M[i][j] = MB[i][j];
                    }
                }
                for(int i = 0; i < N; ++i){
                    for(int j = 0; j < K; ++j){
                        T[i][j] = TB[i][j];
                    }
                }
                conficting = hisBestCon;
            }
            memory = conficting;
        }
        num += 1;
    }


    for(int i = 0; i < N; ++i){
        S[i] = H[i];
    }
    for(int i = 0; i < N; ++i){
        for(int j = 0; j < K; ++j){
            M[i][j] = MB[i][j];
            T[i][j] = TB[i][j];
        }
    }
    conficting = hisBestCon;
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
    cout << "time:" << clock() - begin << "ms" << endl;
    return 0;
}
void Readfile(char filename[]){
    ifstream input (filename);
    string line;
    int x, y;
    while(!input.eof()){
        getline(input, line);
        if(line[0] == 'p'){
            N = stoi(line.substr(line.find_first_of("0123456789"), 
                line.find_last_of(" ") - line.find_first_of("0123456789")));
            S = new int  [N] {0};
            H = new int  [N] {0};
            E = new int *[N];
            M = new int *[N];
            MB= new int *[N];
            T = new int *[N];
            TB= new int *[N];
            for(int i = 0; i < N; ++i){
                E[i] = new int [N] {0};
            }
            for(int i = 0; i < N; ++i){
                M[i] = new int [K] {0};
            }
            for(int i = 0; i < N; ++i){
                T[i] = new int [K] {0};
            }
            for(int i = 0; i < N; ++i){
                TB[i] = new int [K] {0};
            }
            for(int i = 0; i < N; ++i){
                MB[i] = new int [K] {0};
            }
        }
        else if(line[0] == 'e'){
            x = stoi(line.substr(line.find_first_of("0123456789"), 
                line.find_last_of(" ") - line.find_first_of("0123456789")));
            y = stoi(line.substr(line.find_last_of(" ") + 1));
            E[x - 1][y - 1] = 1;
            E[y - 1][x - 1] = 1;
        }
        else{
            continue;
        }
    }
}
void InitialGraphColor(){
    int* degree = new int [N];
    int* listIndex = new int [N];
    int* saturationDegreeList = new int [N];
    int thisMaxElm;
    vector<int> numColor(K, 0);
    int key, index;
    int numnum = 0;
    for(int i = 0; i < N; ++i){
        S[i] = -1;
        saturationDegreeList[i] = 0;
        degree[i] = 0;
        listIndex[i] = i;
        for(int j = 0; j < N; ++j){
            if(E[i][j] == 1)
                degree[i]++;
        }
    }
    for(int i = 1; i < N; ++i){
        key = degree[i];
        index = listIndex[i];
        int j = i - 1;
        while(j >= 0 && key > degree[j]){
            degree[j + 1] = degree[j];
            listIndex[j + 1] = listIndex[j];
            j = j - 1;
        }
        degree[j + 1] = key;
        listIndex[j + 1] = index;
    }
    S[listIndex[0]] = 0;
    numColor[0] = 1;
    numnum = 1;
    while(numnum < N){
        int cycle = 0;
        for(int i = 0; i < N; ++i){
            saturationDegreeList[i] = saturationDegree(i);
        }
        thisMaxElm = getMaxColor(saturationDegreeList);
        for(int i = 0; i < K; ++i){
            if(numColor[i] < numColor[cycle]){
                cycle = i;
            }
        }
        S[thisMaxElm] = cycle;
        numColor[cycle]++;
        numnum++;
    }
    for(int i = 0; i < N; ++i){
        cout << S[i] << " ";
        if(i == 10){
            cout << endl;
        }
    }

    // for(int i = 0; i < N; ++i){
    //     S[i] = rand() % K;
    // }
}

int degreeSubgraph(int node){
    int res = 0;
    for(int i = 0; i < N; ++i){
        if(E[node][i] == 1 && S[i] == -1){
            res++;
        }
    }
    return res;
}
int getMaxColor(int* saturationDegreeList){
    vector<int> MaxSet;
    vector<int> resSet;
    int MaxElm = 0;
    int deg;
    int maxDeg;
    for(int i = 0; i < N; ++i){
        if(S[i] == -1 && (MaxSet.size() == 0 || saturationDegreeList[i] > saturationDegreeList[MaxElm])){
            MaxSet.clear();
            MaxSet.push_back(i);
            MaxElm = i;
        }else if(S[i] == -1 && saturationDegreeList[i] == saturationDegreeList[MaxElm]){
            MaxSet.push_back(i);
        }else{
            continue;
        }
    }
    for(int i = 0; i < MaxSet.size(); ++i){
        deg = degreeSubgraph(MaxSet[i]);
        if(resSet.size() == 0 || deg > maxDeg){
            maxDeg = deg;
            resSet.clear();
            resSet.push_back(MaxSet[i]);
        }else if(deg == maxDeg){
            resSet.push_back(MaxSet[i]);
        }else{
            continue;
        }
    }
    return resSet[rand() % resSet.size()];
}
int saturationDegree(int node){
    int res = 0;
    int* colorList = new int [K];
    for(int i = 0; i < K; ++i){
        colorList[i] = 0;
    }
    for(int j = 0; j < N; ++j){
        if(E[node][j] == 1 && S[j] != -1 && colorList[S[j]] == 0){
            res++;
            colorList[S[j]] = 1;
        }
    }
    return res;
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

void doMove(vector<int> Move, int num, size_t tabuStep){
    int point  = Move[0];
    int color  = Move[1];
    int bestDes= Move[2];
    int fl     = Move[3];
    int oldColor=S[point];
    vector<int> neighbor = getNeighbor(point);
    for(auto it = neighbor.begin(); it != neighbor.end(); ++it){
        M[*it][oldColor] -= 1;
        M[*it][color] += 1;
    }
    if(bestDes < 0){
        T[point][oldColor] = rand() % 7 + num + tabuStep / 2;
    }else{
        T[point][oldColor] = rand() % 7 + num + tabuStep / 2;
    }
    S[point] = color;
    conficting -= bestDes;
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
vector<int> FindBestMove(vector<int> ConfictingPairs, int hisBestCon, bool localBest, int num){
    int thisDes = 0, isFirst = 1;
    int fish = 0;
    int oldColor = 0, MO;
    vector<int> move(4, 0);
    for(auto it = ConfictingPairs.begin(); it != ConfictingPairs.end(); ++it){
        oldColor = S[*it];
        MO = M[*it][oldColor];
        for(int k = 0; k < K; ++k){
            thisDes = 2 * (MO - M[*it][k]);
            if(T[*it][k] >= num && !localBest){
                if(conficting - thisDes < hisBestCon && (thisDes >= move[2])){
                    T[*it][k] = 0;
                    if(thisDes == move[2]){
                        fish++;
                        if(rand() % fish == 0)
                            move = {*it, k, thisDes, 1};
                    }else{
                        fish = 1;
                        move = {*it, k, thisDes, 1};
                    }
                }
            }else if(localBest && thisDes < 0){
                fish++;
                if(rand() % fish == 0)
                    move = {*it, k, thisDes, 2};
            }else if(!localBest && (thisDes >= move[2] || isFirst == 1)){
                isFirst = 0;
                if(thisDes == move[2]){
                    fish++;
                    if(rand() % fish == 0)
                        move = {*it, k, thisDes, 3};
                }else {
                    fish = 1;
                    move = {*it, k, thisDes, 3};
                }
            }else{
                continue;
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