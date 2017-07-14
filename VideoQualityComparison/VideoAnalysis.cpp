#include "VideoAnalysis.h"
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
	int frameCount = ((int)input->get(CAP_PROP_FRAME_COUNT)-1);
	int frameRate = input->get(CAP_PROP_FPS);
	double changePercent = (double)1.0 / (double)(frameCount);
	Mat org, scale, scalePre;
	//scales down the input to allow faster analysis
	input->read(scalePre);
	//resize(org, scalePre, cvSize(SCALE_DOWN_WIDTH, SCALE_DOWN_HEIGHT));

	//checks which pixels changed and adds a overall percent change value to the changePercentmatrix
	for (int i = 1; i < frameCount; i++){
		//if (i % 10 == 0){
			input->read(scale);
			//resize(org, scale, cvSize(SCALE_DOWN_WIDTH, SCALE_DOWN_HEIGHT));
			//checks to see how many pixels change from previous image to next
			Mat dest;
			//removes the difference from orginal image and new image, returned black pixels if the same
			//cv::subtract(scalePre, scale, dest);
			dest = scale - scalePre;
			//counts any pixels that are black
			inRange(dest, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), dest);
			
			for (int w = 0; w < SCALE_DOWN_WIDTH; w++){
				for (int h = 0; h < SCALE_DOWN_HEIGHT; h++){
					
						if (Scalar(dest.at<uchar>(h, w)).val[0] != 255){
							changeAnalysis[w][h] += changePercent;
						}
					}	
			}
			scalePre = scale.clone();
	}
	input->release();
}