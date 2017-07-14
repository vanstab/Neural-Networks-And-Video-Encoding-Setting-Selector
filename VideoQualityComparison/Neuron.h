#pragma once
class Neuron
{
public:
	void createWeights(int);
	Neuron();
	~Neuron();
	double* weights;
	double* delta;
	double bias = 1.0;
	double activation;
	double wgain =1.0;
	double z = 0;
};

