#include "FeedForwardNetwork.h"
#include <math.h>
#include <Windows.h>
#include <iostream>
#include "BFrame.h"
#include "RefFrame.h"
#include "networkMath.h"
#include <ctime>
#include <exception>
using namespace std;
//build and inits first time neural network
FeedForwardNetwork::FeedForwardNetwork(int* cols, int size){
	//builds the neural network
	std::srand(std::time(0));
	neuralNetwork = new Neuron**[size];
	networkColsSize = cols;
	depthOfNetwork = size;
	//create biases 1-1 relationship ie each neuron gets one past first row
	for (int r = 0; r <size; r++){//for the depth not including first input layer
		neuralNetwork[r] = new Neuron*[cols[r]];//create an aray of node pointers
		for (int c = 0; c < cols[r]; c++){//create nodes and add biases to each of the nodes in layer
			neuralNetwork[r][c] = new Neuron();
			if (r >=1)
				neuralNetwork[r][c]->bias = networkMath::round((double)rand()/(RAND_MAX));
		}
	}
	//add weights 1- many ie each neuron has many wieghts realted to the next col excluding last col
	for (int r = 0; r < size-1; r++){//for the depth of the net -1
		for (int c = 0; c < cols[r]; c++){//for each node in that layer add a weight array
			neuralNetwork[r][c]->createWeights(cols[r+1]);
			for (int w = 0; w < cols[r + 1]; w++){//for each element in the array and the weight to the next layers node
				neuralNetwork[r][c]->weights[w] = networkMath::round((double)rand() / (RAND_MAX));//needs to be randomised maybe modified as better way is present apparently?	
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
void FeedForwardNetwork::train(){
	int itter = 0;
	//get input
	//now learn from input
	double* in;
	double* out;
	int batchLength = 500;
	in = new double[3]{-1, 1, -1};
	out = new double[3]{1, -1, 1};

	DoubleTuple* tuple, *temp;
	int testing = 0;
	while (testing < 500){
		std::cout << testing << endl;
		itter = 0;
		while (itter < 100){
			tuple = new DoubleTuple[depthOfNetwork - 1];
			for (int i = 0; i < depthOfNetwork - 1; i++){
				tuple[i].bias = new double*[networkColsSize[i + 1]];
				for (int d = 0; d < networkColsSize[i + 1]; d++){
					tuple[i].bias[d] = new double[1];
					tuple[i].bias[d][0] = 0;
				}

				tuple[i].weight = new double*[networkColsSize[i + 1]];
				for (int d = 0; d < networkColsSize[i + 1]; d++){
					tuple[i].weight[d] = new double[networkColsSize[i]];
					for (int x = 0; x < networkColsSize[i]; x++){
						tuple[i].weight[d][x] = 0;
					}
				}
			}
			try{
				temp = backpropogation(in, out);
			}
			catch (exception& e){
				cout << "error:" << e.what() << endl;
			}
			//keep track of changes
			try{
				for (int i = 0; i < depthOfNetwork - 1; i++){
					for (int d = 0; d < networkColsSize[i + 1]; d++){
						tuple[i].bias[d][0] += temp[i].bias[d][0];
					}
					for (int d = 0; d < networkColsSize[i + 1]; d++){
						for (int y = 0; y < networkColsSize[i]; y++){
							tuple[i].weight[d][y] += temp[i].weight[d][y];
						}
					}
				}

				//delete[] temp;
				//delete[] tuple;
				itter++;
			}catch (exception& e){
				cout << "error:" << e.what() << endl;
			}
		}
		//apply
		/*
		for (int i = 0; i < depthOfNetwork - 1; i++){
			for (int d = 0; d < networkColsSize[i + 1]; d++){
				neuralNetwork[i + 1][d]->bias = networkMath::round(neuralNetwork[i + 1][d]->bias - (LEARNING_RATE / batchLength)*tuple[i].bias[d][0]);
			}
			for (int d = 0; d < networkColsSize[i + 1]; d++){
				for (int y = 0; y < networkColsSize[i]; y++){
					neuralNetwork[i][d]->weights[y] = networkMath::round(neuralNetwork[i][d]->weights[y] - (LEARNING_RATE / batchLength)*tuple[i].weight[d][y]);
				}
			}
		}*/
		//double* input = new double[3]{ 1, 1, 1 };
		//feedForward(input);
		//cout << neuralNetwork[depthOfNetwork - 1][0]->z << "," << neuralNetwork[depthOfNetwork - 1][1]->z << "," << neuralNetwork[depthOfNetwork - 1][2]->z << endl;
		testing++;
		//delete[] in;
		//delete[] out;
	
		if (testing % 10 == 0){
			in = new double[3]{-1, 1, 1};
			out = new double[3]{1, -1, -1};
		}
		else if (testing % 8 == 0){

			in = new double[3]{1, 1, 1};
			out = new double[3]{-1, 1, 1};
		}
		else if (testing % 7 == 0){

			in = new double[3]{1, -1, -1};
			out = new double[3]{-1, 1, 1};
		}
		else if (testing % 6 == 0){

			in = new double[3]{1, 1, 1};
			out = new double[3]{-1, -1, -1};
		}
		else if (testing % 5 == 0){

			in = new double[3]{1, 1, 1};
			out = new double[3]{-1, -1, -1};
		}
		else if (testing % 4 == 0){

			in = new double[3]{-1, -1, 1};
			out = new double[3]{1, 1, -1};
		}
		else if (testing % 3 == 0){

			in = new double[3]{-1, -1, -1};
			out = new double[3]{1, 1, 1};
		}
		else if (testing % 2 == 0){
			in = new double[3]{-1, 1, -1};
			out = new double[3]{1, -1, 1};
		}
		else{
			in = new double[3]{1, -1, 1};
			out = new double[3]{-1, 1, -1};
		}

	}
	double* input = new double[3]{ -1, -1, -1 };
	feedForward(input);
	std::cout << neuralNetwork[depthOfNetwork - 1][0]->activation << "," << neuralNetwork[depthOfNetwork - 1][1]->activation << "," << neuralNetwork[depthOfNetwork - 1][2]->activation << endl;
    input = new double[3]{ 1, 1, 1 };
	feedForward(input);
	std::cout << neuralNetwork[depthOfNetwork - 1][0]->activation << "," << neuralNetwork[depthOfNetwork - 1][1]->activation << "," << neuralNetwork[depthOfNetwork - 1][2]->activation << endl;

	input = new double[3]{ -1, -1, 1 };
	feedForward(input);

	std::cout << "last:" << neuralNetwork[depthOfNetwork - 1][0]->activation << "," << neuralNetwork[depthOfNetwork - 1][1]->activation << "," << neuralNetwork[depthOfNetwork - 1][2]->activation << endl;

}

void FeedForwardNetwork::feedForward(double* inputData){
	for (int inputLayerNode = 0; inputLayerNode < networkColsSize[0]; inputLayerNode++){
		neuralNetwork[0][inputLayerNode]->activation = inputData[inputLayerNode];
	}
	for (int layer = 1; layer < depthOfNetwork; layer++){
		for (int node = 0; node < networkColsSize[layer]; node++){
			double total = 0;
			for (int weightToNode = 0; weightToNode < networkColsSize[layer - 1]; weightToNode++){
				total += networkMath::round(neuralNetwork[layer - 1][weightToNode]->weights[node] * neuralNetwork[layer - 1][weightToNode]->activation);
			}
			neuralNetwork[layer][node]->z = total;
			neuralNetwork[layer][node]->activation = networkMath::round(networkMath::sigmoid(total + neuralNetwork[layer][node]->bias));
		}
	}
	
}

DoubleTuple* FeedForwardNetwork::backpropogation(double* inputData, double* expectedOut){
	feedForward(inputData);

	double** delta = new double*[OUTPUT_NEURONS];
	for (int i = 0; i < OUTPUT_NEURONS; i++)
		delta[i] = new double[1]{};
	double* active[1];
	DoubleTuple* tuple = new DoubleTuple[depthOfNetwork - 1];
	active[0] = new double[networkColsSize[depthOfNetwork - 2]];
	for (int act = 0; act < networkColsSize[depthOfNetwork - 2]; act++)
		active[0][act] = neuralNetwork[depthOfNetwork - 2][act]->activation;
	
	for (int lastLayerNode = 0; lastLayerNode < OUTPUT_NEURONS; lastLayerNode++){
		delta[lastLayerNode][0] = networkMath::round(networkMath::costDerivative(neuralNetwork[depthOfNetwork - 1][lastLayerNode]->activation, expectedOut[lastLayerNode])
							   * networkMath::sigmoidPrime(neuralNetwork[depthOfNetwork - 1][lastLayerNode]->z));
		
	}
	tuple[depthOfNetwork - 2].bias = delta;
	tuple[depthOfNetwork - 2].weight = networkMath::dotProduct(1, OUTPUT_NEURONS, networkColsSize[depthOfNetwork - 2], delta, active);

	for (int curLayer = depthOfNetwork - 2; curLayer > 0; curLayer--){
		//delete active[0];
		active[0] = new double[networkColsSize[curLayer - 1]];
		for (int act = 0; act < networkColsSize[curLayer-1]; act++)
			active[0][act] = neuralNetwork[curLayer-1][act]->activation;


		double* sp = new double[networkColsSize[curLayer]];
		double** weights = new double*[networkColsSize[curLayer]];
		for (int node = 0; node < networkColsSize[curLayer]; node++){
			sp[node] = networkMath::round(networkMath::sigmoidPrime(neuralNetwork[curLayer][node]->z));
			weights[node] = neuralNetwork[curLayer][node]->weights;
		}
		delta = networkMath::dotProduct(networkColsSize[curLayer + 1], networkColsSize[curLayer], 1, weights, delta);
		for (int nodeDelta = 0; nodeDelta < networkColsSize[curLayer]; nodeDelta++)
			delta[0][nodeDelta] *= sp[nodeDelta];

		tuple[curLayer - 1].bias = delta;
		tuple[curLayer - 1].weight = networkMath::dotProduct(1, networkColsSize[curLayer], networkColsSize[curLayer - 1], delta, active);

	}
	//delete active[0];
	return tuple;
}