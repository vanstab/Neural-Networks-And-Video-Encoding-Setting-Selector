#pragma once
#include "VideoData.h"
#include "defs.h"
class TrainningSet
{
public:
	TrainningSet(int,int);
	~TrainningSet();
	void train();
	void init();
	void load();
	void add(double*, double*);
	void normalise();
	double **list;
	double **out;
	void toFile();
private:
	int size;
	int MAX_SIZE;
	int MAX_OUT_SIZE;
	string inPath;
	string outPath;
	bool bitRateNorm = false;
};

