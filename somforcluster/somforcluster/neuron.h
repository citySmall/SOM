//SOM ����֯��Ԫ��ͷ�ļ�

#ifndef _NEURON_H_
#define _NEURON_H_

#include <vector>
#include <set>
#include <string>
using std::string;
using std::vector;
using std::set;

//�������Ԫ����
#define ATTRNUM 9
//����㵱��������Ԫ��ṹ
class Neuron
{
public:
	Neuron(int flag);
	vector<double> getWW();//��ȡ����Ԫ����Ȩ����
	int getInstancenum();//��ȡ����Ԫ��ģʽ����
	set<string> getInstances();//��ȡ����Ԫ�Ļ�Ծģʽ
	double computeDis(vector<double> X);//������ģʽ������Ȩ�����ľ���
	double computeDisByCos(vector<double>X);
	int deleteInstance(string instance);//ɾ����ǰ���ڵ�ʵ��
	void insertInstance(string instance);//�����µ�ʵ��
	void updateWWbyWTA(vector<double>X,double learnrate);//ѵ���׶θ�������Ȩ����,ͨ��Win-take-allԭ��ʤ��Ϊ����
	static int trianFlag;//����ѵ������������ֱ��û��������Ԫ������Ȩ������ֵ���ȶ��˱�ֹͣѵ��
	int tag;//��־����Ԫ
private:
	vector<double> ww;//ÿ����Ԫ������Ȩ����
	int m_Instancenum;//����ǰ��Ԫ�˷ܵ�ģʽ����
	set<string> instances;//��ǰ�̼�ģʽ����
};
#endif