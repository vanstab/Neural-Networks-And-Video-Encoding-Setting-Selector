#include "ToBeDisclosed.h"
#include <math.h>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include "networkMath.h"
#include <ctime>
#include <exception>
#include "BFrame.h"
#include "RefFrame.h"
#include "Logger.h"
#include <random>
#include "ValueCheck.h"
using namespace std;
//build and inits first time neural network
ToBeDisclosed::ToBeDisclosed(int* cols, int size, TrainningSet* train, TrainningSet* check,int type) : NeuralNetwork(cols, size, train, check,type){
	
}

//
void ToBeDisclosed::train(){
	int itter = 0;
	random_device rd;
	mt19937 e2(rd());
	uniform_real_distribution<> dist(0, VIDEO_TRAIN_SET_SIZE);
	//get input
	//now learn from input
	int batchLength = VIDEO_TRAIN_SET_SIZE/10;

	DoubleTuple* tuple, *temp;

	tuple = new DoubleTuple[depthOfNetwork];
	temp = new DoubleTuple[depthOfNetwork];
	for (int i = 0; i < depthOfNetwork; i++){
		tuple[i].DoubleTupleInit(i, networkColsSize);
		temp[i].DoubleTupleInit(i, networkColsSize);
	}


	int testing = 0;
	bool isDone = false;
	while (!isDone){
		testing++;
		itter = 0;
		int startpoint = dist(e2);
		for (int i = 0; i < depthOfNetwork; i++){
			tuple[i].clear();
		}

		while (itter < batchLength){
			for (int i = 0; i < depthOfNetwork; i++){
				temp[i].clear();
			}
			try{//might need to add multitreading here to allow fast evals also you be better to have list of tuples above and use each depending on thread!
				backpropogation(trainSet->list[((itter + startpoint) % VIDEO_TRAIN_SET_SIZE)], trainSet->out[((itter + startpoint) % VIDEO_TRAIN_SET_SIZE)], temp);
			}
			catch (exception& e){
				cout << "error:" << e.what() << endl;
			}
			//keep track of changes
			
			itter++;
		}

		if (testing % 1000 == 0){
			cout << recent_error;
			cout << " Testing: " << testing << " Best: " << best;
			if (test() == VIDEO_CHECK_SET_SIZE)isDone = true;
		}
	}
}

void ToBeDisclosed::feedForward(double* inputData){
	for(int inputLayerNode = 0; inputLayerNode < networkColsSize[0]; inputLayerNode++){
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
				total += neuralNetwork[layer - 1][weightToNode]->weights[node] * neuralNetwork[layer - 1][weightToNode]->activation;
			}
			neuralNetwork[layer][node]->activation = networkMath::sigmoid(total + neuralNetwork[layer][node]->bias);
		}
	}
}

void ToBeDisclosed::backpropogation(double* inputData, double* expectedOut, DoubleTuple* tuple){
	feedForward(inputData);
	m_error = 0;
	
	for (int i = 0; i < networkColsSize[depthOfNetwork - 1]; i++){
		double delta = expectedOut[i] - neuralNetwork[depthOfNetwork - 1][i]->activation;
		m_error += delta*delta;
	}
	m_error /= networkColsSize[depthOfNetwork - 1];
	m_error = sqrt(m_error);
	recent_error = (recent_error*recent_error_smooth + m_error) / (recent_error + 1.0);

	for (int i = 0; i < networkColsSize[depthOfNetwork - 1]; i++){
		neuralNetwork[depthOfNetwork - 1][i]->calcGrad(expectedOut[i]);
	}

	for (int i = depthOfNetwork - 2; i > 0; i--){
		for (int x = 0; x < networkColsSize[i]; x++){
			neuralNetwork[i][x]->calcGrad(neuralNetwork[i + 1], networkColsSize[i+1]); //next layer
			}
		}
	for (int i = depthOfNetwork - 2; i>=0; i--){
		for (int x = 0; x < networkColsSize[i]; x++){
			neuralNetwork[i][x]->updateWieghts(neuralNetwork[i + 1], networkColsSize[i+1]);
		}
	}
}
	/*
double* outputDelta = new double[networkColsSize[depthOfNetwork - 1]];
for (int i = 0; i < networkColsSize[depthOfNetwork - 1]; i++){
outputDelta[i] = (networkMath::costDerivative(neuralNetwork[depthOfNetwork - 1][i]->activation, expectedOut[i])*networkMath::sigmoidPrime(neuralNetwork[depthOfNetwork - 1][i]->activation));
}
double** temp = new double*[networkColsSize[depthOfNetwork - 2]];
for (int i = 0; i < networkColsSize[depthOfNetwork - 2]; i++){
temp[i] = neuralNetwork[depthOfNetwork - 2][i]->weights;
}
double** returnVal = new double*[1];
returnVal[0] = new double[networkColsSize[depthOfNetwork - 2]];
networkMath::dotProduct(networkColsSize[depthOfNetwork - 1], 1, networkColsSize[depthOfNetwork - 2], &outputDelta, temp, returnVal);
for (int i = 0; i < networkColsSize[depthOfNetwork - 2]; i++){
returnVal[0][i] = returnVal[0][i] * networkMath::sigmoidPrime(neuralNetwork[depthOfNetwork - 2][i]->activation);
}

for (int x = 0; x < networkColsSize[0]; x++){
for (int y = 0; y < networkColsSize[1]; y++){
neuralNetwork[0][x]->weights[y] -= LEARNING_RATE/VIDEO_TRAIN_SET_SIZE*outputDelta[y];
if (x == 0)
neuralNetwork[1][y]->bias = outputDelta[y];
}
}
for (int x = 0; x < networkColsSize[1]; x++){
for (int y = 0; y < networkColsSize[2]; y++){
neuralNetwork[1][x]->weights[y] += LEARNING_RATE*neuralNetwork[1][x]->activation*outputDelta[y];
}
}*/
