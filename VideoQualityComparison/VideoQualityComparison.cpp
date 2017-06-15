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
		waitKey(25);
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
	if (compareFrames <= 0 || org.get(CV_CAP_PROP_FRAME_COUNT) < compareFrames){
		compareFrames = org.get(CV_CAP_PROP_FRAME_COUNT);
	}
	if (org.get(CV_CAP_PROP_FRAME_COUNT) != recoded.get(CV_CAP_PROP_FRAME_COUNT)){
		string error = "Frame total difference detected! org: " + to_string(org.get(CV_CAP_PROP_FRAME_COUNT)) + " re: " + to_string(recoded.get(CV_CAP_PROP_FRAME_COUNT));
		Logger::log(error, Logger::warning);
		compareFrames -= ((org.get(CV_CAP_PROP_FRAME_COUNT) - recoded.get(CV_CAP_PROP_FRAME_COUNT)));
	}
	Mat frameOrg, frameRecoded;
	
	/*TO DO::
		add random interval  of segments to check + set number of comparisons needed for each video 
	*/
	if (org.get(CAP_PROP_FRAME_WIDTH) != recoded.get(CAP_PROP_FRAME_WIDTH) ||
		org.get(CAP_PROP_FRAME_HEIGHT) != recoded.get(CAP_PROP_FRAME_HEIGHT)){
		Logger::log("Frame size differences deterted, skipping video analyse", Logger::error);
		return 0;
	}
	for (int i = 0; i < compareFrames; i++){
		org.read(frameOrg);
		recoded.read(frameRecoded);
		count += compareImage(frameOrg, frameRecoded);
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