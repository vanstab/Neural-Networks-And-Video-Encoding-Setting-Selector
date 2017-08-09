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

double FrameRate::getDoubleFromRate(double in){
		if (in == 60) return 1 / (double)MAXFRAMERATE;
		if (in == 59)return 2 / (double)MAXFRAMERATE;
		if (in == 30)return 3 / (double)MAXFRAMERATE;
		if (in == 29)return 4 / (double)MAXFRAMERATE;
		if (in == 25)return 5 / (double)MAXFRAMERATE;
		if (in == 24)return 6 / (double)MAXFRAMERATE;
		if (in == 23)return 7 / (double)MAXFRAMERATE;
		if (in == 15)return 8 / (double)MAXFRAMERATE;

		return 0;
}
double FrameRate::getRateFromDouble(double in){
	if (in <= 1 / (double)MAXFRAMERATE) return 60.0;
	if (in <= 2 / (double)MAXFRAMERATE) return 59.94;
	if (in <= 3 / (double)MAXFRAMERATE) return 30.0;
	if (in <= 4 / (double)MAXFRAMERATE) return 29.97;
	if (in <= 6 / (double)MAXFRAMERATE) return 25.0;
	if (in <= 7 / (double)MAXFRAMERATE) return 24.0;
	if (in <= 8 / (double)MAXFRAMERATE) return 23.976;
	if (in <= 9 / (double)MAXFRAMERATE) return 15.0;
}