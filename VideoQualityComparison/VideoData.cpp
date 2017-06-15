#include "VideoData.h"


VideoData::VideoData(string data)
{//get file open by deliminator
}
VideoData::VideoData(string path, string pathre, double sizeorg, double sizere, float same, int width, int height)
{
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
