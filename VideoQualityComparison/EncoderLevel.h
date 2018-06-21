#pragma once
#include <string>
#include "Neuron.h"
using namespace std;
class EncoderLevel
{
public:
	EncoderLevel();
	~EncoderLevel();
	static int getClassFromDouble(double);
	static double getDoubleFromClass(int);
	static string getStringValue(Neuron**);
};

