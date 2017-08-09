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
