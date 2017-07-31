#pragma once
#include "DoubleTuple.h"
#include "NeuralNetwork.h"
class FeedForwardNetwork : public NeuralNetwork
{
public:

	FeedForwardNetwork(int*, int, TrainningSet*);
	//FeedForwardNetwork(const FeedForwardNetwork&);
	~FeedForwardNetwork();
	void train();
	double* activate(double**,int);
	double** feedForward(int);
	double* gradiantOut();
	double* gradianHidden(int, double*, bool);
	void backpropogation(double*, double*, DoubleTuple*);
	void feedForward(double*);
	void updateWeights(DoubleTuple*);

	double* calculateNetworkError();

private:

};

