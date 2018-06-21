#pragma once
#include <string>
#include "Neuron.h"
using namespace std;
class FrameRate
{
public:
	FrameRate();
	~FrameRate();

	static int getClassFromDouble(double);
	static double getDoubleFromClass(int);
	static string getStringValue(Neuron**);
};

