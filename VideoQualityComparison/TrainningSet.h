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
	void add(VideoData*);
	void normalise();
private:
	VideoData *list[MAX_ARRAY];
	void toFile();
	int size;
	int nin;
	int nout;
};

