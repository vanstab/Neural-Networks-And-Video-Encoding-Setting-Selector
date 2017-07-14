#pragma once
#include "defs.h"
class RefFrame
{
public:
	RefFrame();
	~RefFrame();
	static int getClassFromDouble(double);
	static double RefFrameCompare(double, double);
	static double getDoubleFromClass(int);
};

