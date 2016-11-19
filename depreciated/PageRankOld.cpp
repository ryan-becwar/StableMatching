#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "matrix.h"

using namespace std;

#define LARGE 10000000

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

	cout << "costs: " << endl;
	print_matrix(costs);
	vector<vector<double> > cT = transpose(costs);
//	print_matrix(cT);

	vector<vector<double> > t = multiply(costs, cT);
//	print_matrix(t);

	vector<vector<double> > p = transition_mat(costs);
	print_matrix(p);

	print_matrix(transpose(p));

	vector<vector<double> > ppt = multiply(p, transpose(p));
	print_matrix(ppt);

	const double a = .8;
	//Vector to be repeatedly multiplied by ppt
	//Stored as a matrix for compliance with multiplication functions
	vector<vector<double> > x;
	for(unsigned int i=0; i<width; i++){
		vector<double> row;
		row.push_back(1);
		x.push_back(row);
	}

	print_matrix(x);
	for(int i=0; i<5; i++){
		x = scalar_mult(x, a);
		x = multiply(ppt, x);
		printf("Iteration %d\n", i);
		print_matrix(x);
	}

	//Create vector to sort elements in x based off of transition probability
	vector<pid> rightSort;
	for(unsigned int i=0; i<x.size(); i++){
		pid p;
		p.first = i;
		p.second = x[i][0];
		rightSort.push_back(p);
	}

	cerr << "Sorting\n";
	sort(rightSort.begin(), rightSort.end(), sort_paird());
	vector<int> rightPreference;
	for(unsigned int i=0; i<rightSort.size(); i++){
		rightPreference.push_back(rightSort[i].first);
		cout << rightPreference[i] << endl;
	}

	vector<pii> matches;
	vector<bool> available;
	for(unsigned int i=0; i<rightPreference.size(); i++){
		available.push_back(true);
	}

	for(unsigned int i=0; i<rightPreference.size(); i++){
		int l = rightPreference[i];
		double minCost = LARGE;
		int minIndex;
		for(unsigned int j=0; j<width; j++){
			if(available[j] && costs[l][j] < minCost) {
				minCost = costs[l][j];
				minIndex = j;
			}
		}
		available[minIndex] = false;
		pii p;
		p.first = l;
		p.second = minIndex;
		matches.push_back(p);
	}

	sort(matches.begin(), matches.end(), sort_pairi());
	double totalCost = 0;
	cout << "Matches:" << endl;
	for(unsigned int i=0; i<matches.size(); i++){
		totalCost += costs[matches[i].first][matches[i].second];
		cout << matches[i].first << " " << matches[i].second <<
		" " << costs[matches[i].first][matches[i].second] << endl;
	}
	cout << totalCost << endl;


	return 0;
}