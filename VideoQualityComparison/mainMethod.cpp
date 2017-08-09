
#include <windows.h>
#include "TrainningSet.h"
#include "videoQualityComparison.h"
#include "VideoData.h"
#include "VideoAnalysis.h"
#include "feedforwardnetwork.h"
#include "ToBeDisclosed.h"
#include "TrainningSetBuilder.h"
#include <shellapi.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "BFrame.h"
#include "Profile.h"
int main()
{
	//TrainningSetBuilder* build = new TrainningSetBuilder();
	//build->build();
	int DEPTH_OF_NETWORK = 3;
	int* in = new int[DEPTH_OF_NETWORK]{ INPUT_SIZE, 6, OUTPUT_NEURONS };
	cout << "Building Training Set..." <<endl;
	//builds the trainning set for each value;
	TrainningSet* trainningEncSet = new TrainningSet(0,0);
	TrainningSet* trainningProfSet = new TrainningSet(0, 1);
	TrainningSet* trainningFrameSet = new TrainningSet(0, 2);
	TrainningSet* trainningBitSet = new TrainningSet(0, 3);

	TrainningSet* checkEncSet = new TrainningSet(1, 0);
	TrainningSet* checkProfSet = new TrainningSet(1, 1);
	TrainningSet* checkFrameSet = new TrainningSet(1, 2);
	TrainningSet* checkBitSet = new TrainningSet(1, 3);

	cout << "Training sets built" << endl;
	cout << "Building Nueral Network...";
	//setup individale networks for each output
	FeedForwardNetwork *feedEnc = new FeedForwardNetwork(in, DEPTH_OF_NETWORK, trainningEncSet, checkEncSet,0);
	FeedForwardNetwork *feedProf = new FeedForwardNetwork(in, DEPTH_OF_NETWORK, trainningProfSet, checkProfSet,1);
	FeedForwardNetwork *feedFrame = new FeedForwardNetwork(in, DEPTH_OF_NETWORK, trainningFrameSet, checkFrameSet,2);
	FeedForwardNetwork *feedBit = new FeedForwardNetwork(in, DEPTH_OF_NETWORK, trainningBitSet, checkBitSet,3);

	cout << "Nueral Network built" << endl;
	cout << "Training" << endl;
	try{
		//trains networks
		//feedEnc->train();
		//feedProf->train();
		feedFrame->train();
		//feedBit->train();
	}
	catch (...){
		cout << "error";
	}
	cout << "checking value...";
	for (int i = 0; i < VIDEO_CHECK_SET_SIZE; i++){
	//	feedEnc->feedForward(checkEncSet->list[i]);
//		cout << i << ":" << feed->neuralNetwork[DEPTH_OF_NETWORK - 1][0]->activation;//<< "," << feed->neuralNetwork[2][1]->activation << "," << feed->neuralNetwork[2][2]->activation << endl;
//		cout << " Expected:" << checkSet->out[i][0] <<endl; //<< "," << checkSet->out[i][1] << "," << checkSet->out[i][2] << endl;
	}
	Sleep(1000000000);
}