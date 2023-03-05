#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string.h>
using namespace std;
#define   MAX_SIZE 50
#define   INFINITY 32767
#define   population  200 // 种群数量
#define   pc  0.9 // 交叉的概率
#define   pm  0.1 // 变异的概率
#define   count  200 // 迭代的次数
int num; // 城市的数量
int** city; // 存放population个个体的访问顺序，即population个解向量
int Edge[MAX_SIZE][MAX_SIZE];//权值
int* dis; // 存放每个个体的访问顺序下的路径长度
double* fitness; // 存放每个个体的适应度
int   min_dis = INFINITY;//哈密顿回路的权值
int   min_index = -1;
int* min_path;//最后的哈密顿回路
// 初始化种群
void  init()
{
    cout << "请输入顶点数：";
    cin >> num;
    cout << endl;
    memset(Edge, INFINITY, sizeof(Edge));
    cout << "请输入权值：" << endl;
    for (int i = 0;i < num;i++)
        for (int j = 0;j < num;j++)
            cin >> Edge[i][j];
    cout << endl;
    int* a = new   int[num];//用来生成不同的解向量
    for (int i = 0; i < num; i++)
    {
        a[i] = i;
    }
    city = new   int* [population];
    for (int i = 0; i < population; i++)
    {
        city[i] = new   int[num];
    }
    for (int i = 0; i < population; i++)
    {
        for (int j = num - 1; j >= 0; j--)
        {
            int  n = rand() % (j + 1); // 产出的数是 0-j, 保证交换的后面的数不会再被交换
            swap(a[j], a[n]); // 保证 a 里面全是 0-(num-1) 的数，无重复的数，只是顺序颠倒
            city[i][j] = a[j];
        }
    }
    delete[]a;
    dis = new   int[population];
    fitness = new   double[population];
    min_path = new   int[num];
}
// 计算适应度
void  compute()
{
    double  total = 0;
    for (int i = 0; i < population; i++)
    {
        // 计算每个个体的路径长度
        dis[i] = 0;
        int j;
        for (j = 0; j < num - 1; j++)
            dis[i] += Edge[city[i][j]][city[i][j + 1]];

        dis[i] += Edge[city[i][j]][city[i][0]];
        fitness[i] = 1.0 / dis[i]; // 以距离的倒数作为适应度函数值，距离越小适应度越大
        total += fitness[i];
    }
}

// 选择适应度高的物种，采用轮盘赌算法(在交叉时使用)
int  select()
{
    double  total = 0;
    for (int i = 0; i < population; i++)
    {
        total += fitness[i];//总的适应度
    }
    double  size = rand() / (double)RAND_MAX * total; // 保证不产生 0
    double  sum = 0;
    int  i = 0;
    while (sum <= size && i < population)
    {
        sum += fitness[++i];
    }
    return  --i; // 返回被选中的个体
}
int   getMinDis()
{
    int  result = dis[0];
    int  index = 0;
    for (int i = 1; i < population; i++)
    {
        if (result > dis[i])
        {
            result = dis[i];
            index = i;
        }
    }
    return  index;
}
int   getMaxDis()
{
    int  result = dis[0];
    int  index = 0;
    for (int i = 1; i < population; i++)
    {
        if (result < dis[i])
        {
            result = dis[i];
            index = i;
        }
    }
    return  index;
}
/* 最优保存算法
    最优保留策略指将群体中最优的一部分个体不经过选择, 交叉和变异操作, 直接进入下一代, 以避免优秀个体损失
    若当前群体中最优个体比历史最优个体适应度还高, 则以当前群体最优个体作为历史最优个体; 否则使用历史最优个体替换当前群体最差个体
*/
void  save()
{
    int   current_min_index = getMinDis();
    int   current_max_index = getMaxDis();
    if (dis[current_min_index] < min_dis)
    {
        min_dis = dis[current_min_index];//更新min_dis
        for (int i = 0; i < num; i++)
        {
            min_path[i] = city[current_min_index][i];//更新min路径
        }
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            city[current_max_index][i] = min_path[i];
        }
        dis[current_max_index] = min_dis;
        fitness[current_max_index] = 1.0 / min_dis;
    }
}

bool  isExist(int   value, int* array, int   len)
{
    for (int i = 0; i < len; i++)
    {
        if (value == array[i])
            return   true;
    }
    return   false;
}
void  convert(int   p1, int   p2, int* src, int* dst)//dst初始为父代2，在最后一步才会变成子代1，所以它既充当父代也充当子代 
{   
    int   len = p2 - p1 + 1;//要交换部分的长度
    int* temp = new  int[len];
    for (int i = p1; i <= p2; i++)
    {
        temp[i - p1] = src[i];//父代要交换的部分
    }
    int  j = (p2 + 1) % num;//要交换的其余部分的初始位置
    for (int i = 1; i <= num; i++)
    {
    	//index为查找的位置，j为插入的位置，初始他们在同一个位置，否则可能会导致解向量中的城市重复出现（如 0 1 2 2 2） 
        int  index = (i + p2) % num;
        if (!isExist(dst[index], temp, len))//将父代2中未被选择到的基因按顺序复制到子代1中（在同一个数组dst里面进行操作） 
        {
            dst[j] = dst[index];
            j = (j + 1) % num;//j往后移一位
        }
    }
    for (int i = p1; i <= p2; i++)//将交换部分放至子代的相应位置
    {
        dst[i] = src[i];
    }
    delete[]temp;
}
// 交叉，采用顺序交叉算法
void  cross()
{
    for (int k = 0; k < population; k += 2)//两两交配，得到子代
    {
        int  a = select();//选择出适应度高的
        int  b = select();
        while (a == b)
            b = select(); // 保证被选中的个体不是一样的

        double  p = rand() / double(RAND_MAX);
        int* a1 = new   int[num];//父代1
        int* a2 = new   int[num];//父代2
        int* b1 = new   int[num];//子代1，先存储父代2的信息，要用于交叉算法，convert之后直接成为子代1
        int* b2 = new   int[num];//子代2，先存储父代1的信息
        for (int i = 0; i < num; i++)
        {
            a1[i] = city[a][i];
            a2[i] = city[b][i];
            b1[i] = a2[i];
            b2[i] = a1[i];
        }
        if (p < pc) // 满足交叉条件
        {
            // 选择交叉的两点，并保证 p1<p2
            int  p1 = -1;
            int  p2 = -1;
            while (p1 == p2)
            {
                p1 = rand() % num;
                p2 = rand() % num;
                if (p1 > p2)
                {
                    swap(p1, p2);
                }
            }
            // 开始交叉
            convert(p1, p2, a1, b1);
            convert(p1, p2, a2, b2);
            for (int i = 0; i < num; i++)//将父代更新为子代
            {
                city[k][i] = b1[i];
                city[k + 1][i] = b2[i];
            }
        }
        //else//这一步不太懂为什么，所以没有加进去
        //{
        //    for (int i = 0; i < num; i++)
        //    {
        //        city[k][i] = a1[i];
        //        city[k + 1][i] = a2[i];
        //    }
        //}
        delete[]a1;
        delete[]a2;
        delete[]b1;
        delete[]b2;
    }
}
// 变异，采用对换操作进行变异
void   morphis()
{
    for (int i = 0; i < population; i++)
    {
        double  p = rand() / double(RAND_MAX);
        if (p < pm) // 执行变异
        {
            int  a = -1, b = -1;
            while (a == b)
            {
                a = rand() % num;
                b = rand() % num;
            }
            swap(city[i][a], city[i][b]);//在个体基因序列中选择两个位置的基因进行交换
        }
    }
}
// 释放申请的数组的空间
void  dispose()
{
    for (int i = 0; i < population; i++)
    {
        delete[]city[i];
    }
    delete[]city;
    delete[]dis;
    delete[]fitness;
}
int  main()
{
    init(); // 初始化种群
    int   i = 0;
    srand(time(NULL));
    compute();
    while (i < count)
    {
        cross(); // 交叉
        morphis(); // 变异
        compute(); // 计算适应度
        save(); // 保存当前最优的个体
        i++;
    }
    compute();
    cout << "min distance is: " << min_dis << endl;
    for (int i = 0; i < num; i++)
        cout << min_path[i]+1 << "-->";
    cout << min_path[0] + 1;
    cout << endl;
    dispose(); // 释放空间
    return  0;
}

