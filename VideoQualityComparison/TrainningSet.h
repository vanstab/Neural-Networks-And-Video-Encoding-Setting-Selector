#pragma once
#include "VideoData.h"
#include "defs.h"
class TrainningSet
{
public:
	TrainningSet();
	~TrainningSet();
	void init();
	void load();
	void add(VideoData*);
private:
	VideoData *list[MAX_ARRAY];
	void toFile();
	int size;
};

