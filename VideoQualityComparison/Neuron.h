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
	double gradient=0;
	double z;
	void calcGrad(double);
	void updateWieghts(Neuron**,int);
	void calcGrad(Neuron**,int);
};

