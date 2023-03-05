#include<iostream>
#include<string.h> 
#define MAX_SIZE 50
#define INFINITY 32767
using namespace std;
int n;
int graph[MAX_SIZE][MAX_SIZE];
bool* vis;
int* path, * anspath;//前者用来存储当前的路径，如果符合条件则将其赋值给anspath
int ans;//最小界
int preans;//用来检测最小界是否有变化，若有变化,则说明路径要更新
int start;//起点
void dfs(int x, int temp, int cnt)
{
    int i;
    vis[x] = true;
    path[cnt] = x + 1;
    if (cnt == n - 1)//已访问的顶点数（因为cnt初始值为0）
    {
        preans = ans;
        temp += graph[x][start - 1];
        ans = ans < temp ? ans : temp;//取最小的
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
    cout << "请输入顶点数：";
    cin >> n;
    int temp = 0;
    vis = new bool[n];
    path = new int[n];
    anspath = new int[n];
    memset(graph, INFINITY, sizeof(graph));
    memset(vis, false, sizeof(bool)*n);
    cout << "请输入权值：" << endl;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> graph[i][j];
    cout << endl;
    cout << "请输入起点：";
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
