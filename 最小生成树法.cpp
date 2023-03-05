#include<iostream>
#define MAX_SIZE 50
#define INFINITY 32767//��ʾ����
using namespace std;
int top = -1;
struct Graph {
	int Edge[MAX_SIZE][MAX_SIZE];//�洢Ȩֵ
	int n, e;//�������ͱ���
};
struct Closest {
	int parentvex;//���ڵ�
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
	closest[start - 1].parentvex = closest[start - 1].lowcost = -1;//����ѱ���¼ 
	while (vcount < G.n) {
		minv = FindMinDist(closest, G);
		closest[minv].lowcost = -1;
		vcount++;
		for (i = 0;i < G.n;i++) {
			if (closest[i].lowcost != -1 && closest[i].lowcost > G.Edge[minv][i]) {
				closest[i].lowcost = G.Edge[minv][i];//����ÿ�������lowcost 
				closest[i].parentvex = minv;
			}
		}
	}
	return closest;
}
/*ŷ��ͨ·�����û�б߿��ߣ�˵��(��Ϊ�յ����ȶ��㣬���ֻ�ܴ�����ȥ�����ܳ�����ʼ���෴)
�ĸõ�ı߶������ˣ����
��һ������Ķ���ض���ŷ��ͨ·���յ㣬���õ�һ�����ŵ�ŷ����·(��������)

ŷ����·�����û�б߿��ߣ�˵��(�߲��ص�Ȧ�Ĳ�)
�ص�������Ķ���*/
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
	for ( v = 0;v < vcount;v++) {//ɾ���ظ��Ķ��㣬�������ӵ���� 
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
	cout << "���õĹ��ܶٻ�·�ǣ�";
	for (i = 0;i <G.n ;i++) {
		cout << Euler[i] << "-->";
		sumweight += G.Edge[Euler[i] - 1][Euler[i + 1] - 1];
	}
	cout << Euler[i] << endl;
	cout << "���õ�ȨֵΪ��" << sumweight << endl;
}
//��С������
int main()
{
	Graph G;
	int i, j, v;
	cout << "��������ȫͼ�Ķ�����:";
	cin >> G.n ;
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
	MST(G,v);
	return 0;
}
