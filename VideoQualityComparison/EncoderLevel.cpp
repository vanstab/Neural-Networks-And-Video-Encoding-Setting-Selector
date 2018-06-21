#include "EncoderLevel.h"
#include "defs.h"
EncoderLevel::EncoderLevel()
{
}


EncoderLevel::~EncoderLevel()
{
}

int EncoderLevel::getClassFromDouble(double in){
	for (double i = 1; i <= ENCODERMAX; i++){
		if (in <= (i / (double)ENCODERMAX )) return i;
	}
	return 1;
}


double EncoderLevel::getDoubleFromClass(int in){
	return (double)in / (double)ENCODERMAX;
}

string EncoderLevel::getStringValue(Neuron** in){
	double best = -1;
	int out = -1;
	for (int i = 0; i < ENCODERMAX; i++){
		if (in[i]->activation>best){
			best = in[i]->activation;
			out = i;
		}
	}
	if (out == 0) return "3.0";
	else if (out == 1) return "3.1";
	else if (out == 2) return "3.2";
	else if (out == 3) return "4.0";
	else if (out == 4) return "4.1";
	else return "4.2";
}