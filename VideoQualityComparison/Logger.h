#pragma once
#include <string>

#include <fstream>
using namespace std;
class Logger
{
public:
	Logger();
	~Logger();
	static enum errorlevel {warning, error, critical};
	static void log(string, errorlevel);
};

