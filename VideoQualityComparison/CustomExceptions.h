#pragma once
#include <exception>
class DividByZeroExcpetion : public std::exception{
	public:
		const char * what() const throw(){
			return "Attempted to Divid By Zero";
		}
};

class NANException : public std::exception{
public:
	const char * what() const throw(){
		return "NAN or QNAN Detected";
	}
};