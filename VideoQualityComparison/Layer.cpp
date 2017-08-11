#include "Layer.h"


Layer::Layer(int size) :layersize(size){
	layer = new Neuron*[layersize];
}


Layer::~Layer()
{
}
