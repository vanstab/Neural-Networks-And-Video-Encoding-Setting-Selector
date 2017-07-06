#pragma once
#include "VideoData.h"
#include "defs.h"
class TrainningSet
{
public:
	TrainningSet();
	~TrainningSet();
	void train();
	void init();
	void load();
	void add(double*, double*);
	void normalise();
	double **list;
	double **out;
private:
	void toFile();
	int size;
	int nin;
	int nout;
};

