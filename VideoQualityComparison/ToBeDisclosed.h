 #pragma once
#include "NeuralNetwork.h"
#include "DoubleTuple.h"
class ToBeDisclosed : public NeuralNetwork
{
public:
	ToBeDisclosed(int*, int, TrainningSet*, TrainningSet*,int);
	~ToBeDisclosed();
	void train();
	void feedForward(double*);

	void backpropogation(double*, double*, DoubleTuple*);

	void updateWeights(DoubleTuple*);

};

