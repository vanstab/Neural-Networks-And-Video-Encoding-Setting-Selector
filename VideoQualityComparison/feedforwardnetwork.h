#pragma once
#include "Neuron.h"
class FeedForwardNetwork
{
public:
	FeedForwardNetwork(int*, int size);
	~FeedForwardNetwork();
	void run();
private:
	bool running = true;
	Neuron** neuralNetwork;
	int* networkColsSize; //might not need
	int depthOfNetwork;
	

};

