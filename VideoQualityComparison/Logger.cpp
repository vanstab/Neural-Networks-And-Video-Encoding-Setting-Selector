#include "Logger.h"

#include <fstream>

Logger::Logger()
{
}


Logger::~Logger()
{
}

void Logger::log(string logString, errorlevel e){
	ofstream  logfile;
	logfile.open("logs.txt", ios_base::app);
	if (e == warning)
		logfile << "WARNING: ";
	if (e == error)
		logfile << "ERROR: ";
	if (e == critical)
		logfile << "CRITICAL: ";
	logfile << logString << endl;
	logfile.close();
}