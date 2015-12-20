#ifndef _SOMSI_H_
#define _SOMSI_H_

#include "SOM.h"

class SI
{
public:
	SI();
	//非监督度量
	double computeSi(SOM& som);
	//监督型度量
	void supervisedEvaluateion(SOM &som);
	void strToClsArray(set<string>&instances,vector<int>&cls);
	double computeDis(set<string>::iterator& iterStr,set<string>& instances);//计算实例样本与某个类的距离
	void strToVecDouble(string str,vector<double>& iterDou);
	double getSI();
private:
	double si;
};
#endif