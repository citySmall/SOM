#include "SOM.h"
#include <iostream>
#include <fstream>
#include <map>

//boost库类调用
#include <boost\algorithm\string.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\format.hpp>

//默认竞争层的神经元的个数
#define DEFAULT_NEURON_NUM 2
//改SOM算法中的学习率取1/t,页就是迭代次数的倒数
//迭代学习次数
#define T 500

using std::map;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::runtime_error;
using std::cerr;
using std::cout;
using std::endl;
using std::fstream;
using boost::trim;
using boost::lexical_cast;
using boost::format;
using boost::split;

SOM::SOM()
{
	this->m_neuronNum = DEFAULT_NEURON_NUM;
	for(int i=0; i<DEFAULT_NEURON_NUM; ++i)
	{
		this->SOMnet.push_back(*(new Neuron(i)));
	}
}

SOM::SOM(int num)
{
	this->m_neuronNum = num;
	for(int i=0; i<num; ++i)
	{
		this->SOMnet.push_back(*(new Neuron(i)));
	}
}

void SOM::importInstance(string filename)
{
	ifstream infile;
	try{
		infile.open(filename);
		if(!infile)
		{
			throw runtime_error((format("The file \"%s\" dose not exit,please check the filename.") %filename).str());
		}
		string line;
		int i =0;
		while(getline(infile,line))
		{
			trim(line);
			this->instances.push_back(line);
			cout<<++i<<endl;
		}
	}
	catch(const runtime_error &e)
	{
		cerr<<e.what()<<" Try again!"<<endl;
	}
}

vector<string> SOM::getInstances()
{
	return this->instances;
}

vector<Neuron> SOM::getSOMnet()
{
	return this->SOMnet;
}

int SOM::getNeuronNum()
{
	return this->m_neuronNum;
}

void SOM::unsupervisedLearn()
{
	//这里还能够在算法运行时间上做改进
	map<string,vector<Neuron>::iterator> strNeur;
	vector<vector<double>> douIns;//转换为数值型的数据
	vector<string>::iterator iterIns = this->instances.begin();//多次迭代实例
	while(iterIns != this->instances.end())
	{
		vector<double>temp;//用于转换
		vector<string> lSplit;
		split(lSplit,*iterIns,boost::is_any_of(" "));
		if(lSplit.size()<ATTRNUM+1)
		{
			iterIns = this->instances.erase(iterIns);
			continue;
		}
		vector<string>::iterator iterlsplit = lSplit.begin();
		while(iterlsplit!=lSplit.end())
		{
			temp.push_back(boost::lexical_cast<double>(*iterlsplit));
			++iterlsplit;
		}
		douIns.push_back(temp);
		++iterIns;
	}
	iterIns = this->instances.begin();
	int i = 0;
	double learnrate = 0;
	do{
		Neuron::trianFlag = 0;
		
		vector<vector<double>>::iterator iterDou = douIns.begin();
		while(iterDou != douIns.end())
		{
			double minDis = -1;//当前的最小距离
			vector<Neuron>::iterator iterMax;//当前具有最小距离的神经元的索引
			vector<Neuron>::iterator iterNeur = this->SOMnet.begin();//SOMnet容器
			while(iterNeur != this->SOMnet.end())
			{
				double dis = (*iterNeur).computeDis(*iterDou);
				if(minDis>dis||minDis<=0)
				{
					minDis = dis;
					iterMax = iterNeur;
				}
				++iterNeur;
			}
			//cout<<&iterMax<<endl;
			//此处已经找到最近的神经元
			learnrate = 1.0/(i+1);

			/*这里可以选择是否考虑兴奋神经元的侧抑制现象来调节各神经元的内行权向量的调整*/
			(*iterMax).updateWWbyWTA(*iterDou,learnrate);
			//建立string和Neuron的映射
			string str = *(iterIns+(iterDou-douIns.begin()));
			iterMax->insertInstance(str);
			map<string,vector<Neuron>::iterator>::iterator temp = strNeur.find(str);
			
			if(temp != strNeur.end())
			{
				if(temp->second != iterMax)
				{
					(temp->second)->deleteInstance(str);
					temp->second = iterMax;
					Neuron::trianFlag += 1;
				}
			}
			else
			{
				strNeur[str] = iterMax;
				Neuron::trianFlag += 1;
			}
			++iterDou;
		}
		cout<<++i<<" "<<endl;
	}while(i<T);

	//将聚类结果写入txt文件
	ofstream fwrite("cluster.txt");
	if(!fwrite)return; //打开写文件失败

	vector<Neuron>::iterator iter= this->SOMnet.begin();
	while(iter != this->SOMnet.end())
	{
		vector<double> ww = iter->getWW();
		vector<double>::iterator iterWW = ww.begin();
		set<string> instances = iter->getInstances();
		if(instances.size()==0)
		{
			++iter;
			continue;
		}
		set<string>::iterator instance = instances.begin();
		while(iterWW!=ww.end())
		{
			fwrite<<*iterWW<<" ";
			cout<<*iterWW<<" ";
			++iterWW;
		}
		fwrite<<"聚类个数:"<<instances.size()<<endl;
		cout<<"聚类个数:"<<instances.size()<<endl;
		while(instance != instances.end())
		{
			fwrite<<*instance<<endl;
			cout<<*instance<<endl;
			++instance;
		}
		cout<<endl;
		++iter;
	}
}