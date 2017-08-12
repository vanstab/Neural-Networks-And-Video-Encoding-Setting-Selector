#include "FeedForwardSigmoid.h"
#include <iostream>
#include <exception>
#include <random>
using namespace std;

//build and inits first time neural network
FeedForwardSigmoid::FeedForwardSigmoid(int* cols, int size, TrainningSet* train, TrainningSet* check, int type) : NeuralNetwork(cols, size, train, check, type){
	
}
/*
FeedForwardNetwork::FeedForwardNetwork(const FeedForwardNetwork& obj) :depthOfNetwork(obj.depthOfNetwork), networkColsSize(obj.networkColsSize), trainSet(obj.trainSet), batchLength(obj.batchLength){
	neuralNetwork = new Neuron**[depthOfNetwork];
	for (int r = 0; r <depthOfNetwork; r++){//for the depth not including first input layer
		neuralNetwork[r] = new Neuron*[networkColsSize[r]];//create an aray of node pointers
		for (int c = 0; c < networkColsSize[r]; c++){//create nodes and add biases to each of the nodes in layer
			neuralNetwork[r][c] = new Neuron(*obj.neuralNetwork[r][c]);
		}
	}
}*/

FeedForwardSigmoid::~FeedForwardSigmoid()
{
}
//
void FeedForwardSigmoid::train(){
	//init class values
	int itter = 0;
	random_device rd;
	mt19937 e2(rd());
	uniform_real_distribution<> dist(0, VIDEO_TRAIN_SET_SIZE);

	int testing = 0;
	bool isDone = false;
	while (!isDone){
	
		//reset for batch;
		testing++;
		itter = 0;
		int startpoint = dist(e2);
		//non-thread tuple used to apply updates
		while (itter < VIDEO_TRAIN_SET_SIZE/100){
			backpropogation(trainSet->list[(startpoint + itter) % VIDEO_TRAIN_SET_SIZE], trainSet->out[(startpoint + itter) % VIDEO_TRAIN_SET_SIZE]);
			itter++;
		}
		if (testing % 1== 0){
			cout << " Testing: " << testing << " Best: " << best;
			//test();

			Sleep(500);
		//	if (test() == VIDEO_CHECK_SET_SIZE)isDone = true;
		}
	}
}









void FeedForwardSigmoid::feedForward(double* inputData){
	for (int inputLayerNode = 0; inputLayerNode < networkColsSize[0]; inputLayerNode++){
		try{
			neuralNetwork[0][inputLayerNode]->activation = inputData[inputLayerNode];
		}
		catch (exception& e){
			cout << " feedforward in " << inputLayerNode << e.what() << endl;
			throw e;
		}
	}
	for (int layer = 1; layer < depthOfNetwork; layer++){
		for (int node = 0; node < networkColsSize[layer]; node++){
			double total = 0;
			for (int weightToNode = 0; weightToNode < networkColsSize[layer - 1]; weightToNode++){
				total +=  neuralNetwork[layer - 1][weightToNode]->weights[node] * neuralNetwork[layer - 1][weightToNode]->activation;
			}
			neuralNetwork[layer][node]->activation = networkMath::sigmoid(total+ neuralNetwork[layer][node]->bias);
		}
	}
}
void FeedForwardSigmoid::backpropogation(double* inputData, double* expectedOut){
	feedForward(inputData);
	for (int i = 0; i < networkColsSize[depthOfNetwork - 1]; i++)
		neuralNetwork[depthOfNetwork - 1][i]->gradient = (expectedOut[i] - neuralNetwork[depthOfNetwork - 1][i]->activation)*(1 - neuralNetwork[depthOfNetwork - 1][i]->activation)*(1 + neuralNetwork[depthOfNetwork - 1][i]->activation);

	for (int curLayer = depthOfNetwork - 2; curLayer >= 0; curLayer--){
		for (int node = 0; node < networkColsSize[curLayer]; node++){
			for (int w = 0; w < networkColsSize[curLayer + 1]; w++){
				neuralNetwork[curLayer][node]->gradient += neuralNetwork[curLayer][node]->weights[w] * neuralNetwork[curLayer + 1][w]->gradient;
			}
			neuralNetwork[curLayer][node]->gradient *= (1+neuralNetwork[curLayer][node]->activation)*(1 - neuralNetwork[curLayer][node]->activation);
		}
	}
	for (int input = 0; input < networkColsSize[0]; input++){
		for (int nextLayer = 0; nextLayer < networkColsSize[1]; nextLayer++){
			neuralNetwork[0][input]->delta[nextLayer] = LEARNING_RATE*neuralNetwork[1][nextLayer]->gradient*neuralNetwork[0][input]->activation*networkMath::sigmoidPrime(neuralNetwork[0][input]->activation);
			neuralNetwork[0][input]->weights[nextLayer] += neuralNetwork[0][input]->prevDelta[nextLayer] * MOMENTUM + neuralNetwork[0][input]->delta[nextLayer];
			neuralNetwork[0][input]->prevDelta[nextLayer] = neuralNetwork[0][input]->delta[nextLayer];
		}
		neuralNetwork[0][input]->bias += MOMENTUM *neuralNetwork[0][input]->gradient;
	}
	for (int layer = 1; layer < depthOfNetwork - 1; layer++){
		for (int node = 0; node < networkColsSize[layer]; node++){
			for (int nextLayer = 0; nextLayer < networkColsSize[layer + 1]; nextLayer++){
				neuralNetwork[layer][node]->delta[nextLayer] = LEARNING_RATE*neuralNetwork[layer + 1][nextLayer]->gradient*neuralNetwork[layer][node]->activation*networkMath::sigmoidPrime(neuralNetwork[layer][node]->activation);
				neuralNetwork[layer][node]->weights[nextLayer] += neuralNetwork[layer][node]->prevDelta[nextLayer] * MOMENTUM + neuralNetwork[layer][node]->delta[nextLayer];
				neuralNetwork[layer][node]->prevDelta[nextLayer] = neuralNetwork[layer][node]->delta[nextLayer];
			}
			neuralNetwork[layer][node]->bias += MOMENTUM *neuralNetwork[layer][node]->gradient;
		}
	}

	for (int output = 0; output < networkColsSize[depthOfNetwork- 1]; output++){
		neuralNetwork[depthOfNetwork - 1][output]->bias = LEARNING_RATE *neuralNetwork[depthOfNetwork - 1][output]->gradient;
	}
	/*double** delta = new double*[depthOfNetwork];
	for (int i = 0; i < depthOfNetwork; i++){
		delta[i] = new double[networkColsSize[i]];
	}
	for (int i = 0; i <  networkColsSize[depthOfNetwork- 1]; i++){
		if (EncoderLevel::getClassFromDouble(neuralNetwork[depthOfNetwork - 1][i]->activation) == EncoderLevel::getClassFromDouble(expectedOut[i])) delta[depthOfNetwork - 1][i] = 0;
		else delta[depthOfNetwork-1][i] = (expectedOut[i] - neuralNetwork[depthOfNetwork - 1][i]->activation)*networkMath::sigmoidPrime(neuralNetwork[depthOfNetwork - 1][i]->activation);
	}
	for (int curLayer = depthOfNetwork - 2; curLayer > 0; curLayer--){
		for (int node = 0; node < networkColsSize[curLayer]; node++){
			double error = 0;
			for (int w = 0; w < networkColsSize[curLayer + 1]; w++){
				error += neuralNetwork[curLayer][node]->weights[w] * neuralNetwork[curLayer + 1][w]->delta[0];
			}
			delta[curLayer][node] = error * networkMath::sigmoidPrime(neuralNetwork[curLayer][node]->activation);
		}
	}
	for (int curLayer = depthOfNetwork - 2; curLayer >= 0; curLayer--){
		for (int node = 0; node < networkColsSize[curLayer]; node++){
			for (int x = 0; x < networkColsSize[curLayer + 1]; x++){
				neuralNetwork[curLayer][node]->delta[x] = neuralNetwork[curLayer][node]->activation*delta[curLayer + 1][x];
			}
		}
	}

	for (int i = 0; i < depthOfNetwork-1; i++){
		for (int node = 0; node < networkColsSize[i]; node++){
			for (int r = 0; r < networkColsSize[i + 1]; r++){
				neuralNetwork[i][node]->weights[r] += LEARNING_RATE*neuralNetwork[i][node]->delta[r];//*neuralNetwork[i][node]->activation;
			}
		}
	}




	for (int i = 0; i < depthOfNetwork; i++){
		for (int node = 0; node < networkColsSize[i]; node++){
			neuralNetwork[i][node]->prevDelta = neuralNetwork[i][node]->delta;
		}
	}
	for (int i = 0; i < depthOfNetwork; i++){
		delete delta[i];
	}
	delete delta;*/
	//
	for (int i = 0; i <  networkColsSize[depthOfNetwork- 1]; i++){
		neuralNetwork[depthOfNetwork - 1][i]->gradient = (expectedOut[i] - neuralNetwork[depthOfNetwork - 1][i]->activation)*(1 - neuralNetwork[depthOfNetwork - 1][i]->activation)*(1 + neuralNetwork[depthOfNetwork - 1][i]->activation);
	}

	for (int curLayer = depthOfNetwork - 2; curLayer >= 0; curLayer--){
		for (int node = 0; node < networkColsSize[curLayer]; node++){
			for (int w = 0; w < networkColsSize[curLayer + 1]; w++){
				neuralNetwork[curLayer][node]->gradient += neuralNetwork[curLayer][node]->weights[w] * neuralNetwork[curLayer + 1][w]->gradient;
			}
			neuralNetwork[curLayer][node]->gradient *= neuralNetwork[curLayer][node]->activation*(1 - neuralNetwork[curLayer][node]->activation);
		}
	}
	for (int input = 0; input < networkColsSize[0]; input++){
		for (int nextLayer = 0; nextLayer < networkColsSize[1]; nextLayer++){
			neuralNetwork[0][input]->delta[nextLayer] = LEARNING_RATE*neuralNetwork[1][nextLayer]->gradient*neuralNetwork[0][input]->activation*networkMath::sigmoidPrime(neuralNetwork[0][input]->activation + neuralNetwork[0][input]->bias);
			neuralNetwork[0][input]->weights[nextLayer] += neuralNetwork[0][input]->prevDelta[nextLayer] * LEARNING_RATE + neuralNetwork[0][input]->delta[nextLayer];
			neuralNetwork[0][input]->prevDelta[nextLayer] = neuralNetwork[0][input]->delta[nextLayer];
		}
	}
	for (int layer = 1; layer < depthOfNetwork - 1; layer++){
		for (int node = 0; node < networkColsSize[layer]; node++){
			for (int nextLayer = 0; nextLayer < networkColsSize[layer + 1]; nextLayer++){
				neuralNetwork[layer][node]->delta[nextLayer] = LEARNING_RATE*neuralNetwork[layer + 1][nextLayer]->gradient*neuralNetwork[layer][node]->activation*networkMath::sigmoidPrime(neuralNetwork[layer][node]->activation + neuralNetwork[layer][node]->bias);
				neuralNetwork[layer][node]->weights[nextLayer] += neuralNetwork[layer][node]->prevDelta[nextLayer] * LEARNING_RATE + neuralNetwork[layer][node]->delta[nextLayer];
				neuralNetwork[layer][node]->prevDelta[nextLayer] = neuralNetwork[layer][node]->delta[nextLayer];
			}
			neuralNetwork[layer][node]->bias = LEARNING_RATE *neuralNetwork[layer][node]->gradient;
		}
	}

	for (int output = 0; output <  networkColsSize[depthOfNetwork- 1]; output++){
		neuralNetwork[depthOfNetwork - 1][output]->bias = LEARNING_RATE *neuralNetwork[depthOfNetwork - 1][output]->gradient;
	}	
}

