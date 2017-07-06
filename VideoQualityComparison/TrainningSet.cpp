#include "TrainningSet.h"
#include <windows.h>
#include "VideoQualityComparison.h"
#include "VideoAnalysis.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <tchar.h>
#include "networkMath.h"
using namespace std;
TrainningSet::TrainningSet()
{
	size = 0;
	//if (REBUILD_TRAINNING_DATA)
	list = new double*[VIDEO_TRAIN_SET_SIZE];
	out = new double*[VIDEO_TRAIN_SET_SIZE];
	init();
	//else load();
}


TrainningSet::~TrainningSet()
{
	for (int i = size - 1; i >= 0; i--)
		delete list[i];
}
//load exsiting file
void TrainningSet::load(){

	ifstream input;
	input.open(TRAINNING_INFO_PATH, ifstream::in);
	string s;
	while (getline(input, s)){
		VideoData *data = new VideoData(s);
		//add(data);
	}
}
//create new file
void TrainningSet::init(){
	VideoQualityComparison test;
	VideoData* videos[VIDEO_TRAIN_SET_SIZE];	
	int videoCount = 0;
	ifstream inputVideoFile, outputVideoFile;
	inputVideoFile.open(TRAINNING_INFO_PATH, ifstream::in);
	outputVideoFile.open(OUTPUT_TRAINING_INFO_PATH, ifstream::in);
	string s;
	//loads data of previous videos
	while (videoCount < VIDEO_TRAIN_SET_SIZE){
		getline(inputVideoFile, s);
		videos[videoCount] = new VideoData(s);
		videoCount++;
	}
	VideoAnalysis* vidAnalysis = new VideoAnalysis();
	for (int i = 0; i < VIDEO_TRAIN_SET_SIZE; i++){
		int pos = 0;
		VideoCapture* cap = new VideoCapture(videos[i]->getPathOrg());
		double temp[SCALE_DOWN_WIDTH][SCALE_DOWN_HEIGHT] = { {} };
		vidAnalysis->scaleDownAndAnalysis(cap,temp);
		double* testSet = new double[(SCALE_DOWN_WIDTH*SCALE_DOWN_HEIGHT + 3)];
		for (int x = 0; x < SCALE_DOWN_WIDTH; x++){
			for (int y = 0; y < SCALE_DOWN_HEIGHT; y++){
				testSet[pos++] = temp[x][y];
			}
		}
		testSet[pos++] = videos[i]->getHeight();
		testSet[pos++] = videos[i]->getWidth();
		testSet[pos++] = (videos[i]->getSizeRe() / videos[i]->getSizeOrg());
		double tempOut[3];
		//should be bit rate
		string outString;
		getline(outputVideoFile, outString);
		stringstream ss;
		ss << outString;
		getline(ss, outString, ',');
		tempOut[0] = networkMath::sigmoid(stod(outString));
		getline(ss, outString, ',');
		tempOut[1] = networkMath::sigmoid(stod(outString));
		getline(ss, outString);
		tempOut[2] = networkMath::sigmoid(stod(outString));
		add(testSet,tempOut);
	}
	inputVideoFile.close();
}

//adds video data to array;
void TrainningSet::add(double* data, double* outdata){
	if (size >= MAX_ARRAY)
		return;
	out[size] = outdata;
	list[size++] = data;
	
}
//writes video data to file;
void TrainningSet::toFile(){
	ofstream  trainfile;
	trainfile.open(TRAINNING_INFO_PATH, ios_base::app);
	for (int i = 0; i < MAX_ARRAY; i++){
		//trainfile << list[i]->toString();
	}
	trainfile.close();
}