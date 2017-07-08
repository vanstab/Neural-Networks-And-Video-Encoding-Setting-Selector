#pragma once
#include "defs.h"
class RefFrame
{
public:
	RefFrame(int);
	RefFrame(int[]);
	~RefFrame();
	int value;
	int classRep[REFFRAMEMAX];
	int getClassFromDouble(double);
};

