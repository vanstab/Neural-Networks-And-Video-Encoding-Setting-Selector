#include "Neuron.h"
#include <iostream>

Neuron::Neuron()
{
}


Neuron::~Neuron()
{
	if (weights != nullptr)
		delete[]weights;
	if (prevDelta != nullptr)
		delete[]prevDelta;
	if (delta != nullptr)
		delete[]delta;
}
Neuron::Neuron(const Neuron&obj) : bias(obj.bias)
{
	weights = obj.weights;
	delta = obj.delta;
	prevDelta = obj.prevDelta;
	activation = 0;
}

void Neuron::createWeights(int size){
	weights = new double[size];
	delta = new double[size];
	prevDelta = new double[size];
}