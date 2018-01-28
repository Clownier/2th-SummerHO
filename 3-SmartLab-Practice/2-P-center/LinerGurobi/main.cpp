#include "gurobi_c++.h"
using namespace std;

vector<vector<int>> Graph;
int N = 0;
int P = 0;
void printSolution(GRBModel& model, int nCategories, int nFoods,
	GRBVar* buy, GRBVar* nutrition) throw(GRBException);
void readFile(string filename);
void dijkstra(int sourceNode);
void floyd();
int main(int argc,
	char *argv[])
{
	GRBEnv* env = 0;
	GRBVar F;
	GRBVar* X = 0;
	GRBVar** Y = 0;
	try
	{
		cout << argv[1] << endl;
		readFile(argv[1]);
		cout << "read file success!" << endl;
		for (int i = 0; i < N; ++i) {
			dijkstra(i);
		}
		//floyd();
		cout << "node :" << N << " center:" << P << endl;
		X = new GRBVar[N];
		Y = new GRBVar*[N];
		for (int i = 0; i < N; ++i) {
			Y[i] = new GRBVar[N];
		}
		// Model
		env = new GRBEnv();
		GRBModel model = GRBModel(*env);
		model.set(GRB_StringAttr_ModelName, "Pcenter");

		F = model.addVar(0, INT32_MAX, 0, GRB_INTEGER, "F");
		for (int i = 0; i < N; ++i) {
			string Xname = string("X[") + char(i + '0') + "]";
			X[i] = model.addVar(0, 1, 0, GRB_BINARY, Xname);
			for (int j = 0; j < N; ++j) {
				string Yname= string("Y[") + char(i + '0') + "]" + "[" + char(j + '0') + "]";
				Y[i][j] = model.addVar(0, 1, 0, GRB_BINARY, Yname);
			}
		}
		cout << "add vars success!" << endl;
		GRBLinExpr sumX = 0;
		GRBLinExpr sumY = 0;
		for (int i = 0; i < N; ++i)
		{
			sumY = 0;
			sumX += X[i];
			for (int j = 0; j < N; ++j)
			{
				sumY += Y[j][i];
				model.addConstr(Y[i][j] <= X[i]);
				model.addConstr(Y[i][j] * Graph[i][j] <= F);
			}
			model.addConstr(sumY == 1);
		}
		model.addConstr(sumX == P);
		cout << "add constr success!" << endl;
		model.setObjective( 1 * F, GRB_MINIMIZE);
		// Solve
		model.optimize();

	}
	catch (GRBException e)
	{
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (...)
	{
		cout << "Exception during optimization" << endl;
	}
	delete env;
	system("pause");
	return 0;
}

void readFile(string filename){
	ifstream input(filename);
	int in1, in2, in3;
	input >> in1 >> in2 >> in3;
	N = in1;
	P = in3;
	for (int i = 0; i < N; ++i) {
		vector<int> temp;
		for (int j = 0; j < N; ++j) {
			temp.push_back(-1);
		}
		Graph.push_back(temp);
	}
	while (!input.eof()) {
		input >> in1 >> in2 >> in3;
		Graph[in1 - 1][in2 - 1] = in3;
		Graph[in2 - 1][in1 - 1] = in3;
	}
}

void dijkstra(int sourceNode) {
	int* Distance;
	int* S;
	S = new int[N];
	Distance = new int[N];
	for (int i = 0; i < N; ++i) {
		S[i] = 0;
		Distance[i] = -1;
	}
	int nodeNum = 0, thisMin, minID;
	Distance[sourceNode] = 0;
	while (nodeNum < N) {
		thisMin = -1; minID = -1;
		for (int i = 0; i < N; ++i) {
			if (S[i] == 0 && Distance[i] != -1 && (Distance[i] < thisMin || thisMin == -1)) {
				thisMin = Distance[i];
				minID = i;
			}
		}
		S[minID] = 1;
		for (int i = 0; i < N; ++i) {
			if (S[i] == 0 && Graph[minID][i] != -1
				&& (Distance[i] == -1 || Graph[minID][i] + Distance[minID] < Distance[i])) {
				Distance[i] = Graph[minID][i] + Distance[minID];
			}
		}
		nodeNum += 1;
	}
	for (int i = 0; i < N; ++i) {
		Graph[sourceNode][i] = Distance[i];
	}
}


/*
	Dis[i][j] = min(Dis[i][j], Dis[i][k] + Dis[k][j]) for_each  0<= k < N
	T = O(n*n*n)
	S = O(n * n)
	shortestPath(i, j, 0) = w(i, j);
	shortestPath(i, j, k + 1) = Min(shorestPath(i, j, k), shorestPath(i, k + 1, k) + shorestPath(k + 1, j, k));
*/
void floyd() {
	for (int i = 0; i < N; ++i) {
		Graph[i][i] = 0;
	}
	for (int k = 0; k < N; ++k) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				if ((Graph[i][j] == -1 && Graph[i][k] != -1 && Graph[k][j] != -1)
					|| (Graph[i][j] > Graph[i][k] + Graph[k][j] && Graph[i][k] != -1 && Graph[k][j] != -1)) {
					Graph[i][j] = Graph[i][k] + Graph[k][j];
					Graph[j][i] = Graph[i][j];
				}
			}
		}
	}
}