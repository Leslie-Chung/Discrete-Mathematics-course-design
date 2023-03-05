#include<time.h>
#include<iostream>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#define MAX_NUM 50
using namespace std;
int N;//顶点个数
const int M = 200;//蚂蚁个数
int mp[MAX_NUM ][MAX_NUM ];//权值矩阵
double phe[MAX_NUM ][MAX_NUM ];//路径上存留的信息素
double phe2[MAX_NUM ][MAX_NUM ];//新留下的信息素
double rate = 0.5;//信息素挥发的速率
double Q = 10;//信息素残留参数
int path[M][MAX_NUM ] = { 0 };//每个蚂蚁走过的路径
int alpha = 1;// 表示信息素重要程度的参数
int beta = 4;// 表示启发式因子(可见度)重要程度的参数
int times = 500;//最大迭代次数
bool vis[MAX_NUM ];//是否访问过

void init_map() {//初始化边的情况
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < N; ++j) 
            cin >> mp[i][j];
}
int cal_distance(int a[]) {//计算路径长度
    int tot = 0;
    for (int i = 0; i < N - 1; ++i) {
        tot += mp[a[i]][a[i + 1]];
    }
    tot += mp[a[0]][a[N - 1]];
    return tot;
}
int main() {
    srand(time(NULL));
    cout << "请输入顶点数：";
    cin >> N;
    cout << endl;
    cout << "请输入各顶点间的权值：" << endl;
    init_map();
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            phe[i][j] = 1.0;
    for (int T = 0; T < times; ++T) {
        for (int i = 0; i < M; ++i) {//遍历每个蚂蚁
            memset(vis, false, sizeof(vis));//将visit全部置为false
            int cur = rand() % N; //为每个蚂蚁随机选择出发城市
            vis[cur] = true;
            path[i][0] = cur;//第i个蚂蚁走过的第一个城市
            for (int j = 1; j < N; ++j) {//推算第i个蚂蚁走过的第j个城市,当某顶点没有被访问时，走该点的概率为Pj=某可选点的能见度和信息素幂乘积/所有可选点的能见度和信息素幂乘积，若该顶点被访问过，则概率为0
                double pob[MAX_NUM] = { 0 }, POB = 0;//前者保存各个城市被选中的概率，后者计算当前城市和没去过的城市之间的信息素总和
                for (int k = 0; k < N; ++k) {
                    if (!vis[k]) {//如果没有访问过
                        pob[k] = pow(phe[cur][k], alpha) * pow(1.0 / mp[cur][k], beta);//计算信息素和可选点的能见度幂乘积
                        //  1.0 / mp[cur][k]为能见度，是两点i,j路距离的倒数
                       // phe[cur][k]为时间T时由i到j的信息素强度
                        POB += pob[k];
                    }
                }
                //轮盘赌选择
                //假设轮盘上是扇形区域是每个城市的信息素，它们的概率分别为0.4、0.3等等
                if (POB > 0) {//总的信息素值大于0
                    double zhuan = rand() * 1.0 / RAND_MAX * POB;//产生一个随机数,并运用存储的信息计算出下一步可达节点的概率，即转动的角度
                    for (int k = 0; k < N; ++k) {
                        if (!vis[k]) {
                            zhuan -= pob[k];//转动一次经过了多少角度，并减去  信息素越浓的点,被选中的概率越大
                            if (zhuan <= 0 || k == N - 1) {
                                cur = k;
                                break;
                            }
                        }
                    }
                }
                else {//如果城市间的信息素非常小 ( 小到比double能够表示的最小的数字还要小 )，出现这种情况，就把第一个没去过的城市作为答案
                    for (int ans = 0;ans < N;ans++)
                        if (!vis[ans]) {
                            cur = ans;
                            break;
                        }
                }
                vis[cur] = true;
                path[i][j] = cur;//第i个蚂蚁走过的第j个城市为cur
            }
        }
        memset(phe2, 0, sizeof(phe2));
        //更新信息素
        for (int i = 0; i < M; ++i) {//第i个蚂蚁在它所经过的路径上能留下多少信息素
            int tot = 0;
            tot += cal_distance(path[i]);
            for (int j = 0; j < N - 1; ++j) {
                phe2[path[i][j]][path[i][j + 1]] += Q / tot;//路径越短，新留下的信息素越多
                phe2[path[i][j + 1]][path[i][j]] = phe2[path[i][j]][path[i][j + 1]];
            }
            phe2[path[i][0]][path[i][N - 1]] += Q / tot;
            phe2[path[i][N - 1]][path[i][0]] = phe2[path[i][0]][path[i][N - 1]];
        }
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {//最新的信息素 = 之前留存的信息素 + 这次行走所留下的信息素
                phe[i][j] = phe[i][j] * rate + phe2[i][j];
            }
        }
    }
    int imin = 32767;
    int id = 0;
    for (int i = 0; i < M; ++i) {
        int tmp = cal_distance(path[i]);//计算第i个蚂蚁走过的路径长度
        if (tmp < imin) imin = tmp, id = i;
    }
    cout << "The distance of the best path is: " << imin << endl;
    cout << "The best path is:" << endl;
    for (int i = 0; i < N; ++i) cout << path[id][i] + 1 << "-->";
    cout << path[id][0] + 1;
    return 0;
}
