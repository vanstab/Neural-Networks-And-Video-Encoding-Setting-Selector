#pragma once
#include <string>

using namespace std;
class VideoData
{
public:
	VideoData(string, string, double,double,float,int,int);
	VideoData(string);
	VideoData();
	~VideoData();
	string toString();
	string getPathOrg();
	string getPathRe();
	void setSizeOrg(double);
	void setSizeRe(double);
	void setPercentSame(float);
	void setFrameHeight(int);
	void setFrameWidth(int);
	void setPathOrg(string);
	void setPathRe(string);
private:
	string pathOrg;
	string pathRe;
	double sizeOrg;
	double sizeRe;
	float percentSame;
	int frameHeight;
	int frameWidth;
};

