#include "VideoQualityComparison.h"
VideoQualityComparison::VideoQualityComparison()
{
	 analyse.open("VideoAnalyse.txt", ios_base::app);
}

VideoQualityComparison::~VideoQualityComparison()
{
	analyse.close();
}

/*
compares changes in images
input: original image, new image
output: similarity as a percentage
*/
float VideoQualityComparison::compareImage(Mat org, Mat recoded){
	float percentageTheSame, count;
	Mat dest;
	//removes the difference from orginal image and new image, returned black pixels if the same
	dest = recoded - org;
	//counts any pixels that are black
	inRange(dest, cv::Scalar(0,0,0), cv::Scalar(3,3,3), dest);
	count = countNonZero(dest);
	//if testing set to true display changes detected, white pixels are good black is loss of info
	if (TESTING){
		Mat videos;
		hconcat(org, recoded, videos);
		imshow("Comparison", dest);
		imshow("Videos", videos);
		waitKey(10);
	}

	//percentage of change
	percentageTheSame = (count / org.size().area()) * 100;
	return percentageTheSame;
}

/*
compares changes in video
input: original video, new video
output: similarity as a percentage
*/

float VideoQualityComparison::compareVideo(VideoCapture org, VideoCapture recoded, int compareFrames){

	float count = 0;
	double ratio = 1.0;
	if (compareFrames <= 0 || org.get(CV_CAP_PROP_FRAME_COUNT) < compareFrames){
		compareFrames = org.get(CV_CAP_PROP_FRAME_COUNT);
	}
	if (org.get(CV_CAP_PROP_FRAME_COUNT) != recoded.get(CV_CAP_PROP_FRAME_COUNT)){
		if (org.get(CAP_PROP_FPS) != recoded.get(CAP_PROP_FPS)){
			string error = "FPS difference detected! org: " + to_string(org.get(CAP_PROP_FPS)) + " re: " + to_string(recoded.get(CAP_PROP_FPS));
			Logger::log(error, Logger::warning);
			ratio = org.get(CAP_PROP_FPS) / recoded.get(CAP_PROP_FPS);
			
			compareFrames = compareFrames / ratio;
		}
		if (ratio == 1.0){
			string error = "Frame total difference detected! org: " + to_string(org.get(CV_CAP_PROP_FRAME_COUNT)) + " re: " + to_string(recoded.get(CV_CAP_PROP_FRAME_COUNT));
			Logger::log(error, Logger::warning);
			compareFrames -= ((org.get(CV_CAP_PROP_FRAME_COUNT) - recoded.get(CV_CAP_PROP_FRAME_COUNT)));
		}
		else if(compareFrames != recoded.get(CV_CAP_PROP_FRAME_COUNT)){
			string error = "Frame total difference detected! org: " + to_string(org.get(CV_CAP_PROP_FRAME_COUNT)) + " re: " + to_string(recoded.get(CV_CAP_PROP_FRAME_COUNT));
			Logger::log(error, Logger::warning);
			compareFrames -= ((org.get(CV_CAP_PROP_FRAME_COUNT)/ratio - recoded.get(CV_CAP_PROP_FRAME_COUNT)));
		}
	}
	Mat frameOrg, frameRecoded;
	
	if (org.get(CAP_PROP_FRAME_WIDTH) != recoded.get(CAP_PROP_FRAME_WIDTH) ||
		org.get(CAP_PROP_FRAME_HEIGHT) != recoded.get(CAP_PROP_FRAME_HEIGHT)){
		Logger::log("Frame size differences detected, skipping video analyse", Logger::error);
		return 0;
	}
	for (int i = 1; i < compareFrames+1; i++){
		if (i % 999==0 && round(ratio) != ratio)
			org.read(frameOrg);
		else if ((ratio == 1.0 || fmod(i, round(ratio)) != 0)){
			org.read(frameOrg);
			recoded.read(frameRecoded);
			count += compareImage(frameOrg, frameRecoded);
		}
		else{
			org.read(frameOrg);
			//count += compareImage(frameOrg, frameRecoded);
		}
	}
	count /= compareFrames;
	return count;
}

float VideoQualityComparison::compareQuality(VideoData *data){
	VideoCapture source(data->getPathOrg());
	VideoCapture reencoded(data->getPathRe());
	if (!source.isOpened()){
		Logger::log("Failed to load source video", Logger::error);
		return -1;
	}
	if (!reencoded.isOpened()){
		Logger::log("Failed to load recoded video", Logger::error);
		return -1;
	}
	float value;

	//enabled if testing
	if (TESTING){
		namedWindow("Comparison", WINDOW_NORMAL);
		resizeWindow("Comparison", 640, 360);
		namedWindow("Videos", WINDOW_NORMAL);
		resizeWindow("Videos", 1280,360);
		moveWindow("Comparison", 460, 460);
		moveWindow("Videos", 100,100);
	}
	data->setFrameHeight(source.get(CAP_PROP_FRAME_HEIGHT));
	data->setFrameWidth(source.get(CAP_PROP_FRAME_WIDTH));
	logAnalyseInfo(to_string(source.get(CAP_PROP_FRAME_WIDTH)));
	logAnalyseInfo(to_string(reencoded.get(CAP_PROP_FRAME_HEIGHT)));
	value = compareVideo(source, reencoded,-1);
	
	if (TESTING){
		destroyWindow("Comparison");
		destroyWindow("Videos");
	}

	source.release();
	reencoded.release();
	logAnalyseInfo(to_string(value / 100));
	data->setPercentSame(value);
	return value;
}

void VideoQualityComparison::logAnalyseInfo(string log){
	analyse << log << " ";
}