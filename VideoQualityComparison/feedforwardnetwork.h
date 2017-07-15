#pragma once
#include <Windows.h>
#include "Neuron.h"
#include "DoubleTuple.h"
#include "defs.h"
#include "TrainningSet.h"
#include <string>
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

	void backpropogation(double*, double*, DoubleTuple*);
	void feedForward(double*);
	void updateWeights();

	Neuron*** neuralNetwork;
	double* calculateNetworkError();

	DoubleTuple** threadTuples;
	TrainningSet* trainSet;
private:
	bool running = true;
	int* networkColsSize; //might not need
	int depthOfNetwork;
};

