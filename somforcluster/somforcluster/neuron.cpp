#include "neuron.h"
#include <iostream>
#include <algorithm>
#include <ctime>
using std::pow;
using std::rand;
using std::sqrt;
using std::srand;


int Neuron::trianFlag = 0;
//构造函数
Neuron::Neuron(int flag)
{
	this->tag = flag;
	this->m_Instancenum = 0;
	srand(time(NULL));
	for(int i=0; i<ATTRNUM; ++i)
	{
		this->ww.push_back((rand()%1000)*1.0/1000);
	}
}

vector<double> Neuron::getWW()
{
	return this->ww;
}

set<string> Neuron::getInstances()
{
	return this->instances;
}

int Neuron::getInstancenum()
{
	return this->m_Instancenum;
}

//此处利用欧式距离计算相似度，可以尝试用其他的距离
double Neuron::computeDis(vector<double>X)
{
	double distance = 0;
	vector<double>::iterator iterX = X.begin();
	vector<double>::iterator iterWW = this->ww.begin();
	while(iterX!=X.end() & iterWW!= this->ww.end())
	{
		distance += pow((*iterX)-(*iterWW),2);
		++iterX;
		++iterWW;
	}
	return distance;
}
double Neuron::computeDisByCos(vector<double>X)
{
	double distance = 0;
	vector<double>::iterator iterX = X.begin();
	vector<double>::iterator iterWW = this->ww.begin();
	double XX = 0;
	double YY = 0;
	double XY = 0;
	while(iterX!=X.end() & iterWW!= this->ww.end())
	{
		XX = (*iterX)*(*iterX);
		YY = (*iterWW)*(*iterWW);
		XY = (*iterWW)*(*iterX);
		++iterX;
		++iterWW;
	}
	distance = XY/(sqrt(XX)*sqrt(YY));
	return 1-distance;
}

int Neuron::deleteInstance(string instance)
{
	//返回删除的个数，对于set中的集合只能是删除1个或者不存在该元素
	if(this->instances.count(instance))
	{
		this->m_Instancenum -= 1;
		return this->instances.erase(instance);
	}
	return 0;
}

void Neuron::insertInstance(string instance)
{
	this->instances.insert(instance);
	this->m_Instancenum += 1; 
}
void Neuron::updateWWbyWTA(vector<double>X,double learnrate)
{
	vector<double>::iterator iterX = X.begin();
	vector<double>::iterator iterWW = this->ww.begin();
	while(iterX!=X.end() & iterWW!=ww.end())
	{
		*iterWW += learnrate*((*iterX) - (*iterWW));
		++iterX;
		++iterWW;
	}
}