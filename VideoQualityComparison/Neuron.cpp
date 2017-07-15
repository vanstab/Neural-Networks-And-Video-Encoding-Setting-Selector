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

void Neuron::createWeights(int size){
	weights = new double[size];
}