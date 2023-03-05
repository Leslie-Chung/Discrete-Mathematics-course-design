#include<iostream>
#define MAX_SIZE 100
#define INFINITY 32767//表示无穷
using namespace std;
bool visited[MAX_SIZE];
struct Graph {
	int Edge[MAX_SIZE][MAX_SIZE];//存储权值
	int n, e;//顶点数和边数
};
int FindMinDist(Graph& G,int v) {
	int minv , min = INFINITY;
	for (int i = 0;i < G.n;i++) {
		if (!visited[i] && G.Edge[v][i] < min) {//如果i没有被访问过
			min = G.Edge[v][i];
			minv = i;
		}
	}
	return minv;
}
void NearestNeighbor(Graph& G,int start) {//从start顶点开始算法
	int pre, v, vcount = 0, sumweight = 0, * path = new int[G.n+1];
	path[vcount++] = start;
	pre = start-1;
	visited[start - 1] = true;
	while (vcount < G.n) {
		v = FindMinDist(G, pre);
		sumweight += G.Edge[pre][v];
		G.Edge[pre][v] = G.Edge[v][pre] = -1;//删除该边 
		visited[v] = true;
		path[vcount++] = v + 1;
		pre = v;
	}
	path[vcount] = start ;
	sumweight += G.Edge[pre][start-1];
	cout << "The best path is:" << endl;
	for (v = 0;v < G.n ;v++) cout << path[v] << "-->";
	cout << path[v] << endl;
	cout << "The distance of the best path is: " << sumweight << endl;
}
//最邻近法
int main()
{
	Graph G;
	int i, j, v;
	cout << "请输入完全图的顶点数:";
	cin >> G.n ;
	for (i = 0;i < G.n;i++) visited[i] = false;
	G.e = G.n * (G.n - 1) / 2;//完全图的边数
	for (i = 0;i < G.n;i++)
		for (j = 0;j < G.n;j++)
			G.Edge[i][j] = INFINITY;//初始化权值
	cout << endl;
	cout << "请输入始点和终点及其权值(权值不是∞的边，无需重复输入，如1 2 30、2 1 30)" << endl;
	for (i = 0;i < G.e;i++) {
		cin >> v >> j;
		cin >> G.Edge[v-1][j-1];
		G.Edge[j - 1][v - 1]= G.Edge[v - 1][j - 1];
	}
	cout << "请输入初始顶点:";
	cin >> v;
	cout << endl;
	NearestNeighbor(G,v);
	return 0;
}
