#include "FrameRate.h"
#include "defs.h"

FrameRate::FrameRate()
{
}


FrameRate::~FrameRate()
{
}


int FrameRate::getClassFromDouble(double in){
	for (double i = 1; i <= MAXFRAMERATE; i++){
		if (in <= (i / (double)MAXFRAMERATE)) return i;
	}
	return 1;
}


double FrameRate::getDoubleFromClass(int in){
	return (double)in / (double)MAXFRAMERATE;
}

string FrameRate::getStringValue(Neuron**in){
	double best = -1;
	int out = -1;
	for (int i = 0; i < MAXFRAMERATE; i++){
		if (in[i]->activation>best){
			best = in[i]->activation;
			out = i;
		}
	}
	if (out == 0) return "15";
	else if (out == 1) return "23.976";
	else if (out == 2) return "24";
	else if (out == 3) return "25";
	else if (out == 4) return "29.97";
	else if (out == 5) return "30";
	else if (out == 6) return "59.97";
	else return "60.0";
}