#include "Neuron.h"
#include <iostream>

Neuron::Neuron()
{
}


Neuron::~Neuron()
{
	if (weights != nullptr)
		delete[]weights;
}
Neuron::Neuron(const Neuron&obj) : bias(obj.bias)
{
	weights = obj.weights;
	z = 0;
	activation = 0;
}

void Neuron::createWeights(int size){
	weights = new double[size];
}