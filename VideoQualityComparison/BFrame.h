#pragma once
#include "defs.h"
class BFrame
{
public:
	BFrame();
	~BFrame();
	static int getClassFromDouble(double);
	static double BFrameCompare(double, double);
	static double getDoubleFromClass(int);
};

