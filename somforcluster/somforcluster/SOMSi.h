#ifndef _SOMSI_H_
#define _SOMSI_H_

#include "SOM.h"

class SI
{
public:
	SI();
	//�Ǽල����
	double computeSi(SOM& som);
	//�ල�Ͷ���
	void supervisedEvaluateion(SOM &som);
	void strToClsArray(set<string>&instances,vector<int>&cls);
	double computeDis(set<string>::iterator& iterStr,set<string>& instances);//����ʵ��������ĳ����ľ���
	void strToVecDouble(string str,vector<double>& iterDou);
	double getSI();
private:
	double si;
};
#endif