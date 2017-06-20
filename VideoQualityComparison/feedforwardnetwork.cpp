#include "FeedForwardNetwork.h"
#include <Windows.h>
using namespace std;
//build and inits first time neural network
FeedForwardNetwork::FeedForwardNetwork(int* cols, int size){
	//builds the neural network
	neuralNetwork = new Neuron*[size];
	networkColsSize = cols;
	depthOfNetwork = size;
	for (int i = 0; i < size; i++){
		neuralNetwork[i] = new Neuron[cols[i]];
	}
	//create biases 1-1 relationship ie each neuron gets one past first row
	for (int r = 1; r <size; r++){
		for (int c = 0; c < cols[r]; c++){
			neuralNetwork[r][c].bias = (double)rand()/RAND_MAX;
		}
	}
	//add weights 1- many ie each neuron has many wieghts realted to the next col excluding last col
	for (int r = 0; r < size-1; r++){
		for (int c = 0; c < cols[r]; c++){
			neuralNetwork[r][c].weights = new double[cols[r + 1]];
			for (int w = 0; w < cols[r + 1]; w++){
				neuralNetwork[r][c].weights[w] = (double)rand() / RAND_MAX;//needs to be randomised maybe modified as better way is present apparently?
			}
		}
	}
}


FeedForwardNetwork::~FeedForwardNetwork()
{
	for (int r = 0; r < depthOfNetwork; r++){
			delete neuralNetwork[r];
	}
}
//
void FeedForwardNetwork::run(){
	while (running){
		//not sure what these rep gona find out later
		int dimI = 0;
		int dimO = 0;


	}
}