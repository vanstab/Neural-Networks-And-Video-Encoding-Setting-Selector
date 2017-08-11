#include "NeuralNetwork.h"

#include <iostream>
#include "ValueCheck.h"
using namespace std;
NeuralNetwork::NeuralNetwork(int* cols, int size, TrainningSet* train, TrainningSet* check, int learning)
{
	type = learning;
	//builds the neural network
	trainSet = train;
	checkSet = check;

	neuralNetwork = new Neuron**[size];
	networkColsSize = cols;
	depthOfNetwork = size;
	for (int r = 0; r <size; r++){
		int numOut;
		if (r != size-1)
			 numOut = cols[r + 1];
		else numOut = 1;
		
		neuralNetwork[r] = new Neuron*[cols[r]+1];
		for (int c = 0; c < cols[r]+1; c++){
				neuralNetwork[r][c] = new Neuron(numOut, c);
		}
		neuralNetwork[r][cols[r]]->activation = 1.0;
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

int NeuralNetwork::test(int runCheck){
	int correct = 0, one = 0;
	//	ValueCheck::check(neuralNetwork[depthOfNetwork - 1], trainSet->out[(runCheck)], correct, one, networkColsSize[depthOfNetwork - 1]);
		
//	cout << " Train: " <<correct << " ";
	one = correct= 0;
	try{
		feedForward(checkSet->list[(runCheck)]);
		cout << "     In" << ":";
		for (int i = 0; i < networkColsSize[0]; i++){
			cout << checkSet->list[runCheck][i] << " ";
		}
		cout << endl;
		cout << "Got:      ";
		for (int i = 0; i < networkColsSize[depthOfNetwork - 1]; i++){
			cout << round(neuralNetwork[depthOfNetwork - 1][i]->getoutput()*100)/100<< " ";
		}
		cout << endl;
		cout << "Expected: ";
		for (int i = 0; i < networkColsSize[depthOfNetwork - 1]; i++){
			cout << checkSet->out[(runCheck)][i] << " ";
		}

		cout << endl;
		
	}
	catch (exception& e){
		cout << e.what();
	}
	//cout << " Correct: " << correct << " " << one << endl;	
	return correct;
}
void NeuralNetwork::check(){
	int count = 0;
	int wroung = 0;
	for (int x = 0; x< VIDEO_CHECK_SET_SIZE; x++){
		feedForward(checkSet->list[x]);
		cout << "     In" << ":";
		for (int i = 0; i < networkColsSize[0]; i++){
			cout << checkSet->list[x][i] << " ";
		}
		cout << endl;
		cout << "Got:      ";
		for (int i = 0; i < networkColsSize[depthOfNetwork - 1]; i++){
			cout << neuralNetwork[depthOfNetwork - 1][i]->getoutput() << " ";
		}
		cout << endl;
		cout << "Expected: ";
		for (int i = 0; i < networkColsSize[depthOfNetwork - 1]; i++){
			cout << checkSet->out[(x)][i] << " ";
		}

		cout << endl;
		ValueCheck::check(neuralNetwork[depthOfNetwork - 1], checkSet->out[x], count, wroung, networkColsSize[depthOfNetwork - 1]);
	}
	cout << "Correct: " << count<< " " << wroung<<endl;
}