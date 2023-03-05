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
	cout << "��������ȫͼ�Ķ�����:";
	cin >> G.n ;
	for (i = 0;i < G.n;i++) visited[i] = false;
	G.e = G.n * (G.n - 1) / 2;//��ȫͼ�ı���
	for (i = 0;i < G.n;i++)
		for (j = 0;j < G.n;j++)
			G.Edge[i][j] = INFINITY;//��ʼ��Ȩֵ
	cout << endl;
	cout << "������ʼ����յ㼰��Ȩֵ(Ȩֵ���ǡ޵ıߣ������ظ����룬��1 2 30��2 1 30)" << endl;
	for (i = 0;i < G.e;i++) {
		cin >> v >> j;
		cin >> G.Edge[v-1][j-1];
		G.Edge[j - 1][v - 1]= G.Edge[v - 1][j - 1];
	}
	cout << "�������ʼ����:";
	cin >> v;
	cout << endl;
	NearestNeighbor(G,v);
	return 0;
}
