#include "Neuron.h"
#include <iostream>
#include "networkMath.h"
#include "defs.h"
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

void Neuron::calcGrad(double expected){
	double delta = expected - activation;
	gradient = delta * networkMath::sigmoidPrime(activation);
}
void Neuron::calcGrad(Neuron** in,int size){
	double delta = 0;
	for (int i = 0; i < size; i++){
		delta += weights[i] * in[i]->gradient;
	}
	gradient = delta * networkMath::sigmoidPrime(activation);
}
void Neuron::updateWieghts(Neuron** in, int size){
	for (int i = 0; i < size; i++){
		double oldDelta = delta[i];
		double newDeltaWieght = LEARNING_RATE * in[i]->activation +gradient*MOMENTUM*oldDelta;
		delta[i] = newDeltaWieght;
		weights[i] += newDeltaWieght;
	}
}