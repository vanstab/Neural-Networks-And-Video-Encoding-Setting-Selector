#pragma once
#include "Neuron.h"
class Layer
{
public:
	Layer(int);
	~Layer();
	int size(){ return layersize;}
private:
	int layersize;
	Neuron** layer;
};

