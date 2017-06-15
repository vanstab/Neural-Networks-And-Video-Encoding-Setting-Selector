#pragma once
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <windows.h>

#include "VideoData.h"
#include "defs.h"
#include "logger.h"
using namespace cv;
using namespace std;
class VideoQualityComparison
{
public:
	VideoQualityComparison();
	~VideoQualityComparison();
	float compareQuality(VideoData*);
	void logAnalyseInfo(string);
private:
	float compareImage(Mat,Mat);
	float compareVideo(VideoCapture, VideoCapture, int = FramesToCompare);
	ofstream  analyse;
};

