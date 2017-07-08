#pragma once
class networkMath
{
public:
	static void dotProduct(int, int, int, double**, double**, double**);
	static double** horizontalConcat(double**, double**, int, int, int);
	static double** horizontalConcat(double*, double*, int);
	static double   sigmoid(double);
	static double   sigmoidPrime(double);
	static double*  hammardOneD(double*, double *, int);
	static double*  derivativeOneD(double*, int);
	static double** scalar_mul(double, double**, int,int);
	static double** kronecker(double**, double**, int,int,int,int);
	static double** transpose(double**, int, int);
	static double   costDerivative(double, double);
	static double   round(double);
};
