#pragma once
#include <opencv2/opencv.hpp>
#include "defs.h"
using namespace cv;
class VideoAnalysis
{
public:
	VideoAnalysis();
	~VideoAnalysis();
	void scaleDownAndAnalysis(VideoCapture*, double[SCALE_DOWN_WIDTH][SCALE_DOWN_HEIGHT]);
};

