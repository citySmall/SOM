//SOM 自组织神经元类头文件

#ifndef _NEURON_H_
#define _NEURON_H_

#include <vector>
#include <set>
#include <string>
using std::string;
using std::vector;
using std::set;

//输入层神经元个数
#define ATTRNUM 9
//输出层当个竞争神经元类结构
class Neuron
{
public:
	Neuron(int flag);
	vector<double> getWW();//获取该神经元内星权向量
	int getInstancenum();//获取该神经元的模式个数
	set<string> getInstances();//获取该神经元的活跃模式
	double computeDis(vector<double> X);//计算新模式与内星权向量的距离
	double computeDisByCos(vector<double>X);
	int deleteInstance(string instance);//删除当前存在的实例
	void insertInstance(string instance);//插入新的实例
	void updateWWbyWTA(vector<double>X,double learnrate);//训练阶段更新内星权向量,通过Win-take-all原则“胜者为王”
	static int trianFlag;//控制训练迭代次数，直到没有所有神经元的内星权向量的值都稳定了便停止训练
	int tag;//标志该神经元
private:
	vector<double> ww;//每个神经元的内星权向量
	int m_Instancenum;//引起当前神经元兴奋的模式个数
	set<string> instances;//当前刺激模式集合
};
#endif