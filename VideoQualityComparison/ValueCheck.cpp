#include "ValueCheck.h"
#include "defs.h"
#include "RefFrame.h"
#include "BFrame.h"
#include <math.h>
ValueCheck::ValueCheck()
{
}


ValueCheck::~ValueCheck()
{
}
void ValueCheck::check(Neuron** output, double* expected,int& correct,int& bit,int& r, int& b){
	bool flag = true;
	if (!(abs((output[0]->activation * 15000) - (expected[0] * 15000)) <= 75)){
		bit++;
		flag = false;
	}
	if (RefFrame::getClassFromDouble(output[1]->activation) != RefFrame::getClassFromDouble(expected[1])){
		r++;
		flag = false;
	}
	if (BFrame::getClassFromDouble(output[2]->activation) != BFrame::getClassFromDouble(expected[2])){
		b++;
		flag = false;
	}
	if (flag) correct++;

}