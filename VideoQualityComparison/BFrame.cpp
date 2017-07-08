#include "BFrame.h"
BFrame::BFrame(int i)
{
	if (i >= BFRAMEMAX) i = BFRAMEMAX;
	if (i < 0) i = 0;
	classRep[i] = 1;
	value = i;
}
BFrame::BFrame(int i[])
{
	for (int x = 0; x < BFRAMEMAX; x++){
		if (i[x] == 1){
			value = x;
			classRep[x] = 1;
			break;
		}
	}
}
int BFrame::getClassFromDouble(double in){
	for (int i = 0; i < BFRAMEMAX; i++){
		if (in <= (1.0 / BFRAMEMAX * (i + 1))) return i+1;
	}
	return BFRAMEMAX;
}


BFrame::~BFrame()
{
}
