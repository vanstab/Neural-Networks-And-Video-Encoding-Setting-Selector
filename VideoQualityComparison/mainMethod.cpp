
#include <windows.h>
#include "TrainningSet.h"
#include "videoQualityComparison.h"
#include "VideoData.h"
#include "VideoAnalysis.h"
#include "FeedForwardSigmoid.h"
#include "FeedForwardHyperTangent.h"
#include "TrainningSetBuilder.h"
#include <shellapi.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "EncoderLevel.h"
#include "Profile.h"
#include "FrameRate.h"
int main()
{
	//TrainningSetBuilder* build = new TrainningSetBuilder();
	//build->build();

	int DEPTH_OF_NETWORK =4;
	int* inEnc = new int[DEPTH_OF_NETWORK]{INPUT_SIZE,7,4,ENCODERMAX};
	int* inProf = new int[DEPTH_OF_NETWORK]{INPUT_SIZE, 3,3, MAXPROFILE};
	int* inFrame = new int[DEPTH_OF_NETWORK]{INPUT_SIZE, 5, 2, MAXFRAMERATE};
	cout << "Building Training Set..." <<endl;
	//builds the trainning set for each value;
	TrainningSet* trainningEncSet = new TrainningSet(0,0);
	TrainningSet* trainningProfSet = new TrainningSet(0, 1);
	TrainningSet* trainningFrameSet = new TrainningSet(0, 2);
	TrainningSet* checkEncSet = new TrainningSet(1, 0);
	TrainningSet* checkProfSet = new TrainningSet(1, 1);
	TrainningSet* checkFrameSet = new TrainningSet(1, 2);

	cout << "Training sets built" << endl;
	cout << "Building Nueral Network...";
	FeedForwardHyperTangent* feedEncoderLevel = new FeedForwardHyperTangent(inEnc, DEPTH_OF_NETWORK, trainningEncSet, checkEncSet);
	FeedForwardSigmoid* feedFrameRate = new FeedForwardSigmoid(inFrame, DEPTH_OF_NETWORK, trainningFrameSet, checkFrameSet);
	FeedForwardSigmoid* feedProfile = new FeedForwardSigmoid(inProf, DEPTH_OF_NETWORK, trainningProfSet, checkProfSet);
	cout << "Nueral Network built" << endl;
	cout << "Training...." << endl;
	cout << "Encoder Level trainning started...";
	feedEncoderLevel->train();
	cout << "completed!" << endl;

	cout << "Framerate trainning started...";
	feedFrameRate->train();
	cout << "completed!" << endl;

	cout << "Profile trainning started...";
	feedProfile->train();
	cout << "completed!" << endl;

	string input;
	double* rowIn = new double[9];
	while (true){
		string pathOrg;
		cout << "Would you like to find the optimal settings for a video? Y/N" << endl;
		getline(cin, input);
		string contProg;
		stringstream(input) >> contProg;
		if (contProg != "y" && contProg != "Y")
			break;
		while (true){
			cout << "Please enter in the file path of the video you would like to check" << endl;
			
			WIN32_FIND_DATA FindOrgData, FindReData;
			getline(cin, input);
			stringstream(input) >> pathOrg;
			HANDLE inputVideo = FindFirstFile(pathOrg.c_str(), &FindOrgData);
			if (inputVideo == INVALID_HANDLE_VALUE){
				cout << "Error: File not found";
			}
			else{
				try{
					VideoCapture* orgVideo = new VideoCapture(pathOrg);
					LARGE_INTEGER size;
					if (orgVideo->get(CAP_PROP_FRAME_COUNT) <= 0){
						cout << "File is not Video, please Enter a valid video file..." << endl;
						orgVideo->release();
					}
					else{
						size.HighPart = FindOrgData.nFileSizeHigh;
						size.LowPart = FindOrgData.nFileSizeLow;
						rowIn[0] = (size.QuadPart) / 1000000000000.0;
						rowIn[2] = orgVideo->get(CAP_PROP_FPS)/60;
						rowIn[3] = orgVideo->get(CAP_PROP_FRAME_WIDTH)/2160;
						rowIn[4] = orgVideo->get(CAP_PROP_FRAME_HEIGHT)/3840;
						rowIn[5] = orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS)/14440;
						rowIn[6] = size.QuadPart * 8 / (orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS)) / 1000000000;  //org bitrate in gbps	
						orgVideo->release();
						break;
					}
				}
				catch (exception &e){
					cout << "File is not Video" << endl;
				}
			}
		}
		double desiredSize = 0;
		int support = 2;

		cout << "Please enter in the desired size in MB" << endl;
		while (desiredSize == 0){
			getline(cin, input);
			stringstream(input) >> desiredSize;
			rowIn[1] = desiredSize / 1000000;
		}
		cout << "Would you like to play back the video in slow motion? Y/N... Default is no. " << endl;
		getline(cin, input);
		string answer;
		stringstream(input) >> answer;
		if (answer == "y" || answer == "Y")
			rowIn[7] = 1.0;
		else rowIn[7] = 0.0;
		cout << "Would you like the video to play back on all, most or, some devices? Please enter 1,2,3 respectivly. Default is 2." << endl;
		getline(cin, input);
		stringstream(input) >> support;
		if (support == 1)
			rowIn[8] = 1.0 / 3.0;
		else if (support == 3)
			rowIn[8] = 1.0;
		else rowIn[8] = 2.0 / 3.0;

		feedEncoderLevel->feedForward(rowIn);
		feedProfile->feedForward(rowIn);
		feedFrameRate->feedForward(rowIn);
		cout << EncoderLevel::getStringValue(feedEncoderLevel->neuralNetwork[DEPTH_OF_NETWORK - 1]) << endl;
		cout << Profile::getStringValue(feedProfile->neuralNetwork[DEPTH_OF_NETWORK - 1]) << endl;
		cout << FrameRate::getStringValue(feedFrameRate->neuralNetwork[DEPTH_OF_NETWORK - 1]) << endl;
		cout << rowIn[1] * 1000000000 * 8 / (rowIn[5] * 14440)<<" kbps"<<endl;
		
		cout << "Would you like to encode the video using theses settings? Y?N" << endl;
		getline(cin, input);
		stringstream(input) >> answer;
		if (answer == "y" || answer == "Y"){
			stringstream inputCommand;
			inputCommand << VIDEO_ENCODING_COMMAND <<" -i " << pathOrg<<" --encoder-level " << EncoderLevel::getStringValue(feedEncoderLevel->neuralNetwork[DEPTH_OF_NETWORK - 1]);
			inputCommand << " --encoder-profile " << Profile::getStringValue(feedProfile->neuralNetwork[DEPTH_OF_NETWORK - 1]);
			inputCommand << " -r " << FrameRate::getStringValue(feedFrameRate->neuralNetwork[DEPTH_OF_NETWORK - 1]);
			inputCommand << " -b " << round(rowIn[1] * 1000000000 * 8 / (rowIn[5] * 14440));
			cout << "Please enter the name of output video. If no path is included the video will be placed in the current dirrectory. Please do not include an extension." << endl;
			getline(cin, input);
			string pathRe;
			stringstream ss;
			ss << input <<".mp4";
			ss >> pathRe;
			inputCommand << " -o " << pathRe;
			string commandString;
			commandString = inputCommand.str();
			system(commandString.c_str());
			cout << "Would you like to compare the video to the original? Y?N" << endl;
			getline(cin, input);
			stringstream(input) >> answer;
			if (answer == "y" || answer == "Y"){
				VideoQualityComparison vidQaul = VideoQualityComparison();
				VideoData vid = VideoData();
				vid.setPathOrg(pathOrg);
				vid.setPathRe(pathRe);
				cout << "Percent the same: " << vidQaul.compareQuality(&vid) << endl;
			
			}
		}

	}
}
