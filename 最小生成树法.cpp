#include<iostream>
#define MAX_SIZE 50
#define INFINITY 32767//表示无穷
using namespace std;
int top = -1;
struct Graph {
	int Edge[MAX_SIZE][MAX_SIZE];//存储权值
	int n, e;//顶点数和边数
};
struct Closest {
	int parentvex;//父节点
	int lowcost;
};

int FindMinDist(Closest * closest ,Graph &G) {
	int minv , min = INFINITY;
	for (int i = 0;i < G.n;i++) {
		if (closest[i].lowcost!=-1 && closest[i].lowcost < min ) {
			min = closest[i].lowcost;
			minv = i;
		}
	}
	return minv;
}
Closest *Prim(Graph& G, int start) {
	Closest* closest = new Closest[G.n];
	int minv, vcount = 0,i;
	for (int i = 0;i < G.n;i++) {
		closest[i].lowcost = G.Edge[start - 1][i];
		closest[i].parentvex = start - 1;
	}
	vcount++;
	closest[start - 1].parentvex = closest[start - 1].lowcost = -1;//起点已被收录 
	while (vcount < G.n) {
		minv = FindMinDist(closest, G);
		closest[minv].lowcost = -1;
		vcount++;
		for (i = 0;i < G.n;i++) {
			if (closest[i].lowcost != -1 && closest[i].lowcost > G.Edge[minv][i]) {
				closest[i].lowcost = G.Edge[minv][i];//更新每个顶点的lowcost 
				closest[i].parentvex = minv;
			}
		}
	}
	return closest;
}
/*欧拉通路：如果没有边可走，说明(作为终点的奇度顶点，最后只能从它进去而不能出来，始点相反)
改该点的边都走完了，输出
第一个输出的顶点必定是欧拉通路的终点，最后得到一个倒着的欧拉回路(起点在最后)

欧拉回路：如果没有边可走，说明(边不重的圈的并)
回到了最初的顶点*/
void dfs(int* Eulertmp,  Graph& Gtmp, int v) {
	Eulertmp[++top] = v;
	for (int i = 0;i < Gtmp.n;i++) {
		if (Gtmp.Edge[v][i] > 0) {
			Gtmp.Edge[v][i]--;
			Gtmp.Edge[i][v]--;
			dfs(Eulertmp,Gtmp,i);
			break;
		}
	}
}
void Fluery(int* Euler,Graph &Gtmp,const int start) {
	int Eulertmp[MAX_SIZE],v,vcount=0;
	Eulertmp[++top] = start - 1;
	while (top != -1) {
		bool flag = false;
		for (int i = 0;i < Gtmp.n;i++) {
			if (Gtmp.Edge[Eulertmp[top]][i] > 0) {
				flag = true;
				break;
			}
		}
		if (flag) {
			dfs(Eulertmp, Gtmp, Eulertmp[top--]);
		}
		else {
			Euler[vcount++] = Eulertmp[top--] + 1;
		}
	}
	for (v = 0;v < vcount;v++) {
		cout << Euler[v];
	}
	for ( v = 0;v < vcount;v++) {//删除重复的顶点，最后把起点加到最后 
		int k = 0;
		for (int j = v + 1;j < vcount;j++) {
			if (Euler[j] != Euler[v]) {
				Euler[j - k] = Euler[j];
			}
			else k++;
		}
		vcount -= k;
	}
	Euler[vcount] = start ;
}
void MST(Graph& G,const int start) {//从start顶点开始算法
	Closest *closest=Prim(G, start);
	int i, j;
	Graph Gtmp;
	for (i = 0;i < G.n;i++) {
		for (j = 0;j < G.n;j++) {
			Gtmp.Edge[i][j] = 0;//假设所有点都不连通
		}
	}
	Gtmp.n = G.n, Gtmp.e = G.n - 1;//创造一个新图，用来找哈密顿回路
	for (i = 0;i < G.n;i++) {
		if (closest[i].parentvex != -1) Gtmp.Edge[closest[i].parentvex][i] =  Gtmp.Edge[i][closest[i].parentvex] = 2;//添加平行边
	}
	delete[] closest;
	int Euler[MAX_SIZE], sumweight = 0;
	Fluery(Euler, Gtmp, start);
	cout << "所得的哈密顿回路是：";
	for (i = 0;i <G.n ;i++) {
		cout << Euler[i] << "-->";
		sumweight += G.Edge[Euler[i] - 1][Euler[i + 1] - 1];
	}
	cout << Euler[i] << endl;
	cout << "所得的权值为：" << sumweight << endl;
}
//最小生成树
int main()
{
	Graph G;
	int i, j, v;
	cout << "请输入完全图的顶点数:";
	cin >> G.n ;
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
	MST(G,v);
	return 0;
}
