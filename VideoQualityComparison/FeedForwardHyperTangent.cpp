#include "FeedForwardHyperTangent.h"
#include <iostream>
#include "networkMath.h"
#include <exception>
#include "Logger.h"
#include <random>
using namespace std;
//build and inits first time neural network
FeedForwardHyperTangent::FeedForwardHyperTangent(int* cols, int size, TrainningSet* train, TrainningSet* check) : NeuralNetwork(cols, size, train, check){
	
}

//
void FeedForwardHyperTangent::train(){
	int itter = 0;
	random_device rd;
	mt19937 e2(rd());
	uniform_real_distribution<> dist(0, VIDEO_TRAIN_SET_SIZE);
	//get input
	//now learn from input
	int batchLength = VIDEO_TRAIN_SET_SIZE/100;



	int testing = 0;
	bool isDone = false;
	while (!isDone){
		testing++;
		itter = 1;
		int startpoint = dist(e2);;
		while (itter-1<batchLength){

			feedForward(trainSet->list[((itter + startpoint) % VIDEO_TRAIN_SET_SIZE)]);
			backpropogation(trainSet->list[((itter + startpoint) % VIDEO_TRAIN_SET_SIZE)], trainSet->out[((itter + startpoint) % VIDEO_TRAIN_SET_SIZE)]);
			itter++;
		}
		if (testing % 1000 == 0){
			int correct = check();
			if (correct >= 5900 || testing == 1000000){
				break;
			}
		}
	}
}

void FeedForwardHyperTangent::feedForward(double* inputData){
	for(int inputLayerNode = 0; inputLayerNode < networkColsSize[0]; inputLayerNode++){
			neuralNetwork[0][inputLayerNode]->activation = inputData[inputLayerNode];
	}
	for (int layer = 1; layer < depthOfNetwork; layer++){
		for (int node = 0; node < networkColsSize[layer]; node++){
			double total = 0;
			for (int i = 0; i < networkColsSize[layer - 1] + 1; i++){
				total += neuralNetwork[layer - 1][i]->activation*neuralNetwork[layer - 1][i]->weights[node];
			}
			neuralNetwork[layer][node]->activation = networkMath::tangentNorm(total);
		}
	}

}

void FeedForwardHyperTangent::backpropogation(double* inputData, double* expectedOut){
	/*  To keep track of errors
		Got this from ADD CREDIT
	
		Lines of code 5
	*/
	double error = 0;

	for (int i = 0; i < networkColsSize[depthOfNetwork - 1]; i++){
		double delta = expectedOut[i] - neuralNetwork[depthOfNetwork - 1][i]->activation;
		error += delta*delta;
	}
	error /= networkColsSize[depthOfNetwork - 1];
	error = sqrt(error);
	
	recent_error = (error) / (recent_error +1.0);
	
	//calc grads
	for (int i = 0; i < networkColsSize[depthOfNetwork - 1]; i++){
		neuralNetwork[depthOfNetwork - 1][i]->gradient = (expectedOut[i] - neuralNetwork[depthOfNetwork - 1][i]->activation)*networkMath::tangentDerivative(neuralNetwork[depthOfNetwork - 1][i]->activation);
	}
	for (int layer = depthOfNetwork - 2; layer > 0; layer--){
		for (int x = 0; x < networkColsSize[layer] + 1; x++){
			for (int i = 0; i < networkColsSize[layer + 1]; i++){
				neuralNetwork[layer][x]->gradient += neuralNetwork[layer][x]->weights[i] * neuralNetwork[layer+1][i]->gradient;
			}
			neuralNetwork[layer][x]->gradient *= networkMath::tangentDerivative(neuralNetwork[layer][x]->activation);
		}
	}
	//update weights
	for (int layer = depthOfNetwork - 1; layer>0; layer--){
		for (int x = 0; x < networkColsSize[layer]; x++){
			for (int i = 0; i < networkColsSize[layer - 1] + 1; i++){
				neuralNetwork[layer-1][i]->prevDelta[x] = neuralNetwork[layer-1][i]->delta[x];
				neuralNetwork[layer-1][i]->delta[x] = LEARNING_RATE * neuralNetwork[layer-1][i]->activation*neuralNetwork[layer][x]->gradient + MOMENTUM*neuralNetwork[layer-1][i]->prevDelta[x];
				neuralNetwork[layer-1][i]->weights[x] += neuralNetwork[layer-1][i]->delta[x];
			}
		}
	}
}
