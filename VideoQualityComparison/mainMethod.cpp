
#include <windows.h>
#include "VideoQualityComparison.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <tchar.h>
using namespace std;
int main()
{
	VideoQualityComparison test;
	int i = 1;
	for (int i = 1; i <= 64; i++){
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

		}
		double test1 = resize.QuadPart;
		double test2 = size.QuadPart;
		sfileSize << (test1 / test2) << endl;

		cout << "Currently Analysing: pair" << i << "org to pair" << i << "re1..." << endl;
		test.logAnalyseInfo(to_string(i)); 
		test.compareQuality(pathOrg, pathre);
		test.logAnalyseInfo(sfileSize.str());
		}
	Sleep(10000);
}