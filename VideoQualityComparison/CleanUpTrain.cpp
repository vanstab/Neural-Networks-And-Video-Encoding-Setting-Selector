#include "CleanUpTrain.h"
#include "feedforwardnetwork.h"
#include <iostream>
DWORD WINAPI cleanUp(LPVOID lpParam);

CleanUpTrain::CleanUpTrain()
{
	head = 0;
	hThread = CreateThread(NULL, 0, cleanUp, this, 0, &dwThreadId);
}


CleanUpTrain::~CleanUpTrain()
{
	killThread = true;
	WaitForSingleObject(hThread, INFINITE);
}

void CleanUpTrain::push(FeedForwardNetwork* f){
	Node* tempNode = new Node;
	tempNode->data = f;
	tempNode->next = 0;
	if (head == 0){
		head = tempNode;
		return;
	}
	tempNode->next = head;
	head = tempNode;
}
void CleanUpTrain::pop(){
	if (head == 0) return;
	Node* temp = head->next;
	delete head->data;
	delete head;
	head = temp;
}
bool CleanUpTrain::isEmpty(){
	if (head == 0) return true;
	return false;
}
DWORD WINAPI cleanUp(LPVOID lpParam){
	CleanUpTrain* clean = (CleanUpTrain*)lpParam;
	FeedForwardNetwork* f = 0;
	while (!clean->killThread || !clean->isEmpty()){
		if (clean->isEmpty()){
			Sleep(1);
		}
		clean->pop();
	}
	return 0;
}
