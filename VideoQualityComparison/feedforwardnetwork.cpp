#include "FeedForwardNetwork.h"
#include <math.h>
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
DWORD WINAPI runBackPropagationThreads(LPVOID lpParam);
typedef struct THREADDATA {
	FeedForwardNetwork* fdfw;
	FeedForwardNetwork* copyNet;
	int threadNum;
	int setNum;
} THREADDATA, *PTHREADDATA;

FeedForwardNetwork::FeedForwardNetwork(int* cols, int size, TrainningSet* train){
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
	for (int r = 0; r < size-1; r++){//for the depth of the net -1
		for (int c = 0; c < cols[r]; c++){//for each node in that layer add a weight array
			neuralNetwork[r][c]->createWeights(cols[r+1]);
			for (int w = 0; w < cols[r + 1]; w++){//for each element in the array and the weight to the next layers node
				neuralNetwork[r][c]->weights[w] = dist(e2);//needs to be randomised maybe modified as better way is present apparently?	
			}
		}
	}
	threadTuples = new DoubleTuple*[MAX_THREADS];
}

FeedForwardNetwork::FeedForwardNetwork(const FeedForwardNetwork& obj){
	depthOfNetwork = obj.depthOfNetwork;
	networkColsSize = obj.networkColsSize;
	neuralNetwork = new Neuron**[depthOfNetwork];
	trainSet = NULL;
	threadTuples = NULL;
	for (int r = 0; r <depthOfNetwork; r++){//for the depth not including first input layer
	
		
		neuralNetwork[r] = new Neuron*[networkColsSize[r]];//create an aray of node pointers
		
		for (int c = 0; c < networkColsSize[r]; c++){//create nodes and add biases to each of the nodes in layer
			neuralNetwork[r][c] = new Neuron();
			if (r >= 1){
				neuralNetwork[r][c]->bias = obj.neuralNetwork[r][c]->bias;
			}
		}
	}
	//add weights 1- many ie each neuron has many wieghts realted to the next col excluding last col
	for (int r = 0; r < depthOfNetwork - 1; r++){//for the depth of the net -1
		for (int c = 0; c < networkColsSize[r]; c++){//for each node in that layer add a weight array
			neuralNetwork[r][c]->createWeights(networkColsSize[r + 1]);
			for (int w = 0; w < networkColsSize[r + 1]; w++){//for each element in the array and the weight to the next layers node
				neuralNetwork[r][c]->weights[w] = obj.neuralNetwork[r][c]->weights[w];//needs to be randomised maybe modified as better way is present apparently?	
			}
		}
	}
}

FeedForwardNetwork::~FeedForwardNetwork()
{
	for (int r = 0; r < depthOfNetwork-1; r++){
		
		for (int c = 0; c < networkColsSize[r]; c++){
			 delete neuralNetwork[r][c];
		}
		delete [] neuralNetwork[r];
	}
	delete[] neuralNetwork;

}
//
void FeedForwardNetwork::train(){
	
	//for threads
	PTHREADDATA pDataArray[MAX_THREADS];
	DWORD   dwThreadIdArray[MAX_THREADS];
	HANDLE  hThreadArray[MAX_THREADS];


	int itter = 0;
	random_device rd;
	mt19937 e2(rd());
	uniform_real_distribution<> dist(0, VIDEO_TRAIN_SET_SIZE);
	//get input
	//now learn from input
	int batchLength = VIDEO_TRAIN_SET_SIZE;

	DoubleTuple* tuple, *temp;

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
	//add array of tuple to list
	for (int threadCount = 0; threadCount < MAX_THREADS; threadCount++){
		temp = new DoubleTuple[depthOfNetwork - 1];
		for (int i = 0; i < depthOfNetwork - 1; i++){
			temp[i].bias = new double*[networkColsSize[i + 1]];
			for (int d = 0; d < networkColsSize[i + 1]; d++){
				temp[i].bias[d] = new double[1];
				temp[i].bias[d][0] = 0;
			}

			temp[i].weight = new double*[networkColsSize[i + 1]];
			for (int d = 0; d < networkColsSize[i + 1]; d++){
				temp[i].weight[d] = new double[networkColsSize[i]];
				for (int x = 0; x < networkColsSize[i]; x++){
					temp[i].weight[d][x] = 0;
				}
			}
		}
		threadTuples[threadCount] = temp;
	}

	int testing = 0;
	bool isDone = false;
	while (!isDone){
		testing++;
		itter = 0;
		for (int i = 0; i < depthOfNetwork - 1; i++){
			for (int d = 0; d < networkColsSize[i + 1]; d++){
				tuple[i].bias[d][0] = 0;
			}
			for (int d = 0; d < networkColsSize[i + 1]; d++){
				for (int x = 0; x < networkColsSize[i]; x++){
					tuple[i].weight[d][x] = 0;
				}
			}
		}
		int startpoint = dist(e2);
		while (itter < batchLength){
			//clears array for threads
			for (int threadCount = 0; threadCount < MAX_THREADS; threadCount++){
				for (int i = 0; i < depthOfNetwork - 1; i++){
					for (int d = 0; d < networkColsSize[i + 1]; d++){
						threadTuples[threadCount]->bias[d][0] = 0;
					}
					for (int d = 0; d < networkColsSize[i + 1]; d++){
						for (int x = 0; x < networkColsSize[i]; x++){
							threadTuples[threadCount]->weight[d][x] = 0;
						}
					}
				}
			}
			try{
				for (int threadCount = 0; threadCount < MAX_THREADS; threadCount++){
					pDataArray[threadCount] = (PTHREADDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(THREADDATA));
					if (pDataArray[threadCount] == NULL)
						ExitProcess(2);
					pDataArray[threadCount]->fdfw = this;
					pDataArray[threadCount]->threadNum = threadCount;
					pDataArray[threadCount]->setNum = ((itter + startpoint) % VIDEO_TRAIN_SET_SIZE);
					pDataArray[threadCount]->copyNet = new FeedForwardNetwork(*(pDataArray[threadCount]->fdfw));
					hThreadArray[threadCount] = CreateThread(NULL, 0,runBackPropagationThreads, pDataArray[threadCount], 0, &dwThreadIdArray[threadCount]);
					WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);
				}
				for (int threadCount = 0; threadCount < MAX_THREADS; threadCount++){
					CloseHandle(hThreadArray[threadCount]);
					if (pDataArray[threadCount] != NULL){
						{ 
							delete pDataArray[threadCount]->copyNet;
							HeapFree(GetProcessHeap(), 0, pDataArray[threadCount]);
							pDataArray[threadCount] = NULL;
						}
					}
				}
			}
			catch (exception& e){
				cout << "error:"<< e.what() << endl;
			}
			//keep track of changes
			try{
				for (int treadCount = 0; treadCount < MAX_THREADS; treadCount++){
					for (int i = 0; i < depthOfNetwork - 1; i++){
						for (int d = 0; d < networkColsSize[i + 1]; d++){
							tuple[i].bias[d][0] += threadTuples[treadCount]->bias[d][0];
						}
						for (int d = 0; d < networkColsSize[i + 1]; d++){
							for (int y = 0; y < networkColsSize[i]; y++){
								tuple[i].weight[d][y] += threadTuples[treadCount]->weight[d][y];
							}
						}
					}
				}
			}
			catch (exception& e){
				cout << "error:" << e.what()<< endl;
			}

			itter+=MAX_THREADS;
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
			int counter = 0,bwrong = 0, rwrong = 0, bitwrong = 0;
			cout << testing << ":";
			for (int runCheck = 0; runCheck < VIDEO_TRAIN_SET_SIZE; runCheck++){
				feedForward(trainSet->list[runCheck]);
				int wrongcheck = ValueCheck::check(neuralNetwork[depthOfNetwork - 1], trainSet->out[runCheck]);
				if (wrongcheck == 1) counter++;
				else if (wrongcheck == -3)bwrong++;
				else if (wrongcheck == -1)bitwrong++;
				else if (wrongcheck == -2)rwrong++;
			}
			if (counter >= VIDEO_TRAIN_SET_SIZE - 9)isDone = true;
			cout << counter << " " << bitwrong << " " << rwrong << " " << bwrong <<endl;
		}
	}
	
	/*
	feedForward(input);
	std::cout << neuralNetwork[depthOfNetwork - 1][0]->activation << "," << neuralNetwork[depthOfNetwork - 1][1]->activation << "," << neuralNetwork[depthOfNetwork - 1][2]->activation << endl;
    input = new double[3]{ 1, 1, 1 };
	feedForward(input);
	std::cout << neuralNetwork[depthOfNetwork - 1][0]->activation << "," << neuralNetwork[depthOfNetwork - 1][1]->activation << "," << neuralNetwork[depthOfNetwork - 1][2]->activation << endl;

	input = new double[3]{ -1, -1, 1 };
	feedForward(input);

	std::cout << "last:" << neuralNetwork[depthOfNetwork - 1][0]->activation << "," << neuralNetwork[depthOfNetwork - 1][1]->activation << "," << neuralNetwork[depthOfNetwork - 1][2]->activation << endl;
	*/
}

void FeedForwardNetwork::feedForward(double* inputData){
	for (int inputLayerNode = 0; inputLayerNode < networkColsSize[0]; inputLayerNode++){
			try{
				neuralNetwork[0][inputLayerNode]->activation = inputData[inputLayerNode];
			}
			catch (exception& e){
				cout << " feedforward in " << inputLayerNode << e.what() <<endl;
				throw e;
			}
		}
		for (int layer = 1; layer < depthOfNetwork; layer++){
			for (int node = 0; node < networkColsSize[layer]; node++){
				double total = 0;
				for (int weightToNode = 0; weightToNode < networkColsSize[layer - 1]; weightToNode++){
					total += neuralNetwork[layer - 1][weightToNode]->weights[node] * neuralNetwork[layer - 1][weightToNode]->activation;
				}
				neuralNetwork[layer][node]->z = total;
				try{
					neuralNetwork[layer][node]->activation = networkMath::sigmoid(total + neuralNetwork[layer][node]->bias);
				}catch (exception& e){
				cout << "feedforward hidden ";
				throw e;
				}
			}
		}
	
}

void FeedForwardNetwork::backpropogation(double* inputData, double* expectedOut,DoubleTuple* tuple){
	feedForward(inputData);
	try{
		double* active[1];
		active[0] = new double[networkColsSize[depthOfNetwork - 2]];
		for (int act = 0; act < networkColsSize[depthOfNetwork - 2]; act++)
			active[0][act] = neuralNetwork[depthOfNetwork - 2][act]->activation;

		for (int lastLayerNode = 0; lastLayerNode < OUTPUT_NEURONS; lastLayerNode++){
			tuple[depthOfNetwork - 2].bias[lastLayerNode][0] = networkMath::costDerivative(neuralNetwork[depthOfNetwork - 1][lastLayerNode]->activation, expectedOut[lastLayerNode])
				* networkMath::sigmoidPrime(neuralNetwork[depthOfNetwork - 1][lastLayerNode]->z);

		}
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
	}
}



DWORD WINAPI runBackPropagationThreads(LPVOID lpParam){
	PTHREADDATA data;
	HANDLE hStdout;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;
	data = (PTHREADDATA)lpParam;
	data->copyNet->backpropogation(data->fdfw->trainSet->list[data->setNum], data->fdfw->trainSet->out[data->setNum], data->fdfw->threadTuples[data->threadNum]);
	return 0;
}