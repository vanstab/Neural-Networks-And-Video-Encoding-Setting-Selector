
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
int main()
{
	//TrainningSetBuilder* build = new TrainningSetBuilder();
	//build->build();

	int DEPTH_OF_NETWORK =3;
	int* inEnc = new int[DEPTH_OF_NETWORK]{INPUT_SIZE,6,ENCODERMAX};

	int* inProf = new int[DEPTH_OF_NETWORK]{INPUT_SIZE, 4, MAXPROFILE};

	int* inFrame = new int[DEPTH_OF_NETWORK]{INPUT_SIZE, 8, MAXFRAMERATE};

	int* inBit = new int[DEPTH_OF_NETWORK]{INPUT_SIZE, 8,8, 1};
	cout << "Building Training Set..." <<endl;
	//builds the trainning set for each value;
	TrainningSet* trainningEncSet = new TrainningSet(0,0);
	TrainningSet* trainningProfSet = new TrainningSet(0, 1);
	TrainningSet* trainningFrameSet = new TrainningSet(0, 2);
	TrainningSet* trainningBitSet = new TrainningSet(0, 3);
	//TrainningSet*set = new TrainningSet(3, 0);
	//TrainningSet*testin = new TrainningSet(3, 0);
	TrainningSet* checkEncSet = new TrainningSet(1, 0);
	TrainningSet* checkProfSet = new TrainningSet(1, 1);
	TrainningSet* checkFrameSet = new TrainningSet(1, 2);
	TrainningSet* checkBitSet = new TrainningSet(1, 3);

	cout << "Training sets built" << endl;
	cout << "Building Nueral Network...";
	//setup individale networks for each output
	//ToBeDisclosed *feedEnc = new ToBeDisclosed(inEnc, DEPTH_OF_NETWORK, trainningEncSet, checkEncSet, 0);
	//ToBeDisclosed *feedProf = new ToBeDisclosed(inProf, DEPTH_OF_NETWORK, trainningProfSet, checkProfSet, 1);
	//ToBeDisclosed *feedFrame = new ToBeDisclosed(inFrame, DEPTH_OF_NETWORK, trainningFrameSet, checkFrameSet, 2);
	//ToBeDisclosed *feedBit = new ToBeDisclosed(inBit, DEPTH_OF_NETWORK, trainningBitSet, checkBitSet, 3);
	//ToBeDisclosed *test = new ToBeDisclosed(in, DEPTH_OF_NETWORK, set, testin, 1);
	cout << "Nueral Network built" << endl;
	cout << "Training" << endl;
	time_t start, stop;
	try{
		int i = 1;
		double itter = 0;
		double average =0;
		double timetotal = 0;
		while (i <= 8){
			int* inEnc = new int[DEPTH_OF_NETWORK]{INPUT_SIZE, i, ENCODERMAX};
			cout << "Itter: " << itter << " Hidden Nodes: " << i << " ";
			FeedForwardHyperTangent *feedEnc = new FeedForwardHyperTangent(inEnc, DEPTH_OF_NETWORK, trainningEncSet, checkEncSet, 0);
			time(&start);
			feedEnc->train();
			time(&stop);
			timetotal += difftime(stop, start);
			cout << " Time: " << difftime(stop, start) << "s"<<endl;
			int temp = feedEnc->check();
			average += temp;
			if (itter == 9){
				cout << "Layers: " << DEPTH_OF_NETWORK << "  Hidden Nodes: " << i << "  Average Correct: "<< average / 10.0 << "Average Time: " << timetotal/10.0 <<endl;
				i++;
				itter = 0;
				average = 0;
				timetotal = 0;
			}
			else itter++;
		}
		//test->train();
		//trains networks

		//feedBit->train();
		//
		//feedProf->train();
	//	feedFrame->train();
	}
	catch (...){
		cout << "error";
	}


//	

//	feedProf->check();

	//feedFrame->check();

	//feedBit->check();

	Sleep(1000000000);
}