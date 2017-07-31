#include "networkMath.h"
#include <math.h>
#include <iostream>
#include "defs.h"
#include "CustomExceptions.h"
/*
int shared dimension
int first rows
int seconds cols
*/


void networkMath::dotProduct(int n, int row, int cols, double** first, double** second, double** out){

	for (int y = 0; y < row; y++){
		for (int x = 0; x < cols; x++){
			out[y][x] = (double)0.0;

			for (int dot = 0; dot < n; dot++){
				out[y][x] += first[y][dot] * second[dot][x];
			}
			if (out[y][x] != out[y][x]){
				std::cout << "dot:";
				throw NANException();
			}
		}
	}
}

double networkMath::sigmoid(double z){
	if (z > 45) return 1.0;
	if (z < -45) return 0.0;
	if (z!=z || 1 / (1.0 + exp(-z)) != 1 / (1.0 + exp(-z))){
		throw NANException();
	}
	return  1.0 / (1.0 + exp(-z));
}
/*
int height
int total width
int first array width
*/
double** networkMath::horizontalConcat(double** first, double** second, int height, int width, int firstWidth){
	double** returnMatrix = new double*[height];
	for (int i = 0; i < height; i++)
		returnMatrix[i] = new double[width];

	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			if (x < firstWidth)
				returnMatrix[y][x] = first[y][x];
			else returnMatrix[y][x] = second[y][x-firstWidth];
		}
	}
	return returnMatrix;
}

double** networkMath::horizontalConcat(double* first, double* second, int height){
	double** returnMatrix = new double*[height];
	for (int i = 0; i < height; i++)
		returnMatrix[i] = new double[2];

	for (int y = 0; y < height; y++){
		returnMatrix[y][0] = first[y];
		returnMatrix[y][1] = second[y];
	}
	return returnMatrix;
}
double* networkMath::hammardOneD(double* error, double* active, int size){
	double* der = derivativeOneD(active,size);
	for (int i = 0; i < size; i++){
		der[i] = der[i] * error[i];
	}
	return der;
}
double* networkMath::derivativeOneD(double* in, int size){
	double* out = new double[size];
	for (int i = 0; i < size; i++){
		out[i] = sigmoidPrime(in[i]);
	}
	return out;
}
double networkMath::sigmoidPrime(double z){
	if ((sigmoid(z)*(1 - sigmoid(z))) != (sigmoid(z)*(1 - sigmoid(z)))){
		std::cout << "primesig:";
		throw NANException();
	}
	return sigmoid(z)*(1 - sigmoid(z));
}
double** networkMath::scalar_mul(double eta, double** mat, int height, int length){
	double** out = new double*[height];
	for (int i = 0; i < height; i++){
		out[i] = new double[length];
		for (int x = 0; x < length; x++){
			out[i][x] = mat[i][x] * eta;
		}
	}
	return out;
}
double** networkMath::kronecker(double** a, double** b,int firsth,int firstw, int secondh,int secondw){
	double** out = new double*[firsth*secondh];
	for (int i = 0; i < firsth*secondh; i++)
		out[i] = new double[firstw*secondw];
	for (int i = 0; i < firsth; i++){
		for (int x = 0; x < firstw; x++){
			for (int z = i* secondh; z < (i + 1)*secondh; z++){
				for (int y = x*secondw; y < (x + 1)*secondw; y++){
					out[z][y] = a[i][x] * b[z%secondh][y%secondw];
				}
			}
		}
	}
	return out;
}

double** networkMath::transpose(double** in, int length, int height){
	double** trans = new double*[length];
	for (int i = 0; i < length; i++){
		trans[i] = new double[height];
	}
	for (int i = 0; i < height; i++){
		for (int x = 0; x < length; x++){
			trans[length][height] = in[height][length];
		}
	}
	return trans;
}
double networkMath::costDerivative(double activation, double eta){
	return activation - eta;
}