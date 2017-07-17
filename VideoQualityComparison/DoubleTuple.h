#pragma once
class DoubleTuple
{
public:
	DoubleTuple();

	void DoubleTupleInit(int, int*);
	~DoubleTuple();
	double** weight;
	double** bias;
	void addTuples(DoubleTuple*);
	void clear();
private:
	int* colSize;
	int depthOfNetwork;
};

