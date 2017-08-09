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
