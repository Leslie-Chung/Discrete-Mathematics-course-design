#include<time.h>
#include<iostream>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#define MAX_NUM 50
using namespace std;
int N;//�������
const int M = 200;//���ϸ���
int mp[MAX_NUM ][MAX_NUM ];//Ȩֵ����
double phe[MAX_NUM ][MAX_NUM ];//·���ϴ�������Ϣ��
double phe2[MAX_NUM ][MAX_NUM ];//�����µ���Ϣ��
double rate = 0.5;//��Ϣ�ػӷ�������
double Q = 10;//��Ϣ�ز�������
int path[M][MAX_NUM ] = { 0 };//ÿ�������߹���·��
int alpha = 1;// ��ʾ��Ϣ����Ҫ�̶ȵĲ���
int beta = 4;// ��ʾ����ʽ����(�ɼ���)��Ҫ�̶ȵĲ���
int times = 500;//����������
bool vis[MAX_NUM ];//�Ƿ���ʹ�

void init_map() {//��ʼ���ߵ����
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < N; ++j) 
            cin >> mp[i][j];
}
int cal_distance(int a[]) {//����·������
    int tot = 0;
    for (int i = 0; i < N - 1; ++i) {
        tot += mp[a[i]][a[i + 1]];
    }
    tot += mp[a[0]][a[N - 1]];
    return tot;
}
int main() {
    srand(time(NULL));
    cout << "�����붥������";
    cin >> N;
    cout << endl;
    cout << "�������������Ȩֵ��" << endl;
    init_map();
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            phe[i][j] = 1.0;
    for (int T = 0; T < times; ++T) {
        for (int i = 0; i < M; ++i) {//����ÿ������
            memset(vis, false, sizeof(vis));//��visitȫ����Ϊfalse
            int cur = rand() % N; //Ϊÿ���������ѡ���������
            vis[cur] = true;
            path[i][0] = cur;//��i�������߹��ĵ�һ������
            for (int j = 1; j < N; ++j) {//�����i�������߹��ĵ�j������,��ĳ����û�б�����ʱ���߸õ�ĸ���ΪPj=ĳ��ѡ����ܼ��Ⱥ���Ϣ���ݳ˻�/���п�ѡ����ܼ��Ⱥ���Ϣ���ݳ˻������ö��㱻���ʹ��������Ϊ0
                double pob[MAX_NUM] = { 0 }, POB = 0;//ǰ�߱���������б�ѡ�еĸ��ʣ����߼��㵱ǰ���к�ûȥ���ĳ���֮�����Ϣ���ܺ�
                for (int k = 0; k < N; ++k) {
                    if (!vis[k]) {//���û�з��ʹ�
                        pob[k] = pow(phe[cur][k], alpha) * pow(1.0 / mp[cur][k], beta);//������Ϣ�غͿ�ѡ����ܼ����ݳ˻�
                        //  1.0 / mp[cur][k]Ϊ�ܼ��ȣ�������i,j·����ĵ���
                       // phe[cur][k]Ϊʱ��Tʱ��i��j����Ϣ��ǿ��
                        POB += pob[k];
                    }
                }
                //���̶�ѡ��
                //����������������������ÿ�����е���Ϣ�أ����ǵĸ��ʷֱ�Ϊ0.4��0.3�ȵ�
                if (POB > 0) {//�ܵ���Ϣ��ֵ����0
                    double zhuan = rand() * 1.0 / RAND_MAX * POB;//����һ�������,�����ô洢����Ϣ�������һ���ɴ�ڵ�ĸ��ʣ���ת���ĽǶ�
                    for (int k = 0; k < N; ++k) {
                        if (!vis[k]) {
                            zhuan -= pob[k];//ת��һ�ξ����˶��ٽǶȣ�����ȥ  ��Ϣ��ԽŨ�ĵ�,��ѡ�еĸ���Խ��
                            if (zhuan <= 0 || k == N - 1) {
                                cur = k;
                                break;
                            }
                        }
                    }
                }
                else {//������м����Ϣ�طǳ�С ( С����double�ܹ���ʾ����С�����ֻ�ҪС )����������������Ͱѵ�һ��ûȥ���ĳ�����Ϊ��
                    for (int ans = 0;ans < N;ans++)
                        if (!vis[ans]) {
                            cur = ans;
                            break;
                        }
                }
                vis[cur] = true;
                path[i][j] = cur;//��i�������߹��ĵ�j������Ϊcur
            }
        }
        memset(phe2, 0, sizeof(phe2));
        //������Ϣ��
        for (int i = 0; i < M; ++i) {//��i������������������·���������¶�����Ϣ��
            int tot = 0;
            tot += cal_distance(path[i]);
            for (int j = 0; j < N - 1; ++j) {
                phe2[path[i][j]][path[i][j + 1]] += Q / tot;//·��Խ�̣������µ���Ϣ��Խ��
                phe2[path[i][j + 1]][path[i][j]] = phe2[path[i][j]][path[i][j + 1]];
            }
            phe2[path[i][0]][path[i][N - 1]] += Q / tot;
            phe2[path[i][N - 1]][path[i][0]] = phe2[path[i][0]][path[i][N - 1]];
        }
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {//���µ���Ϣ�� = ֮ǰ�������Ϣ�� + ������������µ���Ϣ��
                phe[i][j] = phe[i][j] * rate + phe2[i][j];
            }
        }
    }
    int imin = 32767;
    int id = 0;
    for (int i = 0; i < M; ++i) {
        int tmp = cal_distance(path[i]);//�����i�������߹���·������
        if (tmp < imin) imin = tmp, id = i;
    }
    cout << "The distance of the best path is: " << imin << endl;
    cout << "The best path is:" << endl;
    for (int i = 0; i < N; ++i) cout << path[id][i] + 1 << "-->";
    cout << path[id][0] + 1;
    return 0;
}
