#include "matrix.h"

//Testing function that prints the contents of a vector matrix
void printMatrix(vector<vector<double> > a){
	for(unsigned int i=0; i<a.size(); i++){
		for(unsigned int j=0; j<a[i].size(); j++){
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

//Function to multiply two rectangular matricies
vector<vector<double> > multiply(vector<vector<double > > a, vector<vector<double > > b){
	unsigned int size = a[0].size();
	unsigned int rows = a.size();
	unsigned int columns = b[0].size();
	if(size != b.size()){
		cerr << "Matrix a height != Matrix b width" << endl;
	}

	//Allocate product matrix
	vector<vector<double> > prod;
	for(unsigned int i=0; i<rows; i++){
		vector<double> row;
		row.resize(columns);
		prod.push_back(row);
	}

	for(unsigned int row =0; row<rows; row++){
		for(unsigned int column = 0; column<columns; column++){
			double sum = 0;
			for(unsigned int k=0; k<size; k++){
				sum+= a[row][k] * b[k][column];
			}
			prod[row][column] = sum;
		}
	}

	return prod;
}

//Multiplies a matrix by a scalar value
vector<vector<double> > scalarMult(vector<vector<double> > a, double x){
	vector<vector<double> > prod;
	for(unsigned int i=0; i<a.size(); i++){
		vector<double> row;
		for(unsigned int j=0; j<a[i].size(); j++){
			row.push_back(a[i][j] * x);
		}
		prod.push_back(row);
	}

	return prod;
}

//Fuction requires a rectangular matrix of doubles stored in vectors
vector<vector<double> > transpose(vector<vector<double> > a){
	unsigned int rows = a.size();
	unsigned int columns = a[0].size();
	//TODO: add test for all rows being same size

	//Create vector to store transpose
	vector<vector<double> > at;
	for(unsigned int i = 0; i < columns; i++){
		vector<double> row;
		row.resize(rows);
		at.push_back(row);	
	}

	for(unsigned int i=0; i<rows; i++){
		for(unsigned int j=0; j<columns; j++){
			at[j][i] = a[i][j];
		}
	}

	return at;
}

vector<vector<double> > transitionMat(vector<vector<double> > costs){
	unsigned int width = costs[0].size();
	unsigned int height = costs.size();

	//Allocate vector
	vector<vector<double > > trans;
	for(unsigned int i=0; i<height; i++){
		vector<double> row;
		row.resize(width);
		trans.push_back(row);
	}

	for(unsigned int i=0; i<height; i++){
		double sum = 0;
		for(unsigned int j=0; j<width; j++){
			sum += 1/(costs[i][j]);
		}

		for(unsigned int j=0; j<width; j++){
			trans[i][j] = (1/(costs[i][j])) / sum;
		}
	}

	return trans;
}

vector<double> sumRows(vector<vector<double> > p){
	vector<double> sums;
	for(unsigned int i=0; i<p.size(); i++){
		double sum = 0;
		for(unsigned int j=0; j<p[i].size(); j++){
			sum += p[i][j];
		}
		sums.push_back(sum);
	}
	return sums;
}