#include "RefFrame.h"


RefFrame::RefFrame()
{
}

int RefFrame::getClassFromDouble(double in){
	for (int i = 0; i < REFFRAMEMAX; i++){
		if (in <= (1.0 / (double)REFFRAMEMAX * (i + 1))) return i + 1;
	}
	return REFFRAMEMAX;
}

double RefFrame::RefFrameCompare(double in, double compareTo){
	int i, c;
	i = getClassFromDouble(in);
	c = getClassFromDouble(compareTo);
	if (i == c) return 1.0;
	else return getDoubleFromClass(i-c);
}
double RefFrame::getDoubleFromClass(int in){
	return (double)in / (double)REFFRAMEMAX;
}


RefFrame::~RefFrame()
{
}
