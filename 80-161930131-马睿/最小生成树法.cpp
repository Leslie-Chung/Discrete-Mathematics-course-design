#include<iostream>
#include<string.h>
#define MAX_SIZE 50
#define INFINITY 32767//表示无穷
using namespace std;
int top = -1;
struct Graph {
	int Edge[MAX_SIZE][MAX_SIZE];//存储权值
	int n, e;//顶点数和边数
};
struct Closest {
	int parentvex;//前驱顶点
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
	closest[start - 1].parentvex = closest[start - 1].lowcost = -1;
	while (vcount < G.n) {
		minv = FindMinDist(closest, G);
		closest[minv].lowcost = -1;
		vcount++;
		for (i = 0;i < G.n;i++) {
			if (closest[i].lowcost != -1 && closest[i].lowcost > G.Edge[minv][i]) {
				closest[i].lowcost = G.Edge[minv][i];
				closest[i].parentvex = minv;
			}
		}
	}
	return closest;
}
/*因为必定存在欧拉回路，所以一定是边不重的圈的并，因此从一个顶点出发直接走（无论是不是桥），直到某个顶点没有边可走了（绝对是第一个出现的） 
说明回到了起点（也是终点），则将该顶点及其之前的遍历过的顶点（且该顶点无边可走了）放入栈中，一直退回到有边可以走的顶点（该顶点为某些圈的公共顶点），那就继续走，直到遍历完所有边
最后就会得到一条欧拉回路*/
void dfs(int* Euler,  Graph& Gtmp, int v) {
	for (int i = 0;i < Gtmp.n;i++) {
		if (Gtmp.Edge[v][i] > 0) {
			Gtmp.Edge[v][i]--;
			Gtmp.Edge[i][v]--;
			dfs(Euler,Gtmp,i);
		}
	}
	Euler[++top] = v+1;
}
void Fluery(int* Euler,Graph &Gtmp,const int start) {
	int v;
	dfs(Euler, Gtmp, start - 1);
	for ( v = 0;v < top;v++) {
		int k = 0;
		for (int j = v + 1;j < top;j++) {
			if (Euler[j] != Euler[v]) {
				Euler[j - k] = Euler[j];
			}
			else k++;
		}
		top -= k;
	}
	Euler[top] = start ;
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
	cout << "The best path is:" << endl;
	for (i = 0;i <G.n ;i++) {
		cout << Euler[i] << "-->";
		sumweight += G.Edge[Euler[i] - 1][Euler[i + 1] - 1];
	}
	cout << Euler[i] << endl;
	cout << "The distance of the best path is: " << sumweight << endl;
}
//最小生成树
int main()
{
	Graph G;
	int i, j, v;
	G.n = 5;
	G.e = G.n * (G.n - 1) / 2;//完全图的边数
	int Edge[G.n][G.n]={
		{0, 5 ,12, 5, 5},
		{5 ,0, 9, 7, 8},
		{12 ,9 , 0, 9,16},
		{5, 7, 9, 0 ,8},
		{5, 8, 16, 8, 0}
	} ;
	memset(G.Edge,INFINITY,sizeof(G.Edge));
	for (i = 0;i < G.n;i++) 
		for(j = 0;j < G.n;j++)
			G.Edge[i][j] = Edge[i][j];
	cout << "初始顶点: 1 " << endl;
	v = 1;
	MST(G,v);
	return 0;
}
