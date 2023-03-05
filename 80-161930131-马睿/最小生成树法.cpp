#include<iostream>
#include<string.h>
#define MAX_SIZE 50
#define INFINITY 32767//��ʾ����
using namespace std;
int top = -1;
struct Graph {
	int Edge[MAX_SIZE][MAX_SIZE];//�洢Ȩֵ
	int n, e;//�������ͱ���
};
struct Closest {
	int parentvex;//ǰ������
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
/*��Ϊ�ض�����ŷ����·������һ���Ǳ߲��ص�Ȧ�Ĳ�����˴�һ���������ֱ���ߣ������ǲ����ţ���ֱ��ĳ������û�б߿����ˣ������ǵ�һ�����ֵģ� 
˵���ص�����㣨Ҳ���յ㣩���򽫸ö��㼰��֮ǰ�ı������Ķ��㣨�Ҹö����ޱ߿����ˣ�����ջ�У�һֱ�˻ص��б߿����ߵĶ��㣨�ö���ΪĳЩȦ�Ĺ������㣩���Ǿͼ����ߣ�ֱ�����������б�
���ͻ�õ�һ��ŷ����·*/
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
void MST(Graph& G,const int start) {//��start���㿪ʼ�㷨
	Closest *closest=Prim(G, start);
	int i, j;
	Graph Gtmp;
	for (i = 0;i < G.n;i++) {
		for (j = 0;j < G.n;j++) {
			Gtmp.Edge[i][j] = 0;//�������е㶼����ͨ
		}
	}
	Gtmp.n = G.n, Gtmp.e = G.n - 1;//����һ����ͼ�������ҹ��ܶٻ�·
	for (i = 0;i < G.n;i++) {
		if (closest[i].parentvex != -1) Gtmp.Edge[closest[i].parentvex][i] =  Gtmp.Edge[i][closest[i].parentvex] = 2;//���ƽ�б�
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
//��С������
int main()
{
	Graph G;
	int i, j, v;
	G.n = 5;
	G.e = G.n * (G.n - 1) / 2;//��ȫͼ�ı���
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
	cout << "��ʼ����: 1 " << endl;
	v = 1;
	MST(G,v);
	return 0;
}
