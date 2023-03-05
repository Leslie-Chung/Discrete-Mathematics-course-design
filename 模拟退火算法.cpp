#include <iostream>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#define MAX_CITY_NUM 50//��󶥵���
#define T0 2000//��ʼ�¶�
#define T 1e-5//�¶ȵ�����
#define ALPHA 0.70//˥������
#define L 2000//���¶ȵ���tʱ��Ҫ��ѭ������
using namespace std;
struct path {
    int* route;
    int dis;
};

int w[MAX_CITY_NUM][MAX_CITY_NUM];
int num;//������ 
int s = 0;//������¼����û�б仯�Ĵ��� 
path p0;

void Init_City()
{
    cout << "�����붥����:" << endl;
    cin >> num;
    cout << "������Ȩֵ:" << endl;
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
        bChange = false;//��ʼ��Ϊ����û�б仯
        for (int i = 0; i < L; ++i)//���¶�Ϊtʱ��ѭ��L��
        {
            next = generate(cur);////����ĳ�������е�λ�ú��γɵ�������
            double df = next.dis - cur.dis;//p=exp[-(Ej-Ei)/(k*T)]  Ϊ�˼������
            if (df <= 0)//���������-ǰһ�����еľ���С��0
            {
                cur = next;
                bChange = true;
            }
            else//ģ���˻��㷨��һ���ĸ���������һ���ȵ�ǰ��Ҫ��Ľ⣬����п��ܻ������ֲ������Ž⣬�ﵽȫ�ֵ����Ž�
            {
                double rndp = rand() / (RAND_MAX + 1.0);//�õ�һ������0��С��1����
                double eps = exp(-df / t);
                if (eps > rndp&& eps < 1)//epsҪ��[0,1)������
                {
                    cur = next;
                    bChange = true;
                }
            }
        }

        if (cur.dis < p0.dis)///����õ��������б�ǰһ�����е��ܾ���С �͸�������
            p0 = cur;

        t *= ALPHA;//�¶Ƚ���˥��
        if (!bChange)//�Ƿ�������µ�����
            ++s;
        else
            s = 0;

        if (s == 2)//������ζ�û�б仯
            break;
    }
}

int main()
{
    Init_City();//��ʼ������
    Init_path();//��ʼ��·������·������һ����·
    TSP_SA();//ģ���˻��㷨

    cout << "The best path is:" << endl;
    for (int i = 0; i < num; ++i)
        cout << p0.route[i] + 1 << " --> ";
    cout << p0.route[0] + 1 << endl;
    cout << "The distance of the best path is: " << p0.dis << endl;
    return 0;
}
