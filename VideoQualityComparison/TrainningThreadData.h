#pragma once
class FeedForwardNetwork;
class DoubleTuple;
#include "Windows.h"
class TrainningThreadData
{
public:
	TrainningThreadData();
	~TrainningThreadData();
	FeedForwardNetwork* copyNet;
	DoubleTuple* temp;
	DoubleTuple* tuple;
	int startpoint;
};

