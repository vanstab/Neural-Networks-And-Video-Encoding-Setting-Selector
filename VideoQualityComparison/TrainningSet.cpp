#include "TrainningSet.h"
#include <windows.h>
#include "VideoQualityComparison.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <tchar.h>
using namespace std;
TrainningSet::TrainningSet()
{
	size = 0;
	if (REBUILD_TRAINNING_DATA)
		init();
	else load();
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
		add(data);
	}
}
//create new file
void TrainningSet::init(){
	VideoQualityComparison test;

	for (int i = 1; i <= MAX_ARRAY; i++){
		VideoData* data = new VideoData();
		stringstream spath, sfileSize;
		spath << "D:/VideoTestDataSet/Comparision/pair" << i << "org.*";
		String pathOrg = spath.str();
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = FindFirstFile(pathOrg.c_str(), &FindFileData);
		LARGE_INTEGER size, resize;
		if (hFind == INVALID_HANDLE_VALUE)
			Logger::log("File not found", Logger::warning);
		else{
			//gets file size
			size.LowPart = FindFileData.nFileSizeLow;
			size.HighPart = FindFileData.nFileSizeHigh;
			if (size.QuadPart % 1024 != 0)
				sfileSize << (size.QuadPart / 1024 + 1) << " ";
			else sfileSize << size.QuadPart / 1024 << " ";

			//get file path
			spath.str("");
			spath << "D:/VideoTestDataSet/Comparision/" << FindFileData.cFileName;
			pathOrg = spath.str();
			data->setPathOrg(pathOrg);
		}

		spath.str("");
		spath << "D:/VideoTestDataSet/Comparision/pair" << i << "re1.mp4";
		String pathre = spath.str();
		hFind = FindFirstFile(pathre.c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
			Logger::log("File not found", Logger::warning);
		else{
			//get file size;
			resize.LowPart = FindFileData.nFileSizeLow;
			resize.HighPart = FindFileData.nFileSizeHigh;
			if (resize.QuadPart % 1024 != 0)
				sfileSize << (resize.QuadPart / 1024 + 1) << " ";
			else sfileSize << resize.QuadPart / 1024 << " ";

			//get file path
			spath.str("");
			spath << "D:/VideoTestDataSet/Comparision/" << FindFileData.cFileName;
			pathre = spath.str();
			data->setPathRe(pathre);

		}
		double reSize = resize.QuadPart;
		double orgSize = size.QuadPart;
		sfileSize << (reSize / orgSize) << endl;
		data->setSizeOrg(orgSize);
		data->setSizeRe(reSize);
		cout << "Currently Analysing: pair" << i << "org to pair" << i << "re1..." << endl;
		test.logAnalyseInfo(to_string(i));
		test.compareQuality(data);
		test.logAnalyseInfo(sfileSize.str());
		add(data);
	}
	toFile();
}

//adds video data to array;
void TrainningSet::add(VideoData* data){
	if (size >= MAX_ARRAY)
		return;
	list[size++] = data;
}
//writes video data to file;
void TrainningSet::toFile(){
	ofstream  trainfile;
	trainfile.open(TRAINNING_INFO_PATH, ios_base::app);
	for (int i = 0; i < MAX_ARRAY; i++){
		trainfile << list[i]->toString();
	}
	trainfile.close();
}