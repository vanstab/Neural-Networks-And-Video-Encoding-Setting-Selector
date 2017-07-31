#pragma once
#include "NeuralNetwork.h"
#include "DoubleTuple.h"
class ToBeDisclosed : NeuralNetwork
{
public:
	ToBeDisclosed(int*, int, TrainningSet*);
	~ToBeDisclosed();
	void train();
	void feedForward(double*);

	void backpropogation(double*, double*, DoubleTuple*);
};

