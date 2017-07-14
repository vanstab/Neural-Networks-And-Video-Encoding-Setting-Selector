#include "Neuron.h"
#include <iostream>

Neuron::Neuron()
{
}


Neuron::~Neuron()
{
}

void Neuron::createWeights(int size){
	weights = new double[size];
	delta = new double[size]{0};
	return;
}