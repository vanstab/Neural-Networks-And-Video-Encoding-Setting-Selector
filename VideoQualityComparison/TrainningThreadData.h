#pragma once
class ToBeDiscolsed;
class DoubleTuple;
#include "Windows.h"
class TrainningThreadData
{
public:
	TrainningThreadData();
	~TrainningThreadData();
	ToBeDiscolsed* copyNet;
	DoubleTuple* temp;
	DoubleTuple* tuple;
	int startpoint;
};

