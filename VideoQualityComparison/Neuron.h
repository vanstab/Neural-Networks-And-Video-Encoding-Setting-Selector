#pragma once
class Neuron
{
public:
	void createWeights(int);
	Neuron();
	Neuron(const Neuron&);
	~Neuron();
	double* weights = nullptr;
	double bias = 1.0;
	double activation = 0.0;
	double* delta = nullptr;
	double* prevDelta = nullptr;
	double gradient;
};

