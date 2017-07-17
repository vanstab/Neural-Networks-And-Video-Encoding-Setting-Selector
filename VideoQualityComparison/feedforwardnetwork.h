#pragma once
#include <Windows.h>
#include "Neuron.h"
#include "DoubleTuple.h"
#include "defs.h"
#include "TrainningSet.h"
#include <string>

#include "CleanUpTrain.h"
using namespace std;
class FeedForwardNetwork
{
public:

	FeedForwardNetwork(int*, int, TrainningSet*);
	FeedForwardNetwork(const FeedForwardNetwork&);
	~FeedForwardNetwork();
	void train();
	double* activate(double**,int);
	double** feedForward(int);
	string translateOutputToString(int);
//	double calcError(double*);
	double* gradiantOut();
	double* gradianHidden(int, double*, bool);
	//double actderiv(double);
	int test();
	void backpropogation(double*, double*, DoubleTuple*);
	void feedForward(double*);
	void updateWeights(DoubleTuple*);

	Neuron*** neuralNetwork;
	double* calculateNetworkError();

	TrainningSet* trainSet;

	int batchLength = VIDEO_TRAIN_SET_SIZE;

	int depthOfNetwork;
private:
	bool running = true;
	int* networkColsSize; //might not need
	CleanUpTrain* clean; 
};

