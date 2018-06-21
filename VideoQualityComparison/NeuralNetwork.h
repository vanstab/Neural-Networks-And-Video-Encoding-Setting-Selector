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
	NeuralNetwork(int*, int, TrainningSet*, TrainningSet*);
	~NeuralNetwork();

	int test(int);
	virtual void train() = 0;
	virtual void feedForward(double*) = 0;
	TrainningSet* trainSet;
	TrainningSet* checkSet;
	int batchLength = VIDEO_TRAIN_SET_SIZE;
	int check();
	Neuron*** neuralNetwork;
	int depthOfNetwork;
protected:
	int best = 0;
	int trainbest = 0;
	bool running = true;
	int* networkColsSize; //might not need
};

