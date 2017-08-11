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
	int batchLength = VIDEO_TRAIN_SET_SIZE/100;



	int testing = 0;
	bool isDone = false;
	while (!isDone){
		testing++;
		itter = 1;
		int startpoint = dist(e2);;
		while (itter-1<batchLength){

			feedForward(trainSet->list[((itter + startpoint) % VIDEO_TRAIN_SET_SIZE)]);
			backpropogation(trainSet->list[((itter + startpoint) % VIDEO_TRAIN_SET_SIZE)], trainSet->out[((itter + startpoint) % VIDEO_TRAIN_SET_SIZE)], NULL);

			itter++;
		}
		if (testing % 100000 == 0){
			cout << "Itter: " << testing;
			test(((itter + startpoint) % VIDEO_TRAIN_SET_SIZE));
			cout << "         Neterror: " << recent_error << endl;
			if (recent_error < 0.005 && networkColsSize[depthOfNetwork-1] != 1) break;
			else if (recent_error < 0.0000005)break; //needs to be smaller as the results are small decimals for bitrate
			//cout << endl;
		}
	}
}

void ToBeDisclosed::feedForward(double* inputData){
	for(int inputLayerNode = 0; inputLayerNode < networkColsSize[0]; inputLayerNode++){
			neuralNetwork[0][inputLayerNode]->setoutput(inputData[inputLayerNode]);
	}
	for (int layer = 1; layer < depthOfNetwork; layer++){
		for (int node = 0; node < networkColsSize[layer]; node++){
			neuralNetwork[layer][node]->feedForward(neuralNetwork[layer-1],networkColsSize[layer-1]);
		}
	}

}

void ToBeDisclosed::backpropogation(double* inputData, double* expectedOut, DoubleTuple* tuple){
	
	m_error = 0;

	for (int i = 0; i < networkColsSize[depthOfNetwork - 1]; i++){
		double delta = expectedOut[i] - neuralNetwork[depthOfNetwork - 1][i]->activation;
		m_error += delta*delta;
	}
	m_error /= networkColsSize[depthOfNetwork - 1];
	m_error = sqrt(m_error);
	recent_error = (recent_error*recent_error_smooth + m_error) / (recent_error + 1.0);
	//calc grads
	for (int i = 0; i < networkColsSize[depthOfNetwork - 1]; i++){
		neuralNetwork[depthOfNetwork - 1][i]->calcGrad(expectedOut[i]);
	}

	for (int i = depthOfNetwork - 2; i > 0; i--){
		for (int x = 0; x < networkColsSize[i]+1; x++){
			neuralNetwork[i][x]->calcGrad(neuralNetwork[i + 1], networkColsSize[i+1]); //next layer
		}
	}

	//update weughts
	for (int i = depthOfNetwork - 1; i>0; i--){
		for (int x = 0; x < networkColsSize[i]+1; x++){
			neuralNetwork[i][x]->updateWieghts(neuralNetwork[i-1], networkColsSize[i-1]);
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
