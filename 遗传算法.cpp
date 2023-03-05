#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string.h>
using namespace std;
#define   MAX_SIZE 50
#define   INFINITY 32767
#define   population  200 // ��Ⱥ����
#define   pc  0.9 // ����ĸ���
#define   pm  0.1 // ����ĸ���
#define   count  200 // �����Ĵ���
int num; // ���е�����
int** city; // ���population������ķ���˳�򣬼�population��������
int Edge[MAX_SIZE][MAX_SIZE];//Ȩֵ
int* dis; // ���ÿ������ķ���˳���µ�·������
double* fitness; // ���ÿ���������Ӧ��
int   min_dis = INFINITY;//���ܶٻ�·��Ȩֵ
int   min_index = -1;
int* min_path;//���Ĺ��ܶٻ�·
// ��ʼ����Ⱥ
void  init()
{
    cout << "�����붥������";
    cin >> num;
    cout << endl;
    memset(Edge, INFINITY, sizeof(Edge));
    cout << "������Ȩֵ��" << endl;
    for (int i = 0;i < num;i++)
        for (int j = 0;j < num;j++)
            cin >> Edge[i][j];
    cout << endl;
    int* a = new   int[num];//�������ɲ�ͬ�Ľ�����
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
            int  n = rand() % (j + 1); // ���������� 0-j, ��֤�����ĺ�����������ٱ�����
            swap(a[j], a[n]); // ��֤ a ����ȫ�� 0-(num-1) ���������ظ�������ֻ��˳��ߵ�
            city[i][j] = a[j];
        }
    }
    delete[]a;
    dis = new   int[population];
    fitness = new   double[population];
    min_path = new   int[num];
}
// ������Ӧ��
void  compute()
{
    double  total = 0;
    for (int i = 0; i < population; i++)
    {
        // ����ÿ�������·������
        dis[i] = 0;
        int j;
        for (j = 0; j < num - 1; j++)
            dis[i] += Edge[city[i][j]][city[i][j + 1]];

        dis[i] += Edge[city[i][j]][city[i][0]];
        fitness[i] = 1.0 / dis[i]; // �Ծ���ĵ�����Ϊ��Ӧ�Ⱥ���ֵ������ԽС��Ӧ��Խ��
        total += fitness[i];
    }
}

// ѡ����Ӧ�ȸߵ����֣��������̶��㷨(�ڽ���ʱʹ��)
int  select()
{
    double  total = 0;
    for (int i = 0; i < population; i++)
    {
        total += fitness[i];//�ܵ���Ӧ��
    }
    double  size = rand() / (double)RAND_MAX * total; // ��֤������ 0
    double  sum = 0;
    int  i = 0;
    while (sum <= size && i < population)
    {
        sum += fitness[++i];
    }
    return  --i; // ���ر�ѡ�еĸ���
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
/* ���ű����㷨
    ���ű�������ָ��Ⱥ�������ŵ�һ���ָ��岻����ѡ��, ����ͱ������, ֱ�ӽ�����һ��, �Ա������������ʧ
    ����ǰȺ�������Ÿ������ʷ���Ÿ�����Ӧ�Ȼ���, ���Ե�ǰȺ�����Ÿ�����Ϊ��ʷ���Ÿ���; ����ʹ����ʷ���Ÿ����滻��ǰȺ��������
*/
void  save()
{
    int   current_min_index = getMinDis();
    int   current_max_index = getMaxDis();
    if (dis[current_min_index] < min_dis)
    {
        min_dis = dis[current_min_index];//����min_dis
        for (int i = 0; i < num; i++)
        {
            min_path[i] = city[current_min_index][i];//����min·��
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
void  convert(int   p1, int   p2, int* src, int* dst)//dst��ʼΪ����2�������һ���Ż����Ӵ�1���������ȳ䵱����Ҳ�䵱�Ӵ� 
{   
    int   len = p2 - p1 + 1;//Ҫ�������ֵĳ���
    int* temp = new  int[len];
    for (int i = p1; i <= p2; i++)
    {
        temp[i - p1] = src[i];//����Ҫ�����Ĳ���
    }
    int  j = (p2 + 1) % num;//Ҫ���������ಿ�ֵĳ�ʼλ��
    for (int i = 1; i <= num; i++)
    {
    	//indexΪ���ҵ�λ�ã�jΪ�����λ�ã���ʼ������ͬһ��λ�ã�������ܻᵼ�½������еĳ����ظ����֣��� 0 1 2 2 2�� 
        int  index = (i + p2) % num;
        if (!isExist(dst[index], temp, len))//������2��δ��ѡ�񵽵Ļ���˳���Ƶ��Ӵ�1�У���ͬһ������dst������в����� 
        {
            dst[j] = dst[index];
            j = (j + 1) % num;//j������һλ
        }
    }
    for (int i = p1; i <= p2; i++)//���������ַ����Ӵ�����Ӧλ��
    {
        dst[i] = src[i];
    }
    delete[]temp;
}
// ���棬����˳�򽻲��㷨
void  cross()
{
    for (int k = 0; k < population; k += 2)//�������䣬�õ��Ӵ�
    {
        int  a = select();//ѡ�����Ӧ�ȸߵ�
        int  b = select();
        while (a == b)
            b = select(); // ��֤��ѡ�еĸ��岻��һ����

        double  p = rand() / double(RAND_MAX);
        int* a1 = new   int[num];//����1
        int* a2 = new   int[num];//����2
        int* b1 = new   int[num];//�Ӵ�1���ȴ洢����2����Ϣ��Ҫ���ڽ����㷨��convert֮��ֱ�ӳ�Ϊ�Ӵ�1
        int* b2 = new   int[num];//�Ӵ�2���ȴ洢����1����Ϣ
        for (int i = 0; i < num; i++)
        {
            a1[i] = city[a][i];
            a2[i] = city[b][i];
            b1[i] = a2[i];
            b2[i] = a1[i];
        }
        if (p < pc) // ���㽻������
        {
            // ѡ�񽻲�����㣬����֤ p1<p2
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
            // ��ʼ����
            convert(p1, p2, a1, b1);
            convert(p1, p2, a2, b2);
            for (int i = 0; i < num; i++)//����������Ϊ�Ӵ�
            {
                city[k][i] = b1[i];
                city[k + 1][i] = b2[i];
            }
        }
        //else//��һ����̫��Ϊʲô������û�мӽ�ȥ
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
// ���죬���öԻ��������б���
void   morphis()
{
    for (int i = 0; i < population; i++)
    {
        double  p = rand() / double(RAND_MAX);
        if (p < pm) // ִ�б���
        {
            int  a = -1, b = -1;
            while (a == b)
            {
                a = rand() % num;
                b = rand() % num;
            }
            swap(city[i][a], city[i][b]);//�ڸ������������ѡ������λ�õĻ�����н���
        }
    }
}
// �ͷ����������Ŀռ�
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
    init(); // ��ʼ����Ⱥ
    int   i = 0;
    srand(time(NULL));
    compute();
    while (i < count)
    {
        cross(); // ����
        morphis(); // ����
        compute(); // ������Ӧ��
        save(); // ���浱ǰ���ŵĸ���
        i++;
    }
    compute();
    cout << "min distance is: " << min_dis << endl;
    for (int i = 0; i < num; i++)
        cout << min_path[i]+1 << "-->";
    cout << min_path[0] + 1;
    cout << endl;
    dispose(); // �ͷſռ�
    return  0;
}

