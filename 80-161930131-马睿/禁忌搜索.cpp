#include<iostream>
#include<string>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<string.h> 
#include<limits.h>
using namespace std;

const int MAX_GEN = 400;//��������
const int NeighborNum=200;//�ھ���Ŀ
const int TabuLen = 8;//���ɳ���

int CityNum;//��������
int** Distance;//�������

int* Ghh;//��ʼ·��
int* bestGh;//���·��
int bestEvaluation;//���·������
int* LocalGhh;//�������·��
int localEvaluation;//�������·������
int* tempGhh;//��ʱ·��
int tempEvaluation;//��ʱ·������

int** TabuList;//���ɱ�
int t;//��ǰ����

// ��ʼ�����ֲ���
void init() {
	CityNum = 5;
	// ����ռ�
	Distance = new int* [CityNum];
	for (int i = 0; i < CityNum; i++) {
		Distance[i] = new int[CityNum];
	}
	int distance[CityNum][CityNum]={
		{0, 5 ,12, 5, 5},
		{5 ,0, 9, 7, 8},
		{12 ,9 , 0, 9,16},
		{5, 7, 9, 0 ,8},
		{5, 8, 16, 8, 0}
	};
	for (int i = 0;i < CityNum;i++) 
		for(int j = 0;j < CityNum;j++)
			Distance[i][j] = distance[i][j];
	// ����ռ� ���·�������
	Ghh = new int[CityNum];
	cout <<"��ʼͨ·Ϊ��";
	for (int i = 0; i < CityNum; i++){
		Ghh[i] = i;//��ʼͨ·
		cout << i + 1 <<"-->"; 
	} 
	cout <<endl;
	bestGh = new int[CityNum];
	bestEvaluation = INT_MAX;
	LocalGhh = new int[CityNum];
	localEvaluation = INT_MAX;
	tempGhh = new int[CityNum];
	tempEvaluation = INT_MAX;
	// ����ռ� ����������ʼ��0
	TabuList = new int* [TabuLen];
	for (int i = 0; i < TabuLen; i++) {
		TabuList[i] = new int[CityNum];
		memset(TabuList[i], -1, sizeof(int) * CityNum);//��ʼ�����ɱ�
	}
	
	t = 0;
	// �������������
	srand((unsigned int)time(0));
}
int evaluate(int* arr) {//����·������
	int len = 0;
	for (int i = 1; i < CityNum; i++) {
		len += Distance[arr[i - 1]][arr[i]];
	}
	len += Distance[arr[CityNum - 1]][arr[0]];
	return len;
}
// �õ���ǰ����Ghh���������tempGhh
void changeneighbor(int* Gh, int* tempGh) {
	int ran1;
	while ((ran1 = rand() % CityNum) == 0);
	int ran2 = rand() % CityNum;
	while (ran1 == ran2 || ran2 == 0) {
		ran2 = rand() % CityNum;
	}

	int ran3 = rand() % 3;
	// �������һ����
	if (ran3 == 0) {
		memcpy(tempGh, Gh, sizeof(int) * CityNum);
		swap(tempGh[ran1], tempGh[ran2]);
	}
	// ��������м�һ�ξ���
	else if (ran3 == 1) {
		if (ran1 > ran2) {
			swap(ran1, ran2);
		}
		int len = ran1 + ran2;
		for (int i = 0; i < CityNum; i++) {
			if (i >= ran1 && i <= ran2) {
				tempGh[i] = Gh[len - i];
			}
			else {
				tempGh[i] = Gh[i];
			}
		}
	}
	// ���м�һ�ξ���������
	else if (ran3 == 2) {
		if (ran1 > ran2) {
			swap(ran1, ran2);
		}
		int index = 0;
		for (int i = 0; i < ran1; i++) {
			tempGh[index++] = Gh[i];
		}
		for (int i = ran2 + 1; i < CityNum; i++) {
			tempGh[index++] = Gh[i];
		}
		for (int i = ran1; i <= ran2; i++) {
			tempGh[index++] = Gh[i];
		}
	}
}
// �ж������Ƿ��ڽ��ɱ���
bool in_TabuList(int* tempGh) {
	int i;
	bool flag;
	for (i = 0; i < TabuLen; i++) {
		flag = false;
		for (int j = 0; j < CityNum; j++) {
			if (tempGh[j] != TabuList[i][j]) {//����ͽ��ɱ�����һ��·����ͬ����Ѱ����һ��
				flag = true;
				break;
			}
		}
		if (flag == false)//˵���ҵ���
			break;
	}
	return !(i == TabuLen);//�����棬˵���ҵ���
}
// ������ɱ�
void pushTabuList(int* arr) {
	// ɾ�����е�һ��·��
	for (int i = 0; i < TabuLen - 1; i++) {
		for (int j = 0; j < CityNum; j++) {
			TabuList[i][j] = TabuList[i + 1][j];
		}
	}
	// �������β��
	for (int k = 0; k < CityNum; k++) {
		TabuList[TabuLen - 1][k] = arr[k];
	}
}
// ���TSP����
/*���Խ��ɱ�H= ��ѡ��һ����ʼ��x�������������ʱ��ֹͣ���㣬������
������x������N(x)��ѡ�����㲻�ܽ��ɵĺ�ѡ��Can_N(x)����Can_N(x)��ѡһ������ֵ��ѵĽ�x1��x=x1��
������ʷ��¼H���ظ�STEP2��
*/
void solve() {

	// ���赱ǰ·��Ϊ����
	memcpy(bestGh, Ghh, sizeof(int) * CityNum);
	bestEvaluation = evaluate(Ghh);

	// ���޴�������
	int nn;
	while (t < MAX_GEN) {
		nn = 0;
		localEvaluation = INT_MAX;// ��ʼ�������
		while (nn < NeighborNum) {
			changeneighbor(Ghh, tempGhh);// �õ���ǰ·��Ghh������·��tempGhh
			if (!in_TabuList(tempGhh)) {// ���ɱ��в�����
				tempEvaluation = evaluate(tempGhh);
				if (tempEvaluation < localEvaluation) {// �ֲ�����
					memcpy(LocalGhh, tempGhh, sizeof(int) * CityNum);
					localEvaluation = tempEvaluation;
				}
				nn++;
			}
		}
		if (localEvaluation < bestEvaluation) {// ���Ÿ���
			memcpy(bestGh, LocalGhh, sizeof(int) * CityNum);
			bestEvaluation = localEvaluation;
		}
		memcpy(Ghh, LocalGhh, sizeof(int) * CityNum);// ���ܸ����ͬ������

		pushTabuList(LocalGhh);// ������ɱ�
		t++;
	}
}
int main()
{
	init();
	solve();
	cout << "���·�����ȣ� " << bestEvaluation << endl;
	cout << "���·����" << endl;
	for (int i = 0; i < CityNum; i++)
		cout << bestGh[i] + 1<< "->";
	cout << bestGh[0] + 1<< endl;
	
	return 0;
}
