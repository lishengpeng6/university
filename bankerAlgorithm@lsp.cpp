// Copyright © 2018 Li Shengpeng Banker Algorithm. All rights reserved. (完全原创)
#include <vector>
#include <iostream>
#include <algorithm>
#define RESOURCE_KIND 3 // 资源种类
#define PROCESS_NUM 5 // 进程个数
using namespace std;
class myVec{ // 自己定义的资源向量类
	public:
		myVec(int value,int len);
		myVec(int len);
		void dis();
		int & operator [] (int index);
		myVec operator + (myVec & x);
		myVec operator - (myVec & x);
		bool operator <= (myVec & x);
		vector <int> resource;
};
myVec::myVec(int value,int len):resource(RESOURCE_KIND,value){}
myVec::myVec(int len = RESOURCE_KIND):resource(RESOURCE_KIND){
	for(int i = 0;i < RESOURCE_KIND;i ++)
		cin >> (*this)[i];
}
void myVec::dis(){
	for(int i = 0;i < RESOURCE_KIND;i ++)
		cout << (*this)[i] << " ";
}
int & myVec::operator [] (int index){
	return this -> resource[index];
}
myVec myVec::operator + (myVec & x){
	myVec t = myVec(0,RESOURCE_KIND);
	for(int i = 0;i < RESOURCE_KIND;i ++)
		t[i] = (*this)[i] + x[i];
	return t;
}
myVec myVec::operator - (myVec & x){
	myVec t = myVec(0,RESOURCE_KIND);
	for(int i = 0;i < RESOURCE_KIND;i ++)
		t[i] = (*this)[i] - x[i];
	return t;
}
bool myVec::operator <= (myVec & x){
	for(int i = 0;i < RESOURCE_KIND;i ++)
		if((*this)[i] > x[i])
			return false;
	return true;
}
struct {
	myVec Avai = myVec();
	vector <myVec> Max = vector <myVec>(PROCESS_NUM);
	vector <myVec> Allo = vector <myVec>(PROCESS_NUM);
	vector <myVec> Need = vector <myVec>(PROCESS_NUM);
} bankMat; // Max Allo Need Avai 矩阵
void disbankMat(){
	cout << "      Max   Allocation   Need    Available" << endl << "     A B C     A B C     A B C     A B C" << endl;
	for(int i = 0;i < PROCESS_NUM;i ++){
		cout << "P" << i << "   ";
		bankMat.Max[i].dis();
		cout << "    ";
		bankMat.Allo[i].dis();
		cout << "    ";
		bankMat.Need[i].dis();
		if(!i){
			cout << "    ";
			bankMat.Avai.dis();
		}
		cout << endl;
	}
	cout << endl;
}
struct {
	myVec Work = myVec(0,RESOURCE_KIND);
	vector <myVec> & Need = bankMat.Need;
	vector <myVec> & Allo = bankMat.Allo;
	vector <bool> Finish;
} safeMat; // 安全性算法所使用的数据结构
vector <int> safeSeq; // 递归过程中用来存放安全序列的数据结构
int safeSeqNum; // 记录安全序列的个数
void dfs(int cnt){ // 搜索所有安全序列
	if(cnt == PROCESS_NUM){
		if(!safeSeqNum)
			cout << "The current status is safe!" << endl << "Here are all of the safe sequences:" << endl;
		safeSeqNum ++;
		bool isFirst = true;
		for(int i = 0;i < PROCESS_NUM;i ++){
			if(isFirst){
				cout << "P" << safeSeq[i];
				isFirst = false;
			}
			else
				cout << " -> " << "P" << safeSeq[i];
		}
		cout << endl;
		return ;
	}
	for(int i = 0;i < PROCESS_NUM;i ++)
		if(find(safeSeq.begin(),safeSeq.end(),i) == safeSeq.end() && safeMat.Finish[i] == false && safeMat.Need[i] <= safeMat.Work){
			safeSeq.push_back(i);
			safeMat.Work = safeMat.Work + safeMat.Allo[i];
			safeMat.Finish[i] == true;
			dfs(cnt + 1);
			safeMat.Finish[i] == false;
			safeMat.Work = safeMat.Work - safeMat.Allo[i];
			safeSeq.pop_back();
		}
}
bool isSafe(){ // 安全性算法
	safeSeqNum = 0;
	safeMat.Work = bankMat.Avai;
	safeMat.Finish = vector <bool> (PROCESS_NUM,false);
	dfs(0);
	if(safeSeqNum)
		cout << "There are " << safeSeqNum << " safe sequences in total!" << endl;
	return safeSeqNum;
}
int main(void)
{
	disbankMat();
	isSafe();
	int pid;
	while(scanf("%d",&pid) != EOF){ // 循环处理每个请求
		myVec request = myVec();
		cout << endl << "A new request is coming:" << endl << "Request" << pid << " : ";
		request.dis();
		cout << endl;
		if(!(request <= bankMat.Need[pid])) // 如果不满足 request <= Need 应该拒绝分配
			cout << "This request has been denied because it ask for to much resource!" << endl;
		else if(!(request <= bankMat.Avai)) // 如果不满足 request <= Avai 应该拒绝分配
			cout << "This request has been denied because there don't have enough resource in system!" << endl;
		else{
			bankMat.Avai = bankMat.Avai - request; // 尝试分配资源
			bankMat.Allo[pid] = bankMat.Allo[pid] + request;
			bankMat.Need[pid] = bankMat.Need[pid] - request;
			if(!isSafe()){ // 如果分配完之后使得系统处于不安全状态 则撤销此次分配
				cout << "This request has been denied because system will enter a unsafe status after this distribution!" << endl;
				bankMat.Avai = bankMat.Avai + request;
				bankMat.Allo[pid] = bankMat.Allo[pid] - request;
				bankMat.Need[pid] = bankMat.Need[pid] + request;
			}
			else
				cout << "This request has been met!" << endl;
		}
	}
	return 0;
}