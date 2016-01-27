#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

//Testing function that prints the contents of a vector matrix
void printMatrix(vector<vector<double> > a){
	for(int i=0; i<a.size(); i++){
		for(int j=0; j<a[i].size(); j++){
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

//Function to multiply two rectangular matricies
vector<vector<double> > multiply(vector<vector<double > > a, vector<vector<double > > b){
	int size = a[0].size();
	int rows = a.size();
	int columns = b[0].size();
	if(size != b.size()){
		cerr << "Matrix a height != Matrix b width" << endl;
	}

	//Allocate product matrix
	vector<vector<double> > prod;
	for(int i=0; i<rows; i++){
		vector<double> row;
		row.resize(columns);
		prod.push_back(row);
	}

	for(int row =0; row<rows; row++){
		for(int column = 0; column<columns; column++){
			double sum = 0;
			for(int k=0; k<size; k++){
				sum+= a[row][k] * b[k][column];
			}
			prod[row][column] = sum;
		}
	}

	return prod;
}

vector<vector<double> > scalarMult(vector<vector<double> > a, double x){
	vector<vector<double> > prod;
	for(int i=0; i<a.size(); i++){
		vector<double> row;
		for(int j=0; j<a[i].size(); j++){
			row.push_back(a[i][j] * x);
		}
		prod.push_back(row);
	}

	return prod;
}

//Fuction requires a rectangular matrix of doubles stored in vectors
vector<vector<double> > transpose(vector<vector<double> > a){
	int rows = a.size();
	int columns = a[0].size();
	//TODO: add test for all rows being same size

	//Create vector to store transpose
	vector<vector<double> > at;
	for(int i = 0; i < columns; i++){
		vector<double> row;
		row.resize(rows);
		at.push_back(row);	
	}

	for(int i=0; i<rows; i++){
		for(int j=0; j<columns; j++){
			at[j][i] = a[i][j];
		}
	}

	return at;
}

vector<vector<double> > transitionMat(vector<vector<double> > costs){
	int width = costs[0].size();
	int height = costs.size();

	//Allocate vector
	vector<vector<double > > trans;
	for(int i=0; i<height; i++){
		vector<double> row;
		row.resize(width);
		trans.push_back(row);
	}

	for(int i=0; i<height; i++){
		double sum = 0;
		for(int j=0; j<width; j++){
			sum += 1/(costs[i][j]);
		}

		for(int j=0; j<width; j++){
			trans[i][j] = (1/(costs[i][j])) / sum;
		}
	}

	return trans;
}

int main(int argc, char *argv[]){
	//Readin Data
	unsigned long width;
	vector<vector<double> > costs;

	ifstream fin;
	fin.open(argv[1]);	

	fin >> width;

	//Readin costs
	for(int i=0; i<width; i++){
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

	vector<vector<double> > ppt = multiply(p, transpose(p));
	printMatrix(ppt);
	return 0;
}