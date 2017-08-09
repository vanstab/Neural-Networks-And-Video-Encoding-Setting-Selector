#pragma once
#include "Neuron.h"
class ValueCheck
{
public:
	ValueCheck();
	~ValueCheck();
	static void check(Neuron**, double*,int&,int&,int);
	
};

