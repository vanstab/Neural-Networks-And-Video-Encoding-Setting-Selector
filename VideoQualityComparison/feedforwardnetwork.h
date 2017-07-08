#pragma once
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
	~FeedForwardNetwork();
	void train();
	double* activate(double**,int);
	double** feedForward(int);
	string translateOutputToString(int);
//	double calcError(double*);
	double* gradiantOut();
	double* gradianHidden(int, double*, bool);
	//double actderiv(double);
	void backpropogate(double);

	void backpropogation(double*, double*, DoubleTuple*);
	void feedForward(double*);


	Neuron*** neuralNetwork;
	double* calculateNetworkError();
private:
	bool running = true;
	int* networkColsSize; //might not need
	int depthOfNetwork;
	double totalError = 0;
	int outputs =1;
	TrainningSet* trainSet;
};

