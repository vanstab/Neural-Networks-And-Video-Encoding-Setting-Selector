#include "TrainningSet.h"
#include <windows.h>
#include "VideoQualityComparison.h"
#include "VideoAnalysis.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <tchar.h>
#include "networkMath.h"
#include "BFrame.h"
#include "RefFrame.h"
using namespace std;
TrainningSet::TrainningSet(int i,int x)
{
	size = 0;
	if (i==0){
		MAX_SIZE = VIDEO_TRAIN_SET_SIZE;
		//if (REBUILD_TRAINNING_DATA)
		list = new double*[MAX_SIZE];
		out = new double*[MAX_SIZE];

		inPath = TRAINNING_TEST_INPUT_PATH;
		if (x == 0){
			outPath = TRAINNING_OUTPUT_ENCODING;
			MAX_OUT_SIZE = ENCODERMAX;
		}
		else if (x == 1){
			outPath = TRAINNING_OUTPUT_PROFILE;
			MAX_OUT_SIZE = MAXPROFILE;
		}
		else if (x == 2){
			outPath = TRAINNING_OUTPUT_FRAMERATE;
			MAX_OUT_SIZE = MAXFRAMERATE;
		}
		else{
			outPath = TRAINNING_OUTPUT_BITRATE;
			MAX_OUT_SIZE = 1;
		}
	}
	if (i==1){
		MAX_SIZE = VIDEO_CHECK_SET_SIZE;
		//if (REBUILD_TRAINNING_DATA)
		list = new double*[MAX_SIZE];
		out = new double*[MAX_SIZE];

		inPath = TRAINNING_CHECK_INPUT_PATH;
		if (x == 0){
			outPath = TRAINNING_OUTPUT_ENCODING;
			MAX_OUT_SIZE = ENCODERMAX;
		}
		else if (x == 1){
			outPath = TRAINNING_OUTPUT_PROFILE;
			MAX_OUT_SIZE = MAXPROFILE;
		}
		else if (x == 2){
			outPath = TRAINNING_OUTPUT_FRAMERATE;
			MAX_OUT_SIZE = MAXFRAMERATE;
		}
		else{
			outPath = TRAINNING_OUTPUT_BITRATE;
			MAX_OUT_SIZE = 1;
		}
	}
	load();
}


TrainningSet::~TrainningSet()
{
	for (int i = size - 1; i >= 0; i--)
		delete list[i];
}
//load exsiting file
void TrainningSet::load(){
	ifstream input,output;
	input.open(inPath, ifstream::in);
	output.open(outPath, ifstream::in);
	ofstream temps;
	temps.open("temp.txt", ios_base::app);
	//sstream ss;
	string in,o;
	for (int i = 0; i < MAX_SIZE; i++){
		getline(input, in);
		getline(output, o);
		istringstream ins(in);
		istringstream outs(o);
		string temp;
		double* testSet = new double[INPUT_SIZE];
		double* tempOut = new double[MAX_OUT_SIZE];

		int tempCount = 0;
		while(getline(ins,in, ',')){	
			testSet[tempCount] = stod(in);
			tempCount++;
			if (tempCount == INPUT_SIZE) break;
		}
		tempCount = 0;
		while (getline(outs, o, ',')){
			tempOut[tempCount] = stod(o);
			temps << tempOut[tempCount] << ",";
			tempCount++;
			if (tempCount == MAX_OUT_SIZE) break;
		}
		temps << endl;
		add(testSet, tempOut);
	}
	input.close();
	output.close();
}

/*void TrainningSet::addIn(string s){
	sstream ss << s;
	string i;
	int temp = 0;
	
	while(temp <(SCALE_DOWN_WIDTH*SCALE_DOWN_HEIGHT + 5)){
		double[temp] = 
		temp++
	}


}*/


//create new file
/*
void TrainningSet::init(){
	VideoQualityComparison test;
	VideoData* videos[MAX_SIZE];
	int videoCount = 0;
	ifstream inputVideoFile, outputVideoFile;
	inputVideoFile.open(TRAINNING_INFO_PATH, ifstream::in);
	outputVideoFile.open(OUTPUT_TRAINING_INFO_PATH, ifstream::in);
	//loads data of previous videos
	string filePath = "D:/VideoTestDataSet/allVideos/";
	while (videoCount <= VIDEO_TRAIN_SET_TOTAL-1){
			stringstream spath, rpath;
			string pathOrg, pathRe;
			LARGE_INTEGER size, sizere;
			spath << filePath << "pair" << videoCount+1 << "org.*";
			rpath << filePath << "pair" << videoCount+1 << "re1.*";

			pathOrg = spath.str();
			pathRe = rpath.str();

			WIN32_FIND_DATA FindFileData, FindFileDataRE;

			HANDLE hFind = FindFirstFile(pathOrg.c_str(), &FindFileData);
			HANDLE hFindRE = FindFirstFile(pathRe.c_str(), &FindFileDataRE);
			if (hFind == INVALID_HANDLE_VALUE || hFindRE == INVALID_HANDLE_VALUE){
				cout << "file not found: " << pathOrg << endl;
				videoCount++;
			}
			else{
				spath.str("");
				spath << filePath << FindFileData.cFileName;

				rpath.str("");
				rpath << filePath << FindFileDataRE.cFileName;

				pathOrg = spath.str();
				pathRe = rpath.str();
				spath.str("");
				rpath.str("");

				size.HighPart = FindFileData.nFileSizeHigh;
				size.LowPart = FindFileData.nFileSizeLow;

				sizere.HighPart = FindFileDataRE.nFileSizeHigh;
				sizere.LowPart = FindFileDataRE.nFileSizeLow;
				videos[videoCount] = new VideoData();
				videos[videoCount]->setPathOrg(pathOrg);
				videos[videoCount]->setPathRe(pathRe);
				videos[videoCount]->setSizeOrg(size.QuadPart);
				videos[videoCount]->setSizeRe(sizere.QuadPart);
				videoCount++;
			}
	}
	VideoAnalysis* vidAnalysis = new VideoAnalysis();
	for (int i = 0; i < VIDEO_TRAIN_SET_TOTAL; i++){
		if (i % (VIDEO_TRAIN_SET_TOTAL / 10) == 0) cout << (i / ((VIDEO_TRAIN_SET_TOTAL/10))) << "0% done" << endl;

		int pos = 0;
		VideoCapture* cap = new VideoCapture(videos[i]->getPathOrg());
		videos[i]->setFrameHeight(cap->get(CAP_PROP_FRAME_HEIGHT));
		videos[i]->setFrameWidth(cap->get(CAP_PROP_FRAME_WIDTH));
		int frameRate = cap->get(CAP_PROP_FPS);
		int frames = cap->get(CAP_PROP_FRAME_COUNT);
		double temp[SCALE_DOWN_WIDTH][SCALE_DOWN_HEIGHT] = { { 0 } };
		vidAnalysis->scaleDownAndAnalysis(cap, temp);
		double* testSet = new double[(SCALE_DOWN_WIDTH*SCALE_DOWN_HEIGHT + 5)];
		for (int x = 0; x < SCALE_DOWN_WIDTH; x++){
			for (int y = 0; y < SCALE_DOWN_HEIGHT; y++){
				testSet[pos++] = temp[x][y];
			}
		}
		testSet[pos++] = videos[i]->getHeight() / 9 / 250;
		testSet[pos++] = videos[i]->getWidth() / 16 / 250;
		testSet[pos++] = (videos[i]->getSizeRe() / videos[i]->getSizeOrg());
		testSet[pos++] = (double)frameRate / 60;
		testSet[pos++] = (double)frames / (double)frameRate / 10800;
		double* tempOut = new double[3];
		//should be bit rate
		string outString;
		getline(outputVideoFile, outString);
		stringstream ss;
		ss << outString;
		getline(ss, outString, ',');
		tempOut[0] = stod(outString) / 15000.0;
		getline(ss, outString, ',');
		tempOut[1] = RefFrame::getDoubleFromClass(stoi(outString));
		getline(ss, outString);
		tempOut[2] = BFrame::getDoubleFromClass(stoi(outString));
		add(testSet, tempOut);
		ofstream  trainfile, outfile;
		trainfile.open(TRAINNING_INFO_INPUT_PATH, ios_base::app);
		outfile.open(TRAINNING_INFO_OUTPUT_PATH, ios_base::app);
		for (int x = 0; x < SCALE_DOWN_WIDTH*SCALE_DOWN_HEIGHT + 5; x++){
			trainfile << testSet[x] << ",";
		}
		trainfile << endl;
		for (int x = 0; x < OUTPUT_NEURONS; x++){
			outfile << tempOut[x] << ",";
		}
		outfile << endl;

		trainfile.close();
		outfile.close();
	}
	inputVideoFile.close();
	//toFile();
}*/

//adds video data to array;
void TrainningSet::add(double* data, double* outdata){
	if (size >= MAX_SIZE)
		return;
	out[size] = outdata;
	list[size] = data;
	size++;
}