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
	{//���ѭ�����ƶ�ÿ��ʵ����������ϵ��
		//�����ǰ����û��ʵ�������ü������
		if(iterNet->getInstancenum() <= 0)
		{
			++iterNet;
			continue;
		}
		set<string> instances = iterNet->getInstances();//�������ѭ���Ľڵ�
		set<string>::iterator iterOutStr = instances.begin();
		double ai = 0;  //�ýڵ������ڲ���ƽ������
		double biMin = 0; //�����������
		while(iterOutStr != instances.end())
		{
			++count;
			//�Ѿ�������ýڵ������ڲ��ڵ����
			ai = this->computeDis(iterOutStr,instances);

			//�������ýڵ����ⲿ����ľ����ƽ��ֵ
			biMin = DBL_MAX;
			double bi = 0;
			vector<Neuron>::iterator iterinNet = SOMnet.begin();
			while(iterinNet != SOMnet.end())
			{
				if(iterinNet->getInstancenum() <= 0 || iterinNet == iterNet)
				{//�������b���ʵ������Ϊ0 ʱ���򲻼��㣬��������һ���࣬���ߵ�ǰ���������Ҫ�����ʵ�����������򲻼���
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
		//�˴�Ϊ������
		/*while(iterInt != cls.end())
		{
			cout<<*iterInt<<",";
			++iterInt;
		}
		cout<<std::endl;*/
		//ͳ��ÿ�����ĸ���
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