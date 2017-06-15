#pragma once
#include <string>

using namespace std;
class VideoData
{
public:
	VideoData(string, string, double,double,float,int,int);
	VideoData(string);
	~VideoData();
private:
	string pathOrg;
	string pathRe;
	double sizeOrg;
	double sizeRe;
	float percentSame;
	int frameHeight;
	int frameWidth;
};

