#pragma once
class EncoderLevel
{
public:
	EncoderLevel();
	~EncoderLevel();
	static int getClassFromDouble(double);
	static double getDoubleFromClass(int);
};

