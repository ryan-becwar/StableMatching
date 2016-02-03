#include <iostream>
#include <vector>
#include <fstream>
#include "matrix.h"

using namespace std;

typedef pair<int, double> pid;

int main(int argc, char *argv[]){
	//Readin Data
	unsigned long width;
	vector<vector<double> > costs;

	ifstream fin;
	fin.open(argv[1]);	

	fin >> width;

	//Readin costs
	for(unsigned int i=0; i<width; i++){
		int connectionCount;
		fin >> connectionCount;
		vector<double> connectionCosts;
        for(int j=0; j<connectionCount; j++){
            double cost;
            fin >> cost;
            connectionCosts.push_back(cost);
        }
        costs.push_back(connectionCosts);
	}

	printMatrix(costs);
	vector<vector<double> > cT = transpose(costs);
//	printMatrix(cT);

	vector<vector<double> > t = multiply(costs, cT);
//	printMatrix(t);

	vector<vector<double> > p = transitionMat(costs);
	printMatrix(p);

	printMatrix(transpose(p));

	vector<vector<double> > ppt = multiply(p, transpose(p));
	printMatrix(ppt);

	const double a = .8;
	//Vector to be repeatedly multiplied by ppt
	//Stored as a matrix for compliance with multiplication functions
	vector<vector<double> > x;
	for(unsigned int i=0; i<width; i++){
		vector<double> row;
		row.push_back(1);
		x.push_back(row);
	}

	printMatrix(x);
	for(int i=0; i<5; i++){
		x = scalarMult(x, a);
		x = multiply(ppt, x);
		printf("Iteration %d\n", i);
		printMatrix(x);
	}

	return 0;
}