#include "ToBeDisclosed.h"
#include <iostream>
#include <sstream>
#include <exception>
#include "BFrame.h"
#include "RefFrame.h"
#include "TrainningThreadData.h"
#include <random>

using namespace std;
ToBeDisclosed::ToBeDisclosed(int* cols, int size, TrainningSet* train) : NeuralNetwork(cols, size, train)
{
}


ToBeDisclosed::~ToBeDisclosed()
{
}



void ToBeDisclosed::feedForward(double* inputData){
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
				total += neuralNetwork[layer - 1][weightToNode]->weights[node] * neuralNetwork[layer - 1][weightToNode]->activation;
			}
			try{
				neuralNetwork[layer][node]->activation = networkMath::sigmoid(total + neuralNetwork[layer][node]->bias);
			}
			catch (exception& e){
				cout << "feedforward hidden ";
				throw e;
			}
		}
	}

}
void ToBeDisclosed::train(){
	//init thread values
	TrainningThreadData *dataArray[MAX_THREADS];
	DWORD dwThreadArray[MAX_THREADS];
	HANDLE hThreadArray[MAX_THREADS];
	for (int threads = 0; threads < MAX_THREADS; threads++){
		dataArray[threads] = new TrainningThreadData();
	//	dataArray[threads]->copyNet = new FeedForwardNetwork(*this);
		DoubleTuple* tuple, *temp;
	//	tuple = new DoubleTuple[depthOfNetwork - 1];
	//	temp = new DoubleTuple[depthOfNetwork - 1];
		for (int i = 0; i < depthOfNetwork - 1; i++){
	//		tuple[i].DoubleTupleInit(i, networkColsSize);
	//		temp[i].DoubleTupleInit(i, networkColsSize);
		}
//		dataArray[threads]->temp = temp;
	//	dataArray[threads]->tuple = tuple;

	}



	//init class values
	int itter = 0;
	random_device rd;
	mt19937 e2(rd());
	uniform_real_distribution<> dist(0, VIDEO_TRAIN_SET_SIZE);

	//store values to be updated
	DoubleTuple* tuple;
	tuple = new DoubleTuple[depthOfNetwork - 1];
	for (int i = 0; i < depthOfNetwork - 1; i++){
		tuple[i].DoubleTupleInit(i, networkColsSize);
	}
	int testing = 0;
	bool isDone = false;
	while (!isDone){

		//reset for batch;
		testing++;
		itter = 0;
		int startpoint = dist(e2);
		//non-thread tuple used to apply updates
		for (int i = 0; i < depthOfNetwork - 1; i++){
			tuple[i].clear();
		}
		while (itter < VIDEO_TRAIN_SET_SIZE){
			backpropogation(trainSet->list[(startpoint + itter) % VIDEO_TRAIN_SET_SIZE], trainSet->out[(startpoint + itter) % VIDEO_TRAIN_SET_SIZE], NULL);
			itter++;
		}
		/*
		try{

		for (int threads = 0; threads < MAX_THREADS; threads++){
		dataArray[threads]->startpoint = threads + startpoint;
		hThreadArray[threads] = CreateThread(NULL, 0, runBackPropagationThreads, dataArray[threads], 0, &dwThreadArray[threads]);
		}

		}

		catch (exception& e){
		cout << "error:" << e.what() << endl;
		}
		WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

		for (int threadCount = 0; threadCount < MAX_THREADS; threadCount++){
		CloseHandle(hThreadArray[threadCount]);
		}
		//add all updates to non-thread tuple from thread tuples
		for (int threads = 0; threads < MAX_THREADS; threads++){
		for (int i = 0; i < depthOfNetwork - 1; i++){
		//might not work?
		tuple[i].addTuples(&dataArray[threads]->tuple[i]);
		}
		}
		*/
		//updateWeights(tuple);
		if (testing % 1000 == 0){
			cout << "Testing: " << testing << " Best: " << best << " Correct:";
			if (test() == VIDEO_TRAIN_SET_TOTAL - VIDEO_TRAIN_SET_SIZE)isDone = true;
		}
	}
}



void ToBeDisclosed::backpropogation(double* inputData, double* expectedOut, DoubleTuple* tuple){
	feedForward(inputData);

	if (abs((neuralNetwork[depthOfNetwork - 1][0]->activation * 15000) - (expectedOut[0] * 15000)) <= 75) neuralNetwork[depthOfNetwork - 1][0]->gradient = 0;
	else neuralNetwork[depthOfNetwork - 1][0]->gradient = (expectedOut[0] - neuralNetwork[depthOfNetwork - 1][0]->activation)*(1 - neuralNetwork[depthOfNetwork - 1][0]->activation)*(1 + neuralNetwork[depthOfNetwork - 1][0]->activation);
	if (RefFrame::getClassFromDouble(expectedOut[1]) == RefFrame::getClassFromDouble(neuralNetwork[depthOfNetwork - 1][1]->activation))
		neuralNetwork[depthOfNetwork - 1][1]->gradient = 0;
	else neuralNetwork[depthOfNetwork - 1][1]->gradient = (expectedOut[1] - neuralNetwork[depthOfNetwork - 1][1]->activation)*(1 - neuralNetwork[depthOfNetwork - 1][1]->activation)*(1 + neuralNetwork[depthOfNetwork - 1][1]->activation);
	if (BFrame::getClassFromDouble(expectedOut[2]) == BFrame::getClassFromDouble(neuralNetwork[depthOfNetwork - 1][2]->activation))
		neuralNetwork[depthOfNetwork - 1][2]->gradient = 0;
	else neuralNetwork[depthOfNetwork - 1][2]->gradient = (expectedOut[2] - neuralNetwork[depthOfNetwork - 1][2]->activation)*(1 - neuralNetwork[depthOfNetwork - 1][2]->activation)*(1 + neuralNetwork[depthOfNetwork - 1][2]->activation);


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
			neuralNetwork[0][input]->weights[nextLayer] += neuralNetwork[0][input]->prevDelta[nextLayer] * MOMENTUM + neuralNetwork[0][input]->delta[nextLayer];
			neuralNetwork[0][input]->prevDelta[nextLayer] = neuralNetwork[0][input]->delta[nextLayer];
		}
	}
	for (int layer = 1; layer < depthOfNetwork - 1; layer++){
		for (int node = 0; node < networkColsSize[layer]; node++){
			for (int nextLayer = 0; nextLayer < networkColsSize[layer + 1]; nextLayer++){
				neuralNetwork[layer][node]->delta[nextLayer] = LEARNING_RATE*neuralNetwork[layer + 1][nextLayer]->gradient*neuralNetwork[layer][node]->activation*networkMath::sigmoidPrime(neuralNetwork[layer][node]->activation + neuralNetwork[layer][node]->bias);
				neuralNetwork[layer][node]->weights[nextLayer] += neuralNetwork[layer][node]->prevDelta[nextLayer] * MOMENTUM + neuralNetwork[layer][node]->delta[nextLayer];
				neuralNetwork[layer][node]->prevDelta[nextLayer] = neuralNetwork[layer][node]->delta[nextLayer];
			}
			neuralNetwork[layer][node]->bias = LEARNING_RATE *neuralNetwork[layer][node]->gradient;
		}
	}

	for (int output = 0; output < OUTPUT_NEURONS; output++){
		neuralNetwork[depthOfNetwork - 1][output]->bias = LEARNING_RATE *neuralNetwork[depthOfNetwork - 1][output]->gradient;
	}

	/*try{
	double* active[1];
	active[0] = new double[networkColsSize[depthOfNetwork - 2]];
	for (int act = 0; act < networkColsSize[depthOfNetwork - 2]; act++)
	active[0][act] = neuralNetwork[depthOfNetwork - 2][act]->activation;

	//for (int lastLayerNode = 0; lastLayerNode < OUTPUT_NEURONS; lastLayerNode++){

	tuple[depthOfNetwork - 2].bias[0][0] = networkMath::costDerivative(neuralNetwork[depthOfNetwork - 1][0]->activation, expectedOut[0])
	* networkMath::sigmoidPrime(neuralNetwork[depthOfNetwork - 1][0]->z);
	if (RefFrame::getClassFromDouble(neuralNetwork[depthOfNetwork - 1][1]->activation) == RefFrame::getClassFromDouble(expectedOut[1]))
	tuple[depthOfNetwork - 2].bias[1][0] = 0;
	else 	tuple[depthOfNetwork - 2].bias[1][0] = networkMath::costDerivative(neuralNetwork[depthOfNetwork - 1][1]->activation, expectedOut[1])
	* networkMath::sigmoidPrime(neuralNetwork[depthOfNetwork - 1][1]->z);

	if (BFrame::getClassFromDouble(neuralNetwork[depthOfNetwork - 1][2]->activation) == BFrame::getClassFromDouble(expectedOut[2]))
	tuple[depthOfNetwork - 2].bias[2][0] = 0;
	else	tuple[depthOfNetwork - 2].bias[2][0] = networkMath::costDerivative(neuralNetwork[depthOfNetwork - 1][2]->activation, expectedOut[2])
	* networkMath::sigmoidPrime(neuralNetwork[depthOfNetwork - 1][2]->z);
	//}
	networkMath::dotProduct(1, OUTPUT_NEURONS, networkColsSize[depthOfNetwork - 2], tuple[depthOfNetwork - 2].bias, active, tuple[depthOfNetwork - 2].weight);

	for (int curLayer = depthOfNetwork - 2; curLayer > 0; curLayer--){
	delete active[0];
	active[0] = new double[networkColsSize[curLayer - 1]];
	for (int act = 0; act < networkColsSize[curLayer - 1]; act++)
	active[0][act] = neuralNetwork[curLayer - 1][act]->activation;


	double* sp = new double[networkColsSize[curLayer]];
	double** weights = new double*[networkColsSize[curLayer]];
	for (int node = 0; node < networkColsSize[curLayer]; node++){
	sp[node] = networkMath::sigmoidPrime(neuralNetwork[curLayer][node]->z);
	weights[node] = neuralNetwork[curLayer][node]->weights;
	}
	networkMath::dotProduct(networkColsSize[curLayer + 1], networkColsSize[curLayer], 1, weights, tuple[curLayer].bias, tuple[curLayer - 1].bias);

	for (int nodeDelta = 0; nodeDelta < networkColsSize[curLayer]; nodeDelta++)
	tuple[curLayer - 1].bias[nodeDelta][0] *= sp[nodeDelta];

	networkMath::dotProduct(1, networkColsSize[curLayer], networkColsSize[curLayer - 1], tuple[curLayer - 1].bias, active, tuple[curLayer - 1].weight);
	delete[] sp;
	delete[] weights;
	}
	delete active[0];
	}
	catch (exception& e){
	cout << "backprop error";
	throw e;
	}*/
}

