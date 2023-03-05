#include<iostream>
#include<string>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<string.h> 
using namespace std;

const int MAX_GEN = 400;//迭代次数
const int NeighborNum=200;//邻居数目
const int TabuLen = 8;//禁忌长度

int CityNum;//城市数量
int** Distance;//距离矩阵

int* Ghh;//初始路径
int* bestGh;//最好路径
int bestEvaluation;//最好路径长度
int* LocalGhh;//当代最好路径
int localEvaluation;//当代最好的路径长度
int* tempGhh;//临时路径
int tempEvaluation;//临时路径长度

int** TabuList;//禁忌表
int t;//当前代数

// 初始化各种参数
void init() {
	cout <<"请输入顶点数：";
	cin >> CityNum;
	// 申请空间
	Distance = new int* [CityNum];
	for (int i = 0; i < CityNum; i++) {
		Distance[i] = new int[CityNum];
	}
	cout <<"请输入权值："<< endl;
	for (int i = 0; i < CityNum; i++) {
		for (int j = 0; j < CityNum; j++) {
			cin >> Distance[i][j];
		}
	}
	cout <<endl;
	// 申请空间 最佳路径无穷大
	Ghh = new int[CityNum];
	cout <<"请输入一条初始通路(起点将作为哈密顿回路的起点)："<<endl;
	for (int i = 0; i < CityNum; i++) {
		int t;
		cin >> t;
		Ghh[i] = t - 1;
	}
	cout <<endl;
	bestGh = new int[CityNum];
	bestEvaluation = INT_MAX;
	LocalGhh = new int[CityNum];
	localEvaluation = INT_MAX;
	tempGhh = new int[CityNum];
	tempEvaluation = INT_MAX;
	// 申请空间 迭代次数初始化0
	TabuList = new int* [TabuLen];
	for (int i = 0; i < TabuLen; i++) {
		TabuList[i] = new int[CityNum];
		memset(TabuList[i], -1, sizeof(int) * CityNum);//初始化禁忌表
	}
	
	t = 0;
	// 设置随机数种子
	srand((unsigned int)time(0));
}
int evaluate(int* arr) {//计算路径长度
	int len = 0;
	for (int i = 1; i < CityNum; i++) {
		len += Distance[arr[i - 1]][arr[i]];
	}
	len += Distance[arr[CityNum - 1]][arr[0]];
	return len;
}
// 得到当前编码Ghh的邻域编码tempGhh
void changeneighbor(int* Gh, int* tempGh) {
	int ran1;
	while ((ran1 = rand() % CityNum) == 0);
	int ran2 = rand() % CityNum;
	while (ran1 == ran2 || ran2 == 0) {
		ran2 = rand() % CityNum;
	}

	int ran3 = rand() % 3;
	// 随机交换一个数
	if (ran3 == 0) {
		memcpy(tempGh, Gh, sizeof(int) * CityNum);
		swap(tempGh[ran1], tempGh[ran2]);
	}
	// 随机交换中间一段距离
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
	// 将中间一段距离放至最后
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
// 判读编码是否在禁忌表中
bool in_TabuList(int* tempGh) {
	int i;
	bool flag;
	for (i = 0; i < TabuLen; i++) {
		flag = false;
		for (int j = 0; j < CityNum; j++) {
			if (tempGh[j] != TabuList[i][j]) {//如果和禁忌表中有一条路径不同，则寻找下一个
				flag = true;
				break;
			}
		}
		if (flag == false)//说明找到了
			break;
	}
	return !(i == TabuLen);//返回真，说明找到了
}
// 加入禁忌表
void pushTabuList(int* arr) {
	// 删除队列第一条路径
	for (int i = 0; i < TabuLen - 1; i++) {
		for (int j = 0; j < CityNum; j++) {
			TabuList[i][j] = TabuList[i + 1][j];
		}
	}
	// 加入队列尾部
	for (int k = 0; k < CityNum; k++) {
		TabuList[TabuLen - 1][k] = arr[k];
	}
}
// 求解TSP问题
/*给以禁忌表H= 并选定一个初始解x；满足迭代次数时，停止计算，输出结果
否则，在x的邻域N(x)中选出满足不受禁忌的候选集Can_N(x)；在Can_N(x)中选一个评价值最佳的解x1，x=x1；
更新历史记录H，重复STEP2。
*/
void solve() {

	// 假设当前路径为最优
	memcpy(bestGh, Ghh, sizeof(int) * CityNum);
	bestEvaluation = evaluate(Ghh);

	// 有限次数迭代
	int nn;
	while (t < MAX_GEN) {
		nn = 0;
		localEvaluation = INT_MAX;// 初始化无穷大
		while (nn < NeighborNum) {
			changeneighbor(Ghh, tempGhh);// 得到当前路径Ghh的邻域路径tempGhh
			if (!in_TabuList(tempGhh)) {// 禁忌表中不存在
				tempEvaluation = evaluate(tempGhh);
				if (tempEvaluation < localEvaluation) {// 局部更新
					memcpy(LocalGhh, tempGhh, sizeof(int) * CityNum);
					localEvaluation = tempEvaluation;
				}
			}
			else if(tempEvaluation<bestEvaluation){//特赦
				memcpy(bestGh, LocalGhh, sizeof(int) * CityNum);
				bestEvaluation = localEvaluation;
			}
			nn++;
		}
		if (localEvaluation < bestEvaluation) {// 最优更新
			memcpy(bestGh, LocalGhh, sizeof(int) * CityNum);
			bestEvaluation = localEvaluation;
		}
		memcpy(Ghh, LocalGhh, sizeof(int) * CityNum);// 可能更差，但同样更新

		pushTabuList(LocalGhh);// 加入禁忌表
		t++;
	}
}
int main()
{
	init();
	clock_t start, finish;
	start = clock();
	solve();
	finish = clock();
	double run_time = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "最佳路径长度： " << bestEvaluation << endl;
	cout << "最佳路径：" << endl;
	for (int i = 0; i < CityNum; i++)
		cout << bestGh[i] + 1<< "->";
	cout << bestGh[0] + 1<< endl;
	
	cout << "Runtime: " << run_time << " seconds" << endl;
	return 0;
}
