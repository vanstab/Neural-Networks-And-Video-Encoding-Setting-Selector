#include "VideoData.h"
#include <sstream>
VideoData::VideoData(){}
VideoData::VideoData(string data)
{//get file open by deliminator
	stringstream ss;
	ss << data;
	getline(ss, pathOrg, ',');
	getline(ss, pathRe, ',');

	string temp;
	getline(ss, temp, ',');
	sizeOrg = stod(temp);
	getline(ss, temp, ',');
	sizeRe = stod(temp);

	getline(ss, temp, ',');
	percentSame = stof(temp);

	getline(ss, temp, ',');
	frameHeight = stoi(temp);
	getline(ss, temp);//last item in line for now
	frameWidth = stoi(temp);
}
VideoData::VideoData(string path, string pathre, double sizeorg, double sizere, float same, int width, int height)
{	//org is th actual data for trainning
	//re is the comparison results to tell network how "close" it is to expected result
	pathOrg = path;
	pathRe = pathre;
	sizeOrg = sizeOrg;
	sizeRe = sizere;
	percentSame = same;
	frameHeight = height;
	frameWidth = width;
}


VideoData::~VideoData()
{
}

string VideoData::toString(){
	stringstream ss;
	ss << pathOrg << ',';
	ss << pathRe << ',';
	ss << sizeOrg << ',';
	ss << sizeRe << ',';
	ss << percentSame << ',';
	ss << frameHeight << ',';
	ss << frameWidth << endl;
	return ss.str();
}
string VideoData::getPathOrg(){
	return pathOrg;
}

string VideoData::getPathRe(){
	return pathRe;
}
void VideoData::setFrameHeight(int h){
	frameHeight = h;
}
void VideoData::setFrameWidth(int w){
	frameWidth = w;
}
void VideoData::setPercentSame(float p){
	percentSame = p;
}
void VideoData::setSizeOrg(double o){
	sizeOrg = o;
}
void VideoData::setSizeRe(double r){
	sizeRe = r;
}
void VideoData::setPathOrg(string s){
	pathOrg = s;
}

void VideoData::setPathRe(string s){
	pathRe = s;
}