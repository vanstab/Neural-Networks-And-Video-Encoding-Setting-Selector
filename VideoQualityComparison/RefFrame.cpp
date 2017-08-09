#include "RefFrame.h"


RefFrame::RefFrame()
{
}

int RefFrame::getClassFromDouble(double in){
	for (double i = 1; i <= REFFRAMEMAX; i++){
		if (in <= (i / (double)REFFRAMEMAX)) return i;
	}
	return 1;
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
