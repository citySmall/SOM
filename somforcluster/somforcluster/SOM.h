#ifndef _SOM_H_
#define _SOM_H_
#include "neuron.h"
//�˴�������Ϊ˵��������Ҫ�����ݽ��е���ѧϰ����ô��Ҫ������ȫ�������ڴ棬��������IO�������������������Ƚ��ٵ������

class SOM
{
public:
	SOM();
	SOM(int num);
	void importInstance(string filename);//��������
	void SOMtest();//���վ������
	vector<string> getInstances();
	vector<Neuron> getSOMnet();
	int getNeuronNum();
	void unsupervisedLearn();//�Ǽල��ѧϰ��Ҳ���Ǿ���Ĺ���
private:
	int m_neuronNum;
	vector<Neuron> SOMnet;//SOM������
	vector<string> instances;//���ݼ�
};
#endif