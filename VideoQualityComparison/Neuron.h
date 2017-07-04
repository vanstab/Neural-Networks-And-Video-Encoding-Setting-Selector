#pragma once
class Neuron
{
public:
	void createWeights(int);
	Neuron();
	~Neuron();
	double* weights;
	double bias = 1.0;
	double activation;
	//also know as net
	double z;
};

