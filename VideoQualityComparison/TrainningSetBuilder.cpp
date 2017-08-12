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

	stringstream spath, ssoutput;
	ifstream readBitsTest, readBitsCheck,invid;
	random_device rd;
	mt19937 e2(rd());
	uniform_real_distribution<> dist(0, 1);

	random_device rand;
	mt19937 e4(rand());
	uniform_real_distribution<> distbit(20, 80);

	for (int itterations = 0; itterations < 75; itterations++){
		for (int i = 30; i < 211; i++){
			string outputline;
			double* rowIn = new double[INPUT_SIZE];
			spath.str("");
			if (itterations%2 == 0)
				spath << VIDEODATABASELOCATION << "pair" << i << "org.*";
			else spath << VIDEODATABASELOCATION << "pair" << i << "re.*";
			string pathOrg = spath.str();
			string pathRe;
			WIN32_FIND_DATA FindOrgData, FindReData;
			HANDLE hFind = FindFirstFile(pathOrg.c_str(), &FindOrgData);
			if (hFind == INVALID_HANDLE_VALUE){
				spath.str("");
				spath << VIDEODATABASELOCATION << "pair" << i << "org.*";
				string pathOrg = spath.str();
				hFind = FindFirstFile(pathOrg.c_str(), &FindOrgData);
			}

			if (hFind == INVALID_HANDLE_VALUE){
				cout << "error: file not found" << endl;
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
					LARGE_INTEGER size;
					size.HighPart = FindOrgData.nFileSizeHigh;
					size.LowPart = FindOrgData.nFileSizeLow;
					VideoCapture* orgVideo = new VideoCapture(pathOrg);
					rowIn[0] = (size.QuadPart) / 1000000000000.0; //size of org video in tb
					rowIn[1] = ((size.QuadPart)*distbit(e4) / 100) / 1000000000000.0; //wanted size in terabytes
					rowIn[2] = orgVideo->get(CAP_PROP_FPS);
					rowIn[3] = orgVideo->get(CAP_PROP_FRAME_WIDTH);
					rowIn[4] = orgVideo->get(CAP_PROP_FRAME_HEIGHT);
					rowIn[5] = orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS);//time
					rowIn[6] = size.QuadPart * 8 / (orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS)) / 1000000000;  //org bitrate in gbps

					//slow mo y/n
					if (dist(e2) < 0.5)
						rowIn[7] = 0.0;
					else rowIn[7] = 1.0;

					//support, all,most,few
					if (dist(e2) < 1.0 / 3.0)
						rowIn[8] = 1.0 / 3.0;
					else if (dist(e2) < 2.0 / 3.0)
						rowIn[8] = 2.0 / 3.0;
					else rowIn[8] = 1.0;


					//based on the output of above a bit
					double encOut[ENCODERMAX] = { 0, 0, 0, 0, 0, 0 };
					if (rowIn[3] <= 640){
						if (rowIn[2] <= 30)
							encOut[0] = 1;
						else encOut[1] = 1;
					}
					else if (rowIn[3] <= 1280){
						if (rowIn[2] <= 30)
							encOut[1] = 1;
						else if (rowIn[7] == 1 && rowIn[2] > 30)
							encOut[2] = 2;
						else encOut[1] = 1;
					}
					else if (rowIn[3] <= 1920){
						if (rowIn[2] <= 30 && rowIn[7] != 1)
							encOut[4] = 1;
						else if (rowIn[7] == 1 && rowIn[2] > 30)
							encOut[5] = 1;
						else encOut[3] = 1;
					}
					else encOut[5] = 1;


					//profile
					double profOut[MAXPROFILE] = { 0, 0, 0 };

					if (rowIn[8] <= 1.0 / 3.0 && rowIn[3] >= 1280){
						profOut[0] = 1;
					}
					else if (rowIn[8] <= 1.0 / 3.0 && rowIn[3] < 1280){
						profOut[1] = 1;
					}
					else if (rowIn[8] <= 2.0 / 3.0  && rowIn[3] >= 1280){
						profOut[1] = 1;
					}
					else if (rowIn[8] <= 2.0 / 3.0  && rowIn[3] < 1280){
						profOut[2] = 1;
					}
					else profOut[2] = 1;

					double frameOut[MAXFRAMERATE] = { 0, 0, 0, 0, 0, 0, 0, 0 };

					double temp;
					if (rowIn[2] >50 && rowIn[6] != 1.0)
						temp = floor(rowIn[2] - 30);
					else temp = floor(rowIn[2]);

					if (temp == 60) frameOut[7] = 1;
					else if (temp == 59) frameOut[6] = 1;
					else if (temp == 30)frameOut[5] = 1;
					else if (temp == 29)frameOut[4] = 1;
					else if (temp == 25)frameOut[3] = 1;
					else if (temp == 24)frameOut[2] = 1;
					else if (temp == 23)frameOut[1] = 1;
					else if (temp == 15)frameOut[0] = 1;
					else frameOut[7] = 1;
					//normalise values if needed
					rowIn[2] = rowIn[2] / 60.0;
					rowIn[3] = orgVideo->get(CAP_PROP_FRAME_WIDTH) / 2160;
					rowIn[4] = orgVideo->get(CAP_PROP_FRAME_HEIGHT) / 3840;
					rowIn[5] = (orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS)) / 14440;//time 


					ofstream inFile, encFile, profFile, frameFile, dataFile;
					double dataOut;
					if (itterations % 2 != 0){
						dataOut = rowIn[1] * 1000000000000.0 * 8 / (orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS)) / 1000000;
						inFile.open(TRAINNING_CHECK_INPUT_PATH, ios_base::app);
						encFile.open(CHECK_OUTPUT_ENCODING, ios_base::app);
						profFile.open(CHECK_OUTPUT_PROFILE, ios_base::app);
						frameFile.open(CHECK_OUTPUT_FRAMERATE, ios_base::app);
						dataFile.open(CHECK_OUTPUT_BITRATE, ios_base::app);
						for (int i = 0; i < INPUT_SIZE; i++){
							inFile << rowIn[i] << ",";
						}
						inFile << endl;
						for (int i = 0; i < ENCODERMAX; i++){
							encFile << encOut[i] << ",";
						}
						encFile << endl;
						for (int i = 0; i < MAXPROFILE; i++){
							profFile << profOut[i] << ",";
						}
						profFile << endl;
						for (int i = 0; i < MAXFRAMERATE; i++){
							frameFile << frameOut[i] << ",";
						}
						frameFile << endl;
						dataFile << dataOut << endl;

					}
					else{
						dataOut = rowIn[1]* 1000000000000.0 * 8 / (orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS)) / 1000000;
						inFile.open(TRAINNING_TEST_INPUT_PATH, ios_base::app);
						encFile.open(TRAINNING_OUTPUT_ENCODING, ios_base::app);
						profFile.open(TRAINNING_OUTPUT_PROFILE, ios_base::app);
						frameFile.open(TRAINNING_OUTPUT_FRAMERATE, ios_base::app);
						dataFile.open(TRAINNING_OUTPUT_BITRATE, ios_base::app);
						for (int i = 0; i < INPUT_SIZE; i++){
							inFile << rowIn[i] << ",";
						}
						inFile << endl;
						for (int i = 0; i < ENCODERMAX; i++){
							encFile << encOut[i] << ",";
						}
						encFile << endl;
						for (int i = 0; i < MAXPROFILE; i++){
							profFile << profOut[i] << ",";
						}
						profFile << endl;

						for (int i = 0; i < MAXFRAMERATE; i++){
							frameFile << frameOut[i] << ",";
						}
						frameFile << endl;
						dataFile << dataOut << endl;
					}

					inFile.close();
					encFile.close();
					profFile.close();
					frameFile.close();
					dataFile.close();
					delete rowIn;
					orgVideo->release();
					
				}

			}
		}
	}
	readBitsCheck.close();
	readBitsTest.close();
	return 1;
}
/*for (int i = 1; i < VIDEO_CHECK_SET_SIZE + VIDEO_TRAIN_SET_SIZE; i++){

			string outputline;
			double* rowIn = new double[INPUT_SIZE];
			spath.str("");
			spath << VIDEODATABASELOCATION << "pair" << i << "org.*";
			string pathOrg = spath.str();
			string pathRe;
			WIN32_FIND_DATA FindOrgData, FindReData;
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
					rowIn[0] = (size.QuadPart) / 1000000000000.0; //size of org video in tb
					rowIn[1] = (sizere.QuadPart) / 1000000000000.0; //wanted size in terabytes
					rowIn[2] = orgVideo->get(CAP_PROP_FPS);
					rowIn[3] = orgVideo->get(CAP_PROP_FRAME_WIDTH);
					rowIn[4] = orgVideo->get(CAP_PROP_FRAME_HEIGHT);
					rowIn[5] = orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS);//time
					rowIn[6] = size.QuadPart * 8 / (orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS)) / 1000000000;  //org bitrate in gbps

					//slow mo y/n
					if (dist(e2) < 0.5)
						rowIn[7] = 0.0;
					else rowIn[7] = 1.0;

					//support, all,most,few
					if (dist(e2) < 1.0 / 3.0)
						rowIn[8] = 1.0 / 3.0;
					else if (dist(e2) < 2.0 / 3.0)
						rowIn[8] = 2.0 / 3.0;
					else rowIn[8] = 1.0;


					//based on the output of above a bit
					double encOut[ENCODERMAX] = { 0, 0, 0, 0, 0, 0 };
					if (rowIn[3] <= 640){
						if (rowIn[2] <= 30)
							encOut[0] = 1;
						else encOut[1] = 1;
					}
					else if (rowIn[3] <= 1280){
						if (rowIn[2] <= 30)
							encOut[1] = 1;
						else if (rowIn[7] == 1 && rowIn[2] > 30)
							encOut[2] = 2;
						else encOut[1] = 1;
					}
					else if (rowIn[3] <= 1920){
						if (rowIn[2] <= 30 && rowIn[7] != 1)
							encOut[4] = 1;
						else if (rowIn[7] == 1 && rowIn[2] > 30)
							encOut[5] = 1;
						else encOut[3] = 1;
					}
					else encOut[5] = 1;


					//profile
					double profOut[MAXPROFILE] = { 0, 0, 0 };

					if (rowIn[8] <= 1.0 / 3.0 && rowIn[3] >= 1280){
						profOut[0] = 1;
					}
					else if (rowIn[8] <= 1.0 / 3.0 && rowIn[3] < 1280){
						profOut[1] = 1;
					}
					else if (rowIn[8] <= 2.0 / 3.0  && rowIn[3] >= 1280){
						profOut[1] = 1;
					}
					else if (rowIn[8] <= 2.0 / 3.0  && rowIn[3] < 1280){
						profOut[2] = 1;
					}
					else profOut[2] = 1;

					double frameOut[MAXFRAMERATE] = { 0, 0, 0, 0, 0, 0, 0, 0 };

					double temp;
					if (rowIn[2] >50 && rowIn[6] != 1.0)
						temp = floor(rowIn[2] - 30);
					else temp = floor(rowIn[2]);

					if (temp == 60) frameOut[7] = 1;
					else if (temp == 59) frameOut[6] = 1;
					else if (temp == 30)frameOut[5] = 1;
					else if (temp == 29)frameOut[4] = 1;
					else if (temp == 25)frameOut[3] = 1;
					else if (temp == 24)frameOut[2] = 1;
					else if (temp == 23)frameOut[1] = 1;
					else if (temp == 15)frameOut[0] = 1;
					else frameOut[7] = 1;
					//normalise values if needed
					rowIn[2] = rowIn[2] / 60.0;
					rowIn[3] = orgVideo->get(CAP_PROP_FRAME_WIDTH) / 2160;
					rowIn[4] = orgVideo->get(CAP_PROP_FRAME_HEIGHT) / 3840;
					rowIn[5] = (orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS)) / 14440;//time 


					ofstream inFile, encFile, profFile, frameFile, dataFile;
					double dataOut;
					if (i % 2 != 0){
						dataOut = sizere.QuadPart * 8 / (orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS)) / 1000000;
						inFile.open(TRAINNING_CHECK_INPUT_PATH, ios_base::app);
						encFile.open(CHECK_OUTPUT_ENCODING, ios_base::app);
						profFile.open(CHECK_OUTPUT_PROFILE, ios_base::app);
						frameFile.open(CHECK_OUTPUT_FRAMERATE, ios_base::app);
						dataFile.open(CHECK_OUTPUT_BITRATE, ios_base::app);
						for (int i = 0; i < INPUT_SIZE; i++){
							inFile << rowIn[i] << ",";
						}
						inFile << endl;
						for (int i = 0; i < ENCODERMAX; i++){
							encFile << encOut[i] << ",";
						}
						encFile << endl;
						for (int i = 0; i < MAXPROFILE; i++){
							profFile << profOut[i] << ",";
						}
						profFile << endl;
						for (int i = 0; i < MAXFRAMERATE; i++){
							frameFile << frameOut[i] << ",";
						}
						frameFile << endl;
						dataFile << dataOut << endl;

					}
					else{
						dataOut = sizere.QuadPart * 8 / (orgVideo->get(CAP_PROP_FRAME_COUNT) / orgVideo->get(CAP_PROP_FPS)) / 1000000;
						inFile.open(TRAINNING_TEST_INPUT_PATH, ios_base::app);
						encFile.open(TRAINNING_OUTPUT_ENCODING, ios_base::app);
						profFile.open(TRAINNING_OUTPUT_PROFILE, ios_base::app);
						frameFile.open(TRAINNING_OUTPUT_FRAMERATE, ios_base::app);
						dataFile.open(TRAINNING_OUTPUT_BITRATE, ios_base::app);
						for (int i = 0; i < INPUT_SIZE; i++){
							inFile << rowIn[i] << ",";
						}
						inFile << endl;
						for (int i = 0; i < ENCODERMAX; i++){
							encFile << encOut[i] << ",";
						}
						encFile << endl;
						for (int i = 0; i < MAXPROFILE; i++){
							profFile << profOut[i] << ",";
						}
						profFile << endl;

						for (int i = 0; i < MAXFRAMERATE; i++){
							frameFile << frameOut[i] << ",";
						}
						frameFile << endl;
						dataFile << dataOut << endl;
					}

					inFile.close();
					encFile.close();
					profFile.close();
					frameFile.close();
					dataFile.close();

				}

			}
			}*/