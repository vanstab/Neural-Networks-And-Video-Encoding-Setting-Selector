#pragma once
#include <Windows.h>
class FeedForwardNetwork;
class CleanUpTrain
{
	class Node
	{
		friend class CleanUpTrain;
	private:
		FeedForwardNetwork* data;
		Node* next;
	};
public:
	CleanUpTrain();
	~CleanUpTrain();
	void push(FeedForwardNetwork*);
	void pop();
	bool isEmpty();

	bool killThread = false;
private:
	Node* head;
	HANDLE hThread;
	DWORD dwThreadId;
};

