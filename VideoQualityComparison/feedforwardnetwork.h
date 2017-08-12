#pragma once
#include "DoubleTuple.h"
#include "NeuralNetwork.h"
class FeedForwardNetwork : public NeuralNetwork
{
public:

	FeedForwardNetwork(int*, int, TrainningSet*, TrainningSet*,int);
	~FeedForwardNetwork();
	void train();
	double** feedForward(int);
	void backpropogation(double*, double*, DoubleTuple*);
	void feedForward(double*);

private:
};

