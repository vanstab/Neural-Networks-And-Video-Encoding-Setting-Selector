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
TrainningSet::TrainningSet()
{
	size = 0;
	//if (REBUILD_TRAINNING_DATA)
	list = new double*[VIDEO_TRAIN_SET_SIZE];
	out = new double*[VIDEO_TRAIN_SET_SIZE];
	//init();
	load();
	//else load();
}


TrainningSet::~TrainningSet()
{
	for (int i = size - 1; i >= 0; i--)
		delete list[i];
}
//load exsiting file
void TrainningSet::load(){

	ifstream input,output;
	input.open(TRAINNING_INFO_INPUT_PATH, ifstream::in);
	output.open(TRAINNING_INFO_OUTPUT_PATH, ifstream::in);
	//sstream ss;
	string in,o;
	int tempCount = 0;
	for (int i = 0; i < VIDEO_TRAIN_SET_SIZE; i++){
		getline(input, in);
		getline(output, o);
		istringstream ins(in);
		istringstream outs(o);
		string temp;
		double* testSet = new double[(SCALE_DOWN_WIDTH*SCALE_DOWN_HEIGHT + 5)];
		double* tempOut = new double[3];

		while(getline(ins,in, ',')){	
			if (tempCount == SCALE_DOWN_WIDTH*SCALE_DOWN_HEIGHT + 5) break;
			testSet[tempCount] = stod(in);
			tempCount++;
		}
		tempCount = 0;
		while (getline(outs, o, ',')){
			if (tempCount == OUTPUT_NEURONS) break;
			tempOut[tempCount] = stod(o);
			tempCount++;
		}
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
		if (i % (VIDEO_TRAIN_SET_SIZE / 10) == 0) cout << (i / VIDEO_TRAIN_SET_SIZE +1)<< "% done" << endl;

		int pos = 0;
		VideoCapture* cap = new VideoCapture(videos[i]->getPathOrg());
		int frameRate = cap->get(CAP_PROP_FPS);
		int frames = cap->get(CAP_PROP_FRAME_COUNT);
		double temp[SCALE_DOWN_WIDTH][SCALE_DOWN_HEIGHT] = { {0} };
		vidAnalysis->scaleDownAndAnalysis(cap,temp);
		double* testSet = new double[(SCALE_DOWN_WIDTH*SCALE_DOWN_HEIGHT + 5)];
		for (int x = 0; x < SCALE_DOWN_WIDTH; x++){
			for (int y = 0; y < SCALE_DOWN_HEIGHT; y++){
				testSet[pos++] = temp[x][y];
			}
		}
		testSet[pos++] = videos[i]->getHeight()/9/250;
		testSet[pos++] = videos[i]->getWidth()/16/250;
		testSet[pos++] = (videos[i]->getSizeRe() / videos[i]->getSizeOrg());
		testSet[pos++] = (double)frameRate/60;
		testSet[pos++] = (double)frames/(double)frameRate/10800;
		double* tempOut = new double[3];
		//should be bit rate
		string outString;
		getline(outputVideoFile, outString);
		stringstream ss;
		ss << outString;
		getline(ss, outString, ',');
		tempOut[0] =stod(outString)/15000.0;
		getline(ss, outString, ',');
		tempOut[1] = RefFrame::getDoubleFromClass(stoi(outString));
		getline(ss, outString);
		tempOut[2] = BFrame::getDoubleFromClass(stoi(outString));
		add(testSet,tempOut);
	}
	inputVideoFile.close();
	toFile();
}

//adds video data to array;
void TrainningSet::add(double* data, double* outdata){
	if (size >= MAX_ARRAY)
		return;
	out[size] = outdata;
	list[size] = data;
	size++;
	
}
//writes video data to file;
void TrainningSet::toFile(){
	ofstream  trainfile,outfile;
	trainfile.open(TRAINNING_INFO_INPUT_PATH, ios_base::app);
	outfile.open(TRAINNING_INFO_OUTPUT_PATH, ios_base::app);

	for (int i = 0; i < VIDEO_TRAIN_SET_SIZE; i++){
		for (int x = 0; x < SCALE_DOWN_WIDTH*SCALE_DOWN_HEIGHT + 5; x++){
			trainfile << list[i][x] << ",";
		}
		trainfile << endl;
		for (int x = 0; x < OUTPUT_NEURONS; x++){
			outfile << out[i][x] << ",";
		}
		outfile << endl;
	}

	trainfile.close();
	outfile.close();
}