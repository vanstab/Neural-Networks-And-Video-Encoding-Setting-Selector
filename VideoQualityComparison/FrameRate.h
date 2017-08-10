#pragma once
class FrameRate
{
public:
	FrameRate();
	~FrameRate();

	static int getClassFromDouble(double);
	static double getDoubleFromClass(int);
};

