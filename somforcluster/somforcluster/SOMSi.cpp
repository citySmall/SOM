#include "SOMSi.h"
#include <iostream>
#include <map>
#include <cfloat>
#include <algorithm>
#include <boost\lexical_cast.hpp>
#include <boost\algorithm\string.hpp>

using std::map;
using std::cout;
using std::pow;
using std::sqrt;

SI::SI()
{
	this->si = -2;
}
double SI::getSI()
{
	return this->si;
}

double SI::computeDis(set<string>::iterator& iterStr,set<string>& instances)
{
	double si = 0;
	vector<double>iterX;
	this->strToVecDouble(*iterStr,iterX);
	set<string>::iterator iterIns = instances.begin();
	while(iterIns != instances.end())
	{
		vector<double>iterY;
		this->strToVecDouble(*iterIns,iterY);
		double temp = 0;
		vector<double>::iterator iterXX = iterX.begin();
		vector<double>::iterator iterYY = iterY.begin();
		while(iterXX!=iterX.end()&&iterYY!=iterY.end())
		{
			temp += pow((*iterXX)-(*iterYY),2);
			++iterXX;
			++iterYY;
		}
		si += sqrt(temp);
		++iterIns;
	}
	return si/instances.size();
}
void SI::strToVecDouble(string str,vector<double>& iterDou)
{
	vector<string> lSplit;
	split(lSplit,str,boost::is_any_of(" "));
	lSplit.pop_back();
	vector<string>::iterator iter = lSplit.begin();
	while(iter!=lSplit.end())
	{
		iterDou.push_back(boost::lexical_cast<double>(*iter));
		++iter;
	}
}

double SI::computeSi(SOM &som)
{
	double si = 0;
	int count = 0;
	vector<Neuron> SOMnet = som.getSOMnet();
	vector<Neuron>::iterator iterNet = SOMnet.begin();
	while(iterNet != SOMnet.end())
	{//外层循环控制对每个实例结算轮廓系数
		//如果当前聚类没有实例，则不用计算聚类
		if(iterNet->getInstancenum() <= 0)
		{
			++iterNet;
			continue;
		}
		set<string> instances = iterNet->getInstances();//所有外层循环的节点
		set<string>::iterator iterOutStr = instances.begin();
		double ai = 0;  //该节点与类内部的平均距离
		double biMin = 0; //最近的类间距离
		while(iterOutStr != instances.end())
		{
			++count;
			//已经计算出该节点与其内部节点距离
			ai = this->computeDis(iterOutStr,instances);

			//下面计算该节点与外部个类的距离的平均值
			biMin = DBL_MAX;
			double bi = 0;
			vector<Neuron>::iterator iterinNet = SOMnet.begin();
			while(iterinNet != SOMnet.end())
			{
				if(iterinNet->getInstancenum() <= 0 || iterinNet == iterNet)
				{//当外面的b类的实例个数为0 时，则不计算，而调到下一个类，或者当前的类就是所要计算的实例所属的类则不计算
					++iterinNet;
					continue;
				}
				set<string> strSet = iterinNet->getInstances();
				bi = this->computeDis(iterOutStr,strSet);
				biMin = bi>biMin?biMin:bi;
				++iterinNet;
			}
			double tempSi = (biMin-ai)/(ai>biMin?ai:biMin);
			si += tempSi;
			++iterOutStr;
		}
		//count += iterNet->getInstancenum();
		++iterNet;
	}
	return si/count;
}

void SI::supervisedEvaluateion(SOM& som)
{
	vector<Neuron> SOMnet = som.getSOMnet();
	vector<Neuron>::iterator iter = SOMnet.begin();
	while(iter != SOMnet.end())
	{
		if(iter->getInstancenum()<= 0)
		{
			++iter;
			continue;
		}
		vector<int>cls;
		this->strToClsArray(iter->getInstances(),cls);
		vector<int>::iterator iterInt = cls.begin();
		//此处为输出类别
		/*while(iterInt != cls.end())
		{
			cout<<*iterInt<<",";
			++iterInt;
		}
		cout<<std::endl;*/
		//统计每个类别的个数
		map<int,int>clsMap;
		while(iterInt != cls.end())
		{
			++clsMap[*iterInt];
			++iterInt;
		}
		map<int,int>::iterator iterMap = clsMap.begin();
		map<int,int>::key_type clsKey = -1;
		map<int,int>::mapped_type clsNum = -1;
		double maxEntropy = 0;
		double entropy = 0;
		int count = 0;
		while(iterMap!=clsMap.end())
		{
			count += iterMap->second;
			entropy -= ((iterMap->second)*1.0/cls.size())*std::log((iterMap->second)*1.0/cls.size());
			if((iterMap->second)>clsNum)
			{
				clsKey = iterMap->first;
				clsNum = iterMap->second;
			}
			++iterMap;
		}
		maxEntropy = std::log(count*1.0);
		cout<<"class "<<clsKey<<" :"<<clsNum<<", Precision:"<<clsNum*1.0/cls.size()<<",Entropy: "<<entropy/maxEntropy<<std::endl;
		++iter;
	}
}

void SI::strToClsArray(set<string>&instances,vector<int>&cls)
{
	set<string>::iterator iterStr = instances.begin();
	while(iterStr != instances.end())
	{
		string str = (*iterStr).substr((*iterStr).size()-1,1);
		cls.push_back(boost::lexical_cast<int>(str));
		++iterStr;
	}
}