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
