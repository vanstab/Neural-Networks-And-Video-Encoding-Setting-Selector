#include "Neuron.h"
#include <iostream>
#include "networkMath.h"
#include "defs.h"

#include <Windows.h>
#include <random>

using namespace std;
Neuron::Neuron(int size, int index) :numOut(size) ,index(index)
{
	random_device rd;
	mt19937 e2(rd());
	uniform_real_distribution<> dist(-1, 1);
	weights = new double[size];
	delta = new double[size];
	prevDelta = new double[size];
	for (int i = 0; i < size; i++){
		weights[i] = dist(e2);
		delta[i] = 0;
		prevDelta[i] = 0;
	}
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


void Neuron::calcGrad(double expected){
	double delta = expected - activation;
	gradient = delta * networkMath::tangentDer(activation);
}

void Neuron::calcGrad(Neuron** in,int size){
	double temp = 0;
	for (int i = 0; i < size; i++){
		temp += weights[i] * in[i]->gradient;
	}
	gradient = temp * networkMath::tangentDer(activation);
}

void Neuron::updateWieghts(Neuron** input, int& size){

	for (int i = 0; i < size; i++){
		
		double oldDelta = input[i]->delta[index];

		double newDeltaWieght = LEARNING_RATE * input[i]->activation*gradient + MOMENTUM*oldDelta;

		input[i]->delta[index] = newDeltaWieght;
		input[i]->weights[index] += newDeltaWieght;
	}
}

void Neuron::feedForward(Neuron** in,int& size){
	double sum = 0.0;
	for (int i = 0; i < size+1; i++){

		sum += in[i]->activation*in[i]->weights[index];
	}
	activation = networkMath::tangentNorm(sum);
}