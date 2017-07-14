#include "BFrame.h"
BFrame::BFrame()
{
}
int BFrame::getClassFromDouble(double in){
	for (int i = 0; i < BFRAMEMAX; i++){
		if (in <= (1.0 / (double)BFRAMEMAX * (i + 1))) return i+1;
	}
	return BFRAMEMAX;
}

double BFrame::BFrameCompare(double in, double compareTo){
	int i, c;
	i = getClassFromDouble(in);
	c = getClassFromDouble(compareTo);
	if (i == c) return 1.0;
	else return getDoubleFromClass(i-c);
}
double BFrame::getDoubleFromClass(int in){
	return (double)in/(double) BFRAMEMAX;
}

BFrame::~BFrame()
{
}
