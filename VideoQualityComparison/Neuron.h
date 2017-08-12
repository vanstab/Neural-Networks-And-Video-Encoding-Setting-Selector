#pragma once
class Neuron
{
public:
	Neuron(int);
	Neuron(const Neuron&);
	~Neuron();
	double* weights = nullptr;
	double bias = 1.0;
	double activation = 0.0;
	double* delta = nullptr;
	double* prevDelta = nullptr;
	double gradient=0;
	double z;
private:
};

