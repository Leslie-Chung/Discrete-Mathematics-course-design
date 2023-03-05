#include<iostream>
#include<string.h> 
#define MAX_SIZE 50
#define INFINITY 32767
using namespace std;
int n;
int graph[MAX_SIZE][MAX_SIZE];
bool* vis;
int* path, * anspath;//ǰ�������洢��ǰ��·������������������丳ֵ��anspath
int ans;//��С��
int preans;//���������С���Ƿ��б仯�����б仯,��˵��·��Ҫ����
int start;//���
void dfs(int x, int temp, int cnt)
{
    int i;
    vis[x] = true;
    path[cnt] = x + 1;
    if (cnt == n - 1)//�ѷ��ʵĶ���������Ϊcnt��ʼֵΪ0��
    {
        preans = ans;
        temp += graph[x][start - 1];
        ans = ans < temp ? ans : temp;//ȡ��С��
        if (preans != ans)  memcpy(anspath, path, sizeof(int)*n);
        return;
    }
    for (i = 0; i < n; i++)
    {
        if (graph[x][i] != INFINITY && !vis[i])
        {
            temp += graph[x][i];
            if (temp < ans)
                dfs(i, temp, cnt + 1);
            temp -= graph[x][i];
            vis[i] = false;
        }
    }
}

int main()
{
    preans = ans = INFINITY;
    cout << "�����붥������";
    cin >> n;
    int temp = 0;
    vis = new bool[n];
    path = new int[n];
    anspath = new int[n];
    memset(graph, INFINITY, sizeof(graph));
    memset(vis, false, sizeof(bool)*n);
    cout << "������Ȩֵ��" << endl;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> graph[i][j];
    cout << endl;
    cout << "��������㣺";
    cin >> start;
    dfs(start - 1, temp, 0);
    cout << "The best path is:" << endl;
    for (int i = 0;i < n;i++) {
        cout << anspath[i] << "-->";
    }
    cout << anspath[0] << endl;
    cout << "The distance of the best path is: " << ans;
    return 0;
}
