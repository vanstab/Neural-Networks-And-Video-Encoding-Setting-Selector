#pragma once

#include <Windows.h>
#include "networkMath.h"
#include "Neuron.h"
#include "defs.h"
#include "TrainningSet.h"
#include <string>
#include "Logger.h"
class NeuralNetwork
{
public:
	NeuralNetwork(int*, int, TrainningSet*);
	~NeuralNetwork();

	int test();
	virtual void train() = 0;
	virtual void feedForward(double*) = 0;
	TrainningSet* trainSet;

	int batchLength = VIDEO_TRAIN_SET_SIZE;

	Neuron*** neuralNetwork;
	int depthOfNetwork;
protected:
	int best = 0;
	bool running = true;
	int* networkColsSize; //might not need
};

