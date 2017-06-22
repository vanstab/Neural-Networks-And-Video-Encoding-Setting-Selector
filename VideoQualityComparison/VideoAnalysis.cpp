#include "VideoAnalysis.h"

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include "defs.h"

using namespace std;
VideoAnalysis::VideoAnalysis()
{
}


VideoAnalysis::~VideoAnalysis()
{
}

void VideoAnalysis::scaleDownAndAnalysis(VideoCapture* input, double changeAnalysis[SCALE_DOWN_WIDTH][SCALE_DOWN_HEIGHT]){
	int frameCount = input->get(CAP_PROP_FRAME_COUNT);
	double changePercent = (double)1 / frameCount;
	Mat org, scale, scalePre;
	//scales down the input to allow faster analysis
	input->read(org);
	resize(org, scalePre, cvSize(SCALE_DOWN_WIDTH, SCALE_DOWN_HEIGHT));
	
	//checks which pixels changed and adds a overall percent change value to the changePercentmatrix
	for (int i = 1; i <frameCount; i++){
		input->read(org);
		resize(org, scale, cvSize(SCALE_DOWN_WIDTH, SCALE_DOWN_HEIGHT));
	//checks to see how many pixels change from previous image to next
		for (int w = 0; w < SCALE_DOWN_WIDTH; w++){
			for (int h = 0; h < SCALE_DOWN_HEIGHT; h++){
				if (Scalar(scalePre.at<uchar>(h, w)).val[0] == Scalar(scale.at<uchar>(h, w)).val[0] &&
					Scalar(scalePre.at<uchar>(h, w)).val[1] == Scalar(scale.at<uchar>(h, w)).val[1] &&
					Scalar(scalePre.at<uchar>(h, w)).val[2] == Scalar(scale.at<uchar>(h, w)).val[2]
					){
					changeAnalysis[w][h] += changePercent;
				}
			}
		}
		scalePre = scale;
	}
	input->release();
}