#include "RefFrame.h"


RefFrame::RefFrame(int i)
{
	if (i > REFFRAMEMAX) i = REFFRAMEMAX;
	if (i <= 0) i = 1;
	classRep[i - 1] = 1;
	value = i;
}
RefFrame::RefFrame(int i[])
{
	for (int x = 0; x < REFFRAMEMAX; x++){
		if (i[x] == 1){
			value = x+1;
			classRep[x] = 1;
			break;
		}
	}
}
int RefFrame::getClassFromDouble(double in){
	for (int i = 0; i < REFFRAMEMAX; i++){
		if (in <= (1.0 / REFFRAMEMAX * (i + 1))) return i + 1;
	}
	return REFFRAMEMAX;
}
RefFrame::~RefFrame()
{
}
