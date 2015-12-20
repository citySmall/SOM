#ifndef _SOM_H_
#define _SOM_H_
#include "neuron.h"
//此处另外作为说明，由于要对数据进行迭代学习，那么就要将数据全部导入内存，避免过多的IO操作，这是在数据量比较少的情况下

class SOM
{
public:
	SOM();
	SOM(int num);
	void importInstance(string filename);//导入数据
	void SOMtest();//最终聚类测试
	vector<string> getInstances();
	vector<Neuron> getSOMnet();
	int getNeuronNum();
	void unsupervisedLearn();//非监督型学习，也就是聚类的过程
private:
	int m_neuronNum;
	vector<Neuron> SOMnet;//SOM神经网络
	vector<string> instances;//数据集
};
#endif