#include "TrainningSetBuilder.h"
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/opencv.hpp>
#include "VideoData.h"
#include "networkMath.h"
#include "TrainningSet.h"
#include "Profile.h"
#include "EncoderLevel.h"
#include "FrameRate.h"
using namespace std;
using namespace cv;
TrainningSetBuilder::TrainningSetBuilder()
{
}


TrainningSetBuilder::~TrainningSetBuilder()
{
}

int TrainningSetBuilder::build(){
	TrainningSet* trainset = new TrainningSet(0,0);
	TrainningSet* checkset = new TrainningSet(1,0);
	stringstream spath, ssoutput;
	ifstream readBitsTest, readBitsCheck,invid;
	random_device rd;
	mt19937 e2(rd());
	uniform_real_distribution<> dist(0, 1);
	readBitsTest.open(OUTPUT_TRAINING_BIT, ifstream::in);
	readBitsCheck.open(OUTPUT_CHECKING_BIT, ifstream::in);
	invid.open(INPUT_TRAINING_BIT, ifstream::in);
	for (int i = 1; i < VIDEO_CHECK_SET_SIZE + VIDEO_TRAIN_SET_SIZE; i++){
		
		string outputline;
		double* rowIn = new double[INPUT_SIZE];
		double* rowOut = new double[OUTPUT_NEURONS];
		spath.str("");
		spath << VIDEODATABASELOCATION << "pair" << i << "org.*";
		string pathOrg = spath.str();
		string pathRe;
		WIN32_FIND_DATA FindOrgData,FindReData;
		HANDLE hFind = FindFirstFile(pathOrg.c_str(), &FindOrgData);
		if (hFind == INVALID_HANDLE_VALUE){
			cout << "file not found" << endl;
		}
		else{
			spath.str("");
			spath << VIDEODATABASELOCATION << FindOrgData.cFileName;
			pathOrg = spath.str();
			spath.str("");
			spath << VIDEODATABASELOCATION << "pair" << i << "re1.mp4";
			pathRe = spath.str();
			HANDLE hFindRE = FindFirstFile(pathRe.c_str(), &FindReData);
			if (hFindRE == INVALID_HANDLE_VALUE){
				cout << "file not found: " << pathRe << endl;
			}
			else{
				LARGE_INTEGER size, sizere;
				size.HighPart = FindOrgData.nFileSizeHigh;
				size.LowPart = FindOrgData.nFileSizeLow;
				sizere.HighPart = FindReData.nFileSizeHigh;
				sizere.LowPart = FindReData.nFileSizeLow;
				VideoCapture* orgVideo = new VideoCapture(pathOrg);
				rowIn[0] = (size.QuadPart) / 1000000000.0;
				rowIn[1] = (sizere.QuadPart) / 1000000000.0; //In gigabytes
				rowIn[2] = orgVideo->get(CAP_PROP_FPS);
				rowIn[3] = orgVideo->get(CAP_PROP_FRAME_WIDTH);
				rowIn[4] = orgVideo->get(CAP_PROP_FRAME_HEIGHT);
				rowIn[5] = orgVideo->get(CAP_PROP_FRAME_COUNT)/orgVideo->get(CAP_PROP_FPS);//time
				
				//slow mo
				if (dist(e2) < 0.5)
					rowIn[6] = 0.0;
				else rowIn[6] = 1.0;

				//support
				if (dist(e2) <0.3)
					rowIn[7] = 0.0;
				else if (dist(e2) >0.6)
					rowIn[7] = 1.0;
				else rowIn[7] = 2.0;

				ssoutput.str("");
				getline(invid, outputline);
				ssoutput << outputline;
				for (int x = 8; x < INPUT_SIZE-1; x++){
					if (i % 2 != 0){
						getline(invid, outputline, ',');
						rowIn[x] = stod(outputline);
					}
					else{
						getline(invid, outputline, ',');
						rowIn[x] = stod(outputline);
					}
				}

				//based on the output of above a bit
				if (rowIn[3] <= 640){
					if (rowIn[2] <= 30)
						rowOut[0] = EncoderLevel::getDoubleFromClass(1);
					else rowOut[0] = EncoderLevel::getDoubleFromClass(2);
				}
				else if (rowIn[3] <=1280)
					if (rowIn[2] <=30)
						rowOut[0] = EncoderLevel::getDoubleFromClass(2);
					else if (rowIn[6] == 1 && rowIn[2] > 30)
						rowOut[0] = EncoderLevel::getDoubleFromClass(3);
					else rowOut[0] = EncoderLevel::getDoubleFromClass(2);
				else if (rowIn[3] <= 1920){
					if (rowIn[2] <= 30 && rowIn[6] != 1)
						rowOut[0] = EncoderLevel::getDoubleFromClass(4);
					else if (rowIn[6] == 1 && rowIn[2] > 30)
						rowOut[0] = EncoderLevel::getDoubleFromClass(6);
					else rowOut[0] = EncoderLevel::getDoubleFromClass(5);
				}
				else EncoderLevel::getDoubleFromClass(6);
				//profile
				if (rowIn[7] == 0.0 && rowIn[3] >= 1280) 
					rowOut[1] = Profile::getDoubleFromClass(1);
				else if (rowIn[7] == 0.0 && rowIn[3] < 1280)
					rowOut[1] = Profile::getDoubleFromClass(2);
				else if (rowIn[7] == 1.0)
					rowOut[1] = Profile::getDoubleFromClass(2);
				else if (rowIn[7] == 2.0)
					rowOut[1] = Profile::getDoubleFromClass(3);
				else rowOut[1] = Profile::getDoubleFromClass(2);

				if (rowIn[2] >50 && rowIn[6] != 1.0)
					rowOut[2] = FrameRate::getDoubleFromRate(floor(rowIn[2] - 30));
				else if(rowIn[2] >50 && rowIn[6] == 1.0)
					rowOut[2] = FrameRate::getDoubleFromRate(floor(rowIn[2]));
				else rowOut[2] = FrameRate::getDoubleFromRate(floor(rowIn[2]));

				//normalise values if needed
				rowIn[2] = FrameRate::getDoubleFromRate(floor(rowIn[2]));
				rowIn[3] = orgVideo->get(CAP_PROP_FRAME_WIDTH)/2160;
				rowIn[4] = orgVideo->get(CAP_PROP_FRAME_HEIGHT)/3840;
				rowIn[5] = (orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS))/14440;//time





			
				if (i % 2 != 0){
					ssoutput.str("");
					getline(readBitsTest, outputline);
					ssoutput << outputline;
					getline(ssoutput, outputline, ',');
					rowOut[3] = stod(outputline) / 10000.0;
					trainset->add(rowIn, rowOut);
				}
				else{
					ssoutput.str("");
					getline(readBitsCheck, outputline);
					ssoutput << outputline;
					getline(ssoutput, outputline, ',');
					rowOut[3] = stod(outputline) / 10000.0;
					checkset->add(rowIn, rowOut);
				}
			}
		}
	}
	readBitsCheck.close();
	readBitsTest.close();
	trainset->toFile();
	checkset->toFile();
}