#include "NeuralNetwork.h"

#include <iostream>
#include <random>
#include "ValueCheck.h"
using namespace std;
NeuralNetwork::NeuralNetwork(int* cols, int size, TrainningSet* train)
{
	//builds the neural network
	trainSet = train;
	random_device rd;
	mt19937 e2(rd());
	uniform_real_distribution<> dist(-1, 1);
	neuralNetwork = new Neuron**[size];
	networkColsSize = cols;
	depthOfNetwork = size;
	//create biases 1-1 relationship ie each neuron gets one past first row
	for (int r = 0; r <size; r++){//for the depth not including first input layer
		neuralNetwork[r] = new Neuron*[cols[r]];//create an aray of node pointers
		for (int c = 0; c < cols[r]; c++){//create nodes and add biases to each of the nodes in layer
			neuralNetwork[r][c] = new Neuron();
			if (r >= 1){
				neuralNetwork[r][c]->bias = dist(e2);
			}
		}
	}
	//add weights 1- many ie each neuron has many wieghts realted to the next col excluding last col
	for (int r = 0; r < size - 1; r++){//for the depth of the net -1
		for (int c = 0; c < cols[r]; c++){//for each node in that layer add a weight array
			neuralNetwork[r][c]->createWeights(cols[r + 1]);
			for (int w = 0; w < cols[r + 1]; w++){//for each element in the array and the weight to the next layers node
				neuralNetwork[r][c]->weights[w] = dist(e2) / (cols[r] * cols[r + 1]);//needs to be randomised maybe modified as better way is present apparently?	
			}
		}
	}
}


NeuralNetwork::~NeuralNetwork()
{
	for (int r = 0; r < depthOfNetwork - 1; r++){

		for (int c = 0; c < networkColsSize[r]; c++){
			delete neuralNetwork[r][c];
		}
		delete[] neuralNetwork[r];
	}
	delete[] neuralNetwork;
}

int NeuralNetwork::test(){
	int correct = 0, bwrong = 0, rwrong = 0, bitwrong = 0;
	for (int runCheck = VIDEO_TRAIN_SET_SIZE; runCheck < VIDEO_TRAIN_SET_TOTAL; runCheck++){
		feedForward(trainSet->list[runCheck]);
		ValueCheck::check(neuralNetwork[depthOfNetwork - 1], trainSet->out[runCheck], correct, bitwrong, rwrong, bwrong);
	}
	cout << correct << " " << bitwrong << " " << rwrong << " " << bwrong << endl;
	if (correct > best){
		best = correct;
	}
	return correct;
}