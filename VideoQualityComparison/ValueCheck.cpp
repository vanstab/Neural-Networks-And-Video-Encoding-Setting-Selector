#include "ValueCheck.h"
#include "defs.h"
#include "RefFrame.h"
#include "BFrame.h"
#include <math.h>
ValueCheck::ValueCheck()
{
}


ValueCheck::~ValueCheck()
{
}

int ValueCheck::check(Neuron** output, double* expected){
	if (!(abs((output[0]->activation * 15000) - (expected[0] * 15000) <= 100)))return -1;
	if (RefFrame::getClassFromDouble(output[1]->activation) != RefFrame::getClassFromDouble(expected[1])) return -2;
	if (BFrame::getClassFromDouble(output[2]->activation) != BFrame::getClassFromDouble(expected[2])) return -3;
	return 1;
}