#include "Profile.h"
#include "defs.h"

Profile::Profile()
{
}


Profile::~Profile()
{
}
int Profile::getClassFromDouble(double in){
	for (double i = MAXPROFILE-1; i >= 0; i--){
		if (in > (i /(double) MAXPROFILE )) return i+1;
	}
	return 1;
}


double Profile::getDoubleFromClass(int in){
	return (double)in / (double)MAXPROFILE;
}
string Profile::getStringValue(Neuron** in){
	double best = -1;
	int out = -1;
	for (int i = 0; i < MAXPROFILE; i++){
		if (in[i]->activation>best){
			best = in[i]->activation;
			out = i;
		}
	}
	if (out == 0) return "baseline";
	else if (out == 1) return "main";
	else return "high";
}