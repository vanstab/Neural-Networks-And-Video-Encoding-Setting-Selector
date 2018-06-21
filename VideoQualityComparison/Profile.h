#pragma once
#include <string>

#include "Neuron.h"
using namespace std;
class Profile
{
public:
	Profile();
	~Profile();
	static int getClassFromDouble(double);
	static double getDoubleFromClass(int);
	static string getStringValue(Neuron** );
};

