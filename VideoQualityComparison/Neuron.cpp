#include "Neuron.h"
#include <iostream>
#include "networkMath.h"
#include "defs.h"

#include <Windows.h>
#include <random>

using namespace std;
Neuron::Neuron(int size)
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

