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
	int maxActSpot = -1;
	double curMaxValue = -1;
	if (type != 1){
		for (int i = 0; i < type; i++){
			if (output[i]->activation > curMaxValue){
				maxActSpot = i;
				curMaxValue = output[i]->activation;
			}
		}
		if (expected[maxActSpot] == 1.0)
			correct++;
		else one++;

	}
	else{//within 75 kbps
		if (!(abs(output[0]->activation - expected[0]) <= 0.000075)){
				one++;
		}
		else correct++;
	}
}