#include<iostream>
#define MAX_SIZE 100
#define INFINITY 32767//��ʾ����
using namespace std;
bool visited[MAX_SIZE];
struct Graph {
	int Edge[MAX_SIZE][MAX_SIZE];//�洢Ȩֵ
	int n, e;//�������ͱ���
};
int FindMinDist(Graph& G,int v) {
	int minv , min = INFINITY;
	for (int i = 0;i < G.n;i++) {
		if (!visited[i] && G.Edge[v][i] < min) {//���iû�б����ʹ�
			min = G.Edge[v][i];
			minv = i;
		}
	}
	return minv;
}
void NearestNeighbor(Graph& G,int start) {//��start���㿪ʼ�㷨
	int pre, v, vcount = 0, sumweight = 0, * path = new int[G.n+1];
	path[vcount++] = start;
	pre = start-1;
	visited[start - 1] = true;
	while (vcount < G.n) {
		v = FindMinDist(G, pre);
		sumweight += G.Edge[pre][v];
		G.Edge[pre][v] = G.Edge[v][pre] = -1;//ɾ���ñ� 
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
//���ڽ���
int main()
{
	Graph G;
	int i, j, v;
	G.n = 5;
	for (i = 0;i < G.n;i++) visited[i] = false;
	G.e = G.n * (G.n - 1) / 2;//��ȫͼ�ı���
	int Edge[G.n][G.n]={
		{0, 5 ,12, 5, 5},
		{5 ,0, 9, 7, 8},
		{12 ,9 , 0, 9,16},
		{5, 7, 9, 0 ,8},
		{5, 8, 16, 8, 0}
	};
	for (int i = 0;i < G.n;i++) 
		for(int j = 0;j < G.n;j++)
			G.Edge[i][j] = Edge[i][j];
	cout << "��ʼ����: 1";
	v = 1;
	cout << endl;
	NearestNeighbor(G,v);
	return 0;
}
