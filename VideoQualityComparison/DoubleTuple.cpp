#include "DoubleTuple.h"
#include <iostream>

DoubleTuple::DoubleTuple()
{
}


DoubleTuple::~DoubleTuple()
{
}

void DoubleTuple::DoubleTupleInit(int depth, int* networkColsSize){
	depthOfNetwork = depth;
	colSize = networkColsSize;
	bias = new double*[networkColsSize[depth + 1]];
	for (int d = 0; d < networkColsSize[depth + 1]; d++){
			bias[d] = new double[1];
			bias[d][0] = 0;
		}

	weight = new double*[networkColsSize[depth + 1]];
	for (int d = 0; d < networkColsSize[depth + 1]; d++){
		weight[d] = new double[networkColsSize[depth]];
		for (int x = 0; x < networkColsSize[depth]; x++){
				weight[d][x] = 0;
		}
	}
}

void DoubleTuple::clear(){
	for (int d = 0; d < colSize[depthOfNetwork + 1]; d++){
		bias[d][0] = 0;
	}
	for (int d = 0; d < colSize[depthOfNetwork + 1]; d++){
		for (int x = 0; x < colSize[depthOfNetwork]; x++){
			weight[d][x] = 0;
		}
	}
}
void DoubleTuple::addTuples(DoubleTuple*values){
	for (int d = 0; d < colSize[depthOfNetwork + 1]; d++){
		bias[d][0] += values->bias[d][0];
	}
	for (int d = 0; d < colSize[depthOfNetwork + 1]; d++){
		for (int y = 0; y < colSize[depthOfNetwork]; y++){
		weight[d][y] += values->weight[d][y];
		}
	}
}