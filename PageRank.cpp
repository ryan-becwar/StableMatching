#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "matrix.h"
#include "echo_instance.h"

using namespace std;

#define LARGE 10000000

#define VERBOSE

typedef pair<int, int> pii;
struct sort_pairi{
	bool operator()(const pii &left, const pii &right){
		return left.first < right.first;
	}
};
typedef pair<int, double> pid;
struct sort_paird{
	bool operator()(const pid &left, const pid &right){
		return left.second > right.second;
	}
};

void print_pid(vector<pid> p){
	for(int i=0; i<p.size(); i++){
		cout << p[i].first << " " << p[i].second <<endl;
	}
}

int main(int argc, char *argv[]){

	Instance I = read_instance();
	//Readin Data
	unsigned long width = (unsigned long) I.lhsnodes.size();

	//Initialize values matrix to correct size
	vector<vector<double> > values;
	for(int i=0; i< I.lhsnodes.size(); i++){
		vector<double> connectionCosts;
		connectionCosts.resize(I.rhsnodes.size());
		values.push_back(connectionCosts);
	}

	//Assign values matrix from Instance edges
	for(int i=0; i<I.edges.size(); i++){
		values[I.edges[i].start][I.edges[i].end] = I.edges[i].value;
	}

	vector<vector<double> > vT = transpose(values);

	vector<vector<double> > vvT = multiply(values, vT);
	#ifdef VERBOSE
	printMatrix(values);
	printMatrix(vT);
	printMatrix(vvT);
	#endif

	vector<vector<double> > p = transitionMat(values);
	vector<vector<double> > ppt = multiply(p, transpose(p));

	const double a = .8;
	//Vector to be repeatedly multiplied by ppt
	//Stored as a matrix for compliance with multiplication functions
	vector<vector<double> > x;
	for(unsigned int i=0; i<width; i++){
		vector<double> row;
		row.push_back(1);
		x.push_back(row);
	}

	#ifdef VERBOSE
	cout << "ppt:" << endl;
	printMatrix(ppt);

	cout << "x\n";
	printMatrix(x);
	#endif

	for(int i=0; i<5; i++){
		x = scalarMult(x, a);
		x = multiply(ppt, x);
		#ifdef VERBOSE
		printf("Iteration %d\n", i);
		printMatrix(x);
		#endif
	}

	//Create vector to sort elements in x based off of transition probability
	vector<pid> rightSort;
	for(unsigned int i=0; i<x.size(); i++){
		pid p;
		p.first = i;
		//x is one dimensional matrix
		p.second = x[i][0];
		rightSort.push_back(p);
	}

	//Sort in ascending order
	sort(rightSort.begin(), rightSort.end(), sort_paird());
	reverse(rightSort.begin(), rightSort.end());

	#ifdef VERBOSE
	cout << "rightSort: \n";
	print_pid(rightSort);
	#endif

	vector<pii> matches;
	//Initialize available array to true
	vector<bool> available;
	for(unsigned int i=0; i<rightSort.size(); i++){
		available.push_back(true);
	}


	/*
	Principal eigenvector x (sorted into rightSort), corresponds to the flexibility of
	matching for each element on the right side of the graph: higher value means higher
	flexibility, so we want to greedily match right side elements with a low rightSort
	value first
	*/
	for(unsigned int i=0; i<rightSort.size(); i++){
		int r = rightSort[i].first;
		double maxVal = 0;
		int maxIndex;
		for(unsigned int j=0; j<width; j++){
			if(available[j] && values[j][r] > maxVal) {
				maxVal = values[j][r];
				maxIndex = j;
			}
		}
		available[maxIndex] = false;
		pii p;
		p.first = maxIndex;
		p.second = r;
		matches.push_back(p);
	}

	sort(matches.begin(), matches.end(), sort_pairi());

	#ifdef VERBOSE
	//write out matches
	double totalCost = 0;
	cout << matches.size() << " Matches:" << endl;
	for(unsigned int i=0; i<matches.size(); i++){
		totalCost += values[matches[i].first][matches[i].second];
		cout << matches[i].first << " " << matches[i].second <<
		" " << values[matches[i].first][matches[i].second] << endl;
	}
	cout << totalCost << endl;
	#endif

	//Write back matches to edges
	for(unsigned int i=0; i<matches.size(); i++){
		//Curretnly will fail of edges are out of order, sorting of edges may be needed to prevent
		int index = (matches[i].first * I.rhsnodes.size()) + matches[i].second;
		I.edges[index].allocation++;
		I.lhsnodes[I.edges[index].start].allocation++;
		I.rhsnodes[I.edges[index].end].allocation++;
	}

    print_instance(I);

    cout << "value: " << get_value(I) << endl;

	return 0;
}