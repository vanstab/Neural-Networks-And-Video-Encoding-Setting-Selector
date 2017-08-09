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
			try{
				for (int i = 0; i < depthOfNetwork; i++){
					tuple[i].addTuples(&temp[i]);
				}
			}
			catch (exception& e){
				cout << "error:" << e.what() << endl;
			}
			itter++;
		}
		//apply
		for (int i = 0; i < depthOfNetwork - 1; i++){

			for (int d = 0; d < networkColsSize[i + 1]; d++){
				neuralNetwork[i + 1][d]->bias = neuralNetwork[i + 1][d]->bias - (LEARNING_RATE / batchLength)*tuple[i].bias[d][0];
			}
			for (int d = 0; d < networkColsSize[i + 1]; d++){
				for (int y = 0; y < networkColsSize[i]; y++){
					neuralNetwork[i][y]->weights[d] = neuralNetwork[i][y]->weights[d] - (LEARNING_RATE / batchLength)*tuple[i].weight[d][y];
				}
			}
		}

		if (testing % 10000 == 0){
			cout << "Testing: " << testing << " Best: " << best;
			if (test() == VIDEO_CHECK_SET_SIZE)isDone = true;
		}
	}
}

void ToBeDisclosed::feedForward(double* inputData){
	for (int i = 0; i < networkColsSize[0]; i++)
		neuralNetwork[0][i]->activation = inputData[i];
	for (int i = 1; i < depthOfNetwork; i++){
		for (int x = 0; x < networkColsSize[i]; x++){
			for (int r = 0; r < networkColsSize[i + 1]; r++){
				neuralNetwork[i][x]->activation = neuralNetwork[i][x]->weights[r] * neuralNetwork[i-1][x]->activation + neuralNetwork[i][x]->bias;
			}
		}
	}
}

void ToBeDisclosed::backpropogation(double* inputData, double* expectedOut, DoubleTuple* tuple){
	feedForward(inputData);
	/*for (int i = 0; i < depthOfNetwork; i++){
		for (int x = 0; x < networkColsSize[i]; x++){
			for (int r = 0; r < networkColsSize[i + 1]; r++){
				if (i == 0)	neuralNetwork[i][x]->z += inputData[x] + neuralNetwork[i][x]->bias;
				else neuralNetwork[i][x]->z += neuralNetwork[i - 1][x]->weights[r] * neuralNetwork[i - 1][x]->activation + neuralNetwork[i][x]->bias;
			}
			neuralNetwork[i][x]->activation = networkMath::sigmoid(neuralNetwork[i][x]->z);
		}
	}*/

	for (int i = 0; i < OUTPUT_NEURONS; i++){
		double delta;
		delta = networkMath::costDerivative(neuralNetwork[depthOfNetwork - 1][i]->activation, expectedOut[i]) * networkMath::sigmoidPrime(neuralNetwork[depthOfNetwork - 1][i]->z);
		tuple[depthOfNetwork - 1].bias[i][0] = delta;

		for (int x = 0; x < networkColsSize[i]; x++){
			tuple[depthOfNetwork - 1].weight[i][x] = neuralNetwork[i][x]->activation * delta;
		}
	}

	for (int i = depthOfNetwork-2; i > 0; i--){
		for (int x = 0; x < networkColsSize[i]; x++){

			double sp = networkMath::sigmoidPrime(neuralNetwork[i][x]->z);
			double delta = 0;
			for (int r = 0; r < networkColsSize[i + 1]; r++){
				delta += neuralNetwork[i][x]->weights[r] * tuple[i + 1].bias[i][0];
			}
			tuple[i].bias[i][0] = delta;
			tuple[i].weight[i][x] = delta*neuralNetwork[i][x]->activation;
		}
	}

}