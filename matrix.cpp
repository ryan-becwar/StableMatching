#include "matrix.h"

//Testing function that prints the contents of a std::vector matrix
void print_matrix(std::vector<std::vector<double> > a){
	for(unsigned int i=0; i<a.size(); i++){
		for(unsigned int j=0; j<a[i].size(); j++){
			std::cout << a[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//Function to multiply two rectangular matricies
std::vector<std::vector<double> > multiply(std::vector<std::vector<double > > a, std::vector<std::vector<double > > b){
	unsigned int size = a[0].size();
	unsigned int rows = a.size();
	unsigned int columns = b[0].size();
	if(size != b.size()){
		std::cerr << "Matrix a height != Matrix b width" << std::endl;
	}

	//Allocate product matrix
	std::vector<std::vector<double> > prod;
	for(unsigned int i=0; i<rows; i++){
		std::vector<double> row;
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
std::vector<std::vector<double> > scalar_mult(std::vector<std::vector<double> > a, double x){
	std::vector<std::vector<double> > prod;
	for(unsigned int i=0; i<a.size(); i++){
		std::vector<double> row;
		for(unsigned int j=0; j<a[i].size(); j++){
			row.push_back(a[i][j] * x);
		}
		prod.push_back(row);
	}

	return prod;
}

//Fuction requires a rectangular matrix of doubles stored in std::vectors
std::vector<std::vector<double> > transpose(std::vector<std::vector<double> > a){
	unsigned int rows = a.size();
	unsigned int columns = a[0].size();
	//TODO: add test for all rows being same size

	//Create std::vector to store transpose
	std::vector<std::vector<double> > at;
	for(unsigned int i = 0; i < columns; i++){
		std::vector<double> row;
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

//inverts the values of the provided matrix and scales to make each row sum to 1
std::vector<std::vector<double> > transition_mat(std::vector<std::vector<double> > costs){
	unsigned int width = costs[0].size();
	unsigned int height = costs.size();

	//Allocate std::vector
	std::vector<std::vector<double > > trans;
	for(unsigned int i=0; i<height; i++){
		std::vector<double> row;
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

std::vector<double> sum_rows(std::vector<std::vector<double> > p){
	std::vector<double> sums;
	for(unsigned int i=0; i<p.size(); i++){
		double sum = 0;
		for(unsigned int j=0; j<p[i].size(); j++){
			sum += p[i][j];
		}
		sums.push_back(sum);
	}
	return sums;
}

//get a vector of values in the specified column
std::vector<double> get_column(std::vector<std::vector<double> >& matrix, unsigned int index){
	std::vector<double> column;

	//handle error cases
	if(matrix.size() == 0){
		std::cerr << "Empty matrix provided to get_column function" << std::endl;
		return column;
	}

	for(unsigned int i=0; i<matrix.size(); i++){
		//if this row does not contain a value in this column error cases
		if(matrix[i].size() <= index){
			std::cerr << "Column specified is out of bounds" << std::endl;
			return column;
		}

		column.push_back(matrix[i][index]);
	}

	return column;
}
