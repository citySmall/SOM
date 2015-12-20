#include "SOMSi.h"
#include <iostream>

#define FILEPATH "cancer.txt"
using std::cout;
using std::endl;
void main()
{
	SOM somT = SOM();
	somT.importInstance(FILEPATH);
	somT.unsupervisedLearn();
	SI si = SI();
	cout<<si.computeSi(somT)<<endl;
	si.supervisedEvaluateion(somT);
}