#include "ValueCheck.h"
#include "defs.h"
#include "RefFrame.h"
#include "BFrame.h"
#include "EncoderLevel.h"
#include "FrameRate.h"
#include "Profile.h"
#include <math.h>
#include <iostream>
ValueCheck::ValueCheck()
{
}


ValueCheck::~ValueCheck()
{
}
void ValueCheck::check(Neuron** output, double* expected,int& correct,int& one, int type){
	bool flag = true;
/*	if (!(abs((output[0]->activation * 15000) - (expected[0] * 15000)) <= 75)){
		bit++;
		flag = false;
	}*
	if (RefFrame::getClassFromDouble(output[1]->activation) != RefFrame::getClassFromDouble(expected[1])){
		r++;
		flag = false;
	}
	if (BFrame::getClassFromDouble(output[2]->activation) != BFrame::getClassFromDouble(expected[2])){
		b++;
		flag = false;
	}*/
	if (type == 0 && EncoderLevel::getClassFromDouble(output[0]->activation) != EncoderLevel::getClassFromDouble(expected[0])){
		one++;
		flag = false;
	}
	else if (type == 1 && Profile::getClassFromDouble(output[0]->activation) != Profile::getClassFromDouble(expected[1])){
		one++;
		flag = false;
	}
	else if (type == 2 && FrameRate::getClassFromDouble(output[0]->activation) != FrameRate::getClassFromDouble(expected[2])){
		one++;
		flag = false;
	}
	else if (type == 3) {
		one++;
		flag = false;
	}


	if (flag) correct++;
	
}