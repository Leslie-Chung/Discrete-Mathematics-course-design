#include <iostream>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#define MAX_CITY_NUM 50//最大顶点数
#define T0 2000//初始温度
#define T 1e-5//温度的下限
#define ALPHA 0.70//衰减因子
#define L 2000//当温度等于t时需要的循环次数
using namespace std;
struct path {
    int* route;
    int dis;
};

int w[MAX_CITY_NUM][MAX_CITY_NUM];
int num;//顶点数 
int s = 0;//用来记录序列没有变化的次数 
path p0;

void Init_City()
{
    cout << "请输入顶点数:" << endl;
    cin >> num;
    cout << "请输入权值:" << endl;
    for (int i = 0; i < num; ++i)
        for (int j = 0; j < num; ++j)
            cin >> w[i][j];
  	cout << endl; 
}

void Init_path()
{
	p0.route = new int[num];
    p0.dis = 0;
    for (int i = 0; i < num; ++i)
    {
        p0.route[i] = i;
        if (i != num - 1)
            p0.dis += w[i][i + 1];
    }
    p0.dis += w[num - 1][0];
}

path generate(path p)
{
    int x = 0, y = 0;
    while (x == y)
    {
        x = (int)(num * (rand() / (RAND_MAX + 1.0)));
        y = (int)(num * (rand() / (RAND_MAX + 1.0)));
    }
    path gen = p;
    int tmp;
    tmp = gen.route[x];
    gen.route[x] = gen.route[y];
    gen.route[y] = tmp;

    gen.dis = 0;
    for (int i = 0; i < num - 1; ++i)
        gen.dis += w[gen.route[i]][gen.route[i + 1]];
    gen.dis += w[gen.route[num - 1]][gen.route[0]];
    return gen;
}

void TSP_SA()
{
    double t = T0;
    srand(time(NULL));
    path cur = p0;
    path next = p0;
    bool bChange;
    while (t > T)
    {
        bChange = false;//初始设为序列没有变化
        for (int i = 0; i < L; ++i)//当温度为t时，循环L次
        {
            next = generate(cur);////调整某两个城市的位置后形成的新序列
            double df = next.dis - cur.dis;//p=exp[-(Ej-Ei)/(k*T)]  为了计算概率
            if (df <= 0)//如果新序列-前一个序列的距离小于0
            {
                cur = next;
                bChange = true;
            }
            else//模拟退火算法以一定的概率来接受一个比当前解要差的解，因此有可能会跳出局部的最优解，达到全局的最优解
            {
                double rndp = rand() / (RAND_MAX + 1.0);//得到一个大于0、小于1的数
                double eps = exp(-df / t);
                if (eps > rndp&& eps < 1)//eps要在[0,1)区间内
                {
                    cur = next;
                    bChange = true;
                }
            }
        }

        if (cur.dis < p0.dis)///如果得到的新序列比前一个序列的总距离小 就更新序列
            p0 = cur;

        t *= ALPHA;//温度进行衰减
        if (!bChange)//是否产生了新的序列
            ++s;
        else
            s = 0;

        if (s == 2)//如果两次都没有变化
            break;
    }
}

int main()
{
    Init_City();//初始化城市
    Init_path();//初始化路径，该路径不是一个回路
    TSP_SA();//模拟退火算法

    cout << "The best path is:" << endl;
    for (int i = 0; i < num; ++i)
        cout << p0.route[i] + 1 << " --> ";
    cout << p0.route[0] + 1 << endl;
    cout << "The distance of the best path is: " << p0.dis << endl;
    return 0;
}
