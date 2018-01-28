#include <iostream>
#include <fstream>
#include <climits>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <set>

using namespace std;
int N = 0;
int P = 0;

int* C;
int* TI;
int* TD;
int** D;
int** F;
int** DT;
int** FT;
int** Graph;
set<int> Pcenter;
set<int> PcenterTemp;

void copyDTFT();
void initialDFtable();
void initialPcenter();
void addKcenter(int k, int num);
void addKcenterTemp(int k);
void deleteKcenter(int k, int num);
void deleteKcenterTemp(int k);
void dijkstra(int sourceNode);
void Readfile(char filename[]);

vector<int> findWorestedge();
vector<int> findWorestedgeTemp();

int main(int argv, char *argc[]){
    int num = 0;
    int beginWor = 0;
    Readfile(argc[1]);
    for(int i = 0; i < N; ++i){
        dijkstra(i);
    }
    cout << "node:" << N << " center:" << P << endl;
    srand(static_cast<unsigned int>(time(0)));
    
    initialPcenter();
    initialDFtable();
    cout << "initial success!" << endl;
    TI = new int [N];
    TD = new int [N];
    for(int i = 0; i < N; ++i){
        TI[i] = 0;
        TD[i] = 0;
    }
    vector<int> move(2, 0);
    vector<int> worstedge(2, 0);
    vector<int> tempworstedge(2, 0);

    vector<vector<int>> moves;
    int localBest = 0;
    int flag = 0;
    while(num < 10000){
        int how = 1;
        worstedge = findWorestedge();
        beginWor = worstedge[1];
        cout << num << " " << worstedge[1] << endl;
        for(int i = 0; i < N; ++i){
            copyDTFT();
            how = 0;
            if(TI[i] <= num && Graph[worstedge[0]][i] <= worstedge[1]){
                cout << " addTemp" << endl;
                addKcenterTemp(i);
                how = 1;
                for(auto it = PcenterTemp.begin(); it != PcenterTemp.end();){
                    int k = *it;
                    if(TD[k] > num){
                        break;
                    }
                    it++;
                    cout << " deleteTemp" << endl;
                    deleteKcenterTemp(k);
                    cout << " findTemp" << endl;
                    tempworstedge = findWorestedgeTemp();
                    if(localBest){
                        move = {i, k};
                        moves.push_back(move);
                    }else if(tempworstedge[1] <= worstedge[1]){
                        move = {i, k};
                        if(tempworstedge[1] < worstedge[1]){
                            moves.clear();
                        }
                        moves.push_back(move);
                        worstedge = tempworstedge;
                    }
                    cout << " addTemp" << endl;
                    addKcenterTemp(k);
                    cout << "P: " << PcenterTemp.size() << endl;
                }
                deleteKcenterTemp(i);
            }
        }
        if(beginWor <= worstedge[1] && localBest == 0) localBest = 1;
        else localBest = 0;
        int coin = rand() % moves.size();
        move = moves[coin];
        cout << " add" << endl;
        addKcenter(move[0], num);
        cout << " delete" << endl;
        deleteKcenter(move[1], num);
        num++;
    }

    for(int i = 0; i < N; ++i){
        if(C[i] == 1){
            cout << i << " ";
        }
    }
    cout << endl;
    return 0;
}

void copyDTFT(){
    for(int i = 0; i < N; ++i){
        DT[i][0] = D[i][0];
        DT[i][1] = D[i][1];
    }
    for(int i = 0; i < N; ++i){
        FT[i][0] = F[i][0];
        FT[i][1] = F[i][1];
    }
    PcenterTemp = Pcenter;
}
void dijkstra(int sourceNode){
    int num = 0;
    int thisMin = -1;
    int minID = -1;
    int *T = new int [N] {0};
    int *Distance = new int [N] {-1};
    for(int i = 1; i < N; ++i){
        Distance[i] = -1;
        T[i] = 0;
    }
    Distance[sourceNode] = 0;
    while(num < N){
        thisMin = -1; minID = -1;
        for(int i = 0; i < N; ++i){
            if(T[i] == 0 && Distance[i] != -1 && (Distance[i] < thisMin || thisMin == -1)){
                thisMin = Distance[i];
                minID = i;
            }
        }
        T[minID] = 1;
        for(int i = 0; i < N; ++i){
            if(T[i] == 0 && Graph[minID][i] != -1 && (Distance[i] == -1 || Graph[minID][i] + Distance[minID] < Distance[i])){
                Distance[i] = Graph[minID][i] + Distance[minID];
            }
        }
        num += 1;
    }
    for(int i = 0; i < N; ++i){
        Graph[sourceNode][i] = Distance[i];
    }
}

void initialPcenter(){
    int seed = 0;
    while(Pcenter.size() < P){
        seed = rand() % N;
        if(C[seed] != 1){
            C[seed] = 1;
            Pcenter.insert(seed);
        }
    }
}

void initialDFtable(){
    vector<int> nearest, secNearest;
    for(int i = 0; i < N; ++i){
        auto beg = Pcenter.begin();
        int fir = *beg;
        beg++;
        int sed = *beg;
        beg++;
        if(Graph[i][fir] > Graph[i][sed]){
            nearest.push_back(sed);
            secNearest.push_back(fir);
        }else if(Graph[i][fir] == Graph[i][sed]){
            nearest.push_back(sed);
            nearest.push_back(fir);
            secNearest = nearest;
        }else {
            nearest.push_back(fir);
            secNearest.push_back(sed);
        }
        for(auto it = beg; it != Pcenter.end(); ++it){
            int dis = Graph[i][*it];
            if(dis < Graph[i][nearest[0]]){
                secNearest = nearest;
                nearest.clear();
                nearest.push_back(*it);
            }else if(dis == Graph[i][nearest[0]]){
                if(Graph[i][nearest[0]] == Graph[i][secNearest[0]]){
                    nearest.push_back(*it);
                    secNearest.push_back(*it);
                }else{
                    nearest.push_back(*it);
                }
            }else if(dis < Graph[i][secNearest[0]]){
                secNearest.clear();
                secNearest.push_back(*it);
            }else if(dis == Graph[i][secNearest[0]]){
                secNearest.push_back(*it);
            }else{
                continue;
            }
        }
        int randNear, randSec;
        randNear = rand() % nearest.size();
        randSec = rand() % secNearest.size();
        while(nearest[randNear] == secNearest[randSec]){
            randNear = rand() % nearest.size();
            randSec = rand() % secNearest.size();
        }
        F[i][0] = nearest[randNear]; 
        F[i][1] = secNearest[randSec];
        D[i][0] = Graph[i][F[i][0]];
        D[i][1] = Graph[i][F[i][1]];
    }
}

vector<int> findWorestedge(){
    vector<vector<int>> worests;
    vector<int> worest(2, 0);
    for(int i =0; i < N; ++i){
        if(worest[1] < D[i][0]){
            worest[0] = i;
            worest[1] = D[i][0];
            worests.clear();
            worests.push_back(worest);
        }else if(worest[1] == D[i][0]){
            worest[0] = i;
            worests.push_back(worest);
        }else{
            continue;
        }
    }
    return worests[rand() % worests.size()];
}

vector<int> getNeighborBlowK(int node, int k){
    vector<int> neighbor;
    for(int i = 0; i < N; ++i){
        if(Graph[node][i] <= k){
            neighbor.push_back(i);
        }
    }
    return neighbor;
}

void addKcenter(int k, int num){
    C[k] = 1;
    TD[k] = num + rand() % 10;
    Pcenter.insert(k);
    for(int i = 0; i < N; ++i){
        int dis = Graph[k][i];
        if(dis < D[i][0]){
            if(D[i][0] == D[i][1]){
                if(rand() % 2 == 1){
                    F[i][1] = F[i][0];
                }
            }else{
                F[i][1] = F[i][0];
                D[i][1] = D[i][0];
            }
            F[i][0] = k;
            D[i][0] = dis;
        }else if(dis == D[i][0]){
            if(D[i][0] == D[i][1]){
                if(rand() % 2 == 1){
                    F[i][1] = F[i][0];
                }
            }else{
                F[i][1] = F[i][0];
                D[i][1] = D[i][0];
            }
            if(rand() % 2 == 1){
                F[i][0] = k;
                D[i][0] = dis;
            }
        }else if(dis < D[i][1]){
            F[i][1] = k;
            D[i][1] = dis;
        }else if(dis == D[i][1]){
            if(rand() % 1 == 1){
                F[i][1] = k;
            }
        }else {
            continue;
        }
    }
}

void deleteKcenter(int k, int num){
    TI[k] = num + rand() % 10;
    C[k] = 0;
    Pcenter.erase(k);
    auto beg = Pcenter.begin();
    int fir = *beg;
    beg++;
    int sed = *beg;
    beg++;
    for(int i = 0; i < N; ++i){
        vector<int> secNearest, nearest;
        int temp1 = 0, temp2 = 0;
        if(F[i][0] == k || F[i][1] == k){
            if(Graph[i][fir] > Graph[i][sed]){
                nearest.push_back(sed);
                secNearest.push_back(fir);
            }else if(Graph[i][fir] == Graph[i][sed]){
                nearest.push_back(sed);
                nearest.push_back(fir);
                secNearest = nearest;
            }else {
                nearest.push_back(fir);
                secNearest.push_back(sed);
            }
            for(auto it = beg; it != Pcenter.end(); ++it){
                int dis = Graph[i][*it];
                if(dis < Graph[i][nearest[0]]){
                    secNearest = nearest;
                    nearest.clear();
                    nearest.push_back(*it);
                }else if(dis == Graph[i][nearest[0]]){
                    if(Graph[i][nearest[0]] == Graph[i][secNearest[0]]){
                        nearest.push_back(*it);
                        secNearest.push_back(*it);
                    }else{
                        nearest.push_back(*it);
                    }
                }else if(dis < Graph[i][secNearest[0]]){
                    secNearest.clear();
                    secNearest.push_back(*it);
                }else if(dis == Graph[i][secNearest[0]]){
                    secNearest.push_back(*it);
                }else{
                    continue;
                }
            }
            int randNear, randSec;
            randNear = rand() % nearest.size();
            randSec = rand() % secNearest.size();
            while(nearest[randNear] == secNearest[randSec]){
                randNear = rand() % nearest.size();
                randSec = rand() % secNearest.size();
            }
            F[i][0] = nearest[randNear]; 
            F[i][1] = secNearest[randSec];
            D[i][0] = Graph[i][F[i][0]];
            D[i][1] = Graph[i][F[i][1]];
        }
    }
}

void Readfile(char filename[]){
    ifstream input (filename);
    int f = 0, s = 0, t = 0;
    input >> f >> s >> t;
    N = f;
    P = t;
    Graph = new int *[N];
    D = new int *[N];
    DT= new int *[N];
    F = new int *[N];
    FT= new int *[N];
    C = new int [N];
    for(int j = 0; j < N; ++j){
        Graph[j] = new int [N];
    }
    for(int j = 0; j < N; ++j){
        D[j] = new int [2];   
    }
    for(int j = 0; j < N; ++j){
        DT[j] = new int [2];
    }
    for(int j = 0; j < N; ++j){
        F[j] = new int [2];
    }
    for(int j = 0; j < N; ++j){
        FT[j] = new int [2];
    }
    for(int j = 0; j < N; ++j){
        C[j] = 0;
    }
    for(int j = 0; j < N; ++j){
        for(int k = 0; k < N; ++k){
            Graph[j][k] = -1;
        } 
    }   
    while(!input.eof()){
        input >> f >> s >> t;
        Graph[f - 1][s - 1] = t;
        Graph[s - 1][f - 1] = t;
    }
}

void addKcenterTemp(int k){
    PcenterTemp.insert(k);
    for(int i = 0; i < N; ++i){
        int dis = Graph[k][i];
        if(dis < DT[i][0]){
            if(DT[i][0] == DT[i][1]){
                if(rand() % 2 == 1){
                    FT[i][1] = FT[i][0];
                }
            }else{
                FT[i][1] = FT[i][0];
                DT[i][1] = DT[i][0];
            }
            FT[i][0] = k;
            DT[i][0] = dis;
        }else if(dis == DT[i][0]){
            if(DT[i][0] == DT[i][1]){
                if(rand() % 2 == 1){
                    FT[i][1] = FT[i][0];
                }
            }else{
                FT[i][1] = FT[i][0];
                DT[i][1] = DT[i][0];
            }
            if(rand() % 2 == 1){
                FT[i][0] = k;
                DT[i][0] = dis;
            }
        }else if(dis < DT[i][1]){
            FT[i][1] = k;
            DT[i][1] = dis;
        }else if(dis == DT[i][1]){
            if(rand() % 1 == 1){
                FT[i][1] = k;
            }
        }else {
            continue;
        }
    }
}

void deleteKcenterTemp(int k){
    PcenterTemp.erase(k);
    auto beg = PcenterTemp.begin();
    int fir = *beg;
    beg++;
    int sed = *beg;
    beg++;
    cout << " ????" << endl;
    for(int i = 0; i < N; ++i){
        vector<int> secNearest, nearest;
        int temp1 = 0, temp2 = 0;
        if(FT[i][0] == k || FT[i][1] == k){
            if(Graph[i][fir] > Graph[i][sed]){
                nearest.push_back(sed);
                secNearest.push_back(fir);
            }else if(Graph[i][fir] == Graph[i][sed]){
                nearest.push_back(sed);
                nearest.push_back(fir);
                secNearest = nearest;
            }else {
                nearest.push_back(fir);
                secNearest.push_back(sed);
            }
            for(auto it = beg; it != PcenterTemp.end(); ++it){
                int dis = Graph[i][*it];
                if(dis < Graph[i][nearest[0]]){
                    secNearest = nearest;
                    nearest.clear();
                    nearest.push_back(*it);
                }else if(dis == Graph[i][nearest[0]]){
                    if(Graph[i][nearest[0]] == Graph[i][secNearest[0]]){
                        nearest.push_back(*it);
                        secNearest.push_back(*it);
                    }else{
                        nearest.push_back(*it);
                    }
                }else if(dis < Graph[i][secNearest[0]]){
                    secNearest.clear();
                    secNearest.push_back(*it);
                }else if(dis == Graph[i][secNearest[0]]){
                    secNearest.push_back(*it);
                }else{
                    continue;
                }
            }
            int randNear, randSec;
            randNear = rand() % nearest.size();
            randSec = rand() % secNearest.size();
            while(nearest[randNear] == secNearest[randSec]){
                randNear = rand() % nearest.size();
                randSec = rand() % secNearest.size();
                cout << " 1" << endl;
            }
            FT[i][0] = nearest[randNear]; 
            FT[i][1] = secNearest[randSec];
            DT[i][0] = Graph[i][FT[i][0]];
            DT[i][1] = Graph[i][FT[i][1]];
        }
    }
}

vector<int> findWorestedgeTemp(){
    vector<vector<int>> worests;
    vector<int> worest(2, 0);
    for(int i = 0; i < N; ++i){
        if(F[i][0] != FT[i][0]){
            if(worest[1] < DT[i][0]){
                worest[0] = i;
                worest[1] = DT[i][0];
                worests.clear();
                worests.push_back(worest);
            }else if(worest[1] == DT[i][0]){
                worest[0] = i;
                worests.push_back(worest);
            }else{
                continue;
            }
        }
    }
    return worests[rand() % worests.size()];
}