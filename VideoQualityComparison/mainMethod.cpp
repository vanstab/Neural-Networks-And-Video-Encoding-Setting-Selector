
#include <windows.h>
#include "TrainningSet.h"
#include "videoQualityComparison.h"
#include "VideoData.h"
#include "VideoAnalysis.h"
#include "feedforwardnetwork.h"
#include <shellapi.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "BFrame.h"
int main()
{
	int in[4] = { (SCALE_DOWN_WIDTH*SCALE_DOWN_HEIGHT + 5),20,10,3 };
	cout << "Building Training Set..." <<endl;
	TrainningSet* trainningSet = new TrainningSet();
	cout << "Training set built" << endl;
	cout << "Building Nueral Network...";
	FeedForwardNetwork *feed = new FeedForwardNetwork(in,4, trainningSet);
	cout << "Nueral Network built" << endl;
	cout << "Training" << endl;

	try{
		feed->train();
	}
	catch (...){
		cout << "error";
	}
	cout << "checking value...";
	for (int i = VIDEO_TRAIN_SET_SIZE; i < VIDEO_TRAIN_SET_TOTAL; i++){
		feed->feedForward(trainningSet->list[i]);
		cout << i << ":" << feed->neuralNetwork[2][0]->activation << "," << feed->neuralNetwork[2][1]->activation << "," << feed->neuralNetwork[2][2]->activation << endl;
		cout << i << ":" << trainningSet->out[i][0] << "," << trainningSet->out[i][1] << "," << trainningSet->out[i][2] << endl;
	}
	
	Sleep(1000000000);
}