#pragma once
#include "defs.h"
class BFrame
{
public:
	BFrame(int);
	BFrame(int[]);
	~BFrame();
	int value;
	int classRep[BFRAMEMAX];
	int getClassFromDouble(double);
};

