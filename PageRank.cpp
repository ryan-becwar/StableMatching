#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include "matrix.h"
#include "matching_utilities.h"

#define LARGE 10000000
#define PAGERANK_EXP_VALUE .8
#define PAGERANK_STEP_COUNT 10




/*
Using an algorithm loosely modeled after the google pagerank algorith, we can
determine the priority with which we should match each node on the left side
of the matching based off of its centrality.  This generates a ranked order
based on that priority
*/
vector<unsigned int> generate_pagerank_order(vector<vector<double>> values, unsigned long width){

	vector<vector<double> > p = transitionMat(values);
	vector<vector<double> > ppt = multiply(p, transpose(p));

	//Vector to be repeatedly multiplied by ppt
	//Stored as a matrix for compliance with multiplication functions
	vector<vector<double> > x;
	for(unsigned int i=0; i<width; i++){
		vector<double> row;
		row.push_back(1);
		x.push_back(row);
	}

	#ifdef VERBOSE
		std::cout << "p:" << std::endl;
		printMatrix(p);

		std::cout << "ppt:" << std::endl;
		printMatrix(ppt);

		std::cout << "x\n";
		printMatrix(x);
	#endif

	for(int i=0; i<PAGERANK_STEP_COUNT; i++){
		x = scalarMult(x, PAGERANK_EXP_VALUE);
		x = multiply(ppt, x);
		#ifdef VERBOSE
		printf("Iteration %d\n", i);
		printMatrix(x);
		#endif
	}

	//Create vector to sort elements in x based off of transition probability
	vector<pid> rightSort;
	for(unsigned int i=0; i<x.size(); i++){
		pid pair;
		pair.first = i;
		//x is one dimensional matrix
		pair.second = x[i][0];
		rightSort.push_back(pair);
	}

	//Sort in ascending order
	sort(rightSort.begin(), rightSort.end(), sort_paird());
	reverse(rightSort.begin(), rightSort.end());

	vector<unsigned int> generatedOrder;
	for(unsigned int i=0; i<rightSort.size(); i++){
		generatedOrder.push_back(rightSort[i].first);
	}

	#ifdef VERBOSE
		std::cout << "rightSort: \n";
		print_pid(rightSort);
	#endif

	return generatedOrder;
}

int main(int argc, char *argv[]){

	//Readin Data
	Instance I = read_instance();
	unsigned long width = (unsigned long) I.lhsnodes.size();

	//get the edge values in matrix form
	vector<vector<double> > values = get_value_matrix(I);

	//find the greedy order that we obtain from the pagerank algorithm
	vector<unsigned int> pagerankOrder = generate_pagerank_order(values, width);

	vector<pii> matches = find_matches(values, pagerankOrder, width);
	//Write back matches to the instance
	write_matches(I, matches);

	double pageRankValue = get_value(I);

	#ifdef VERBOSE
		//write out information about matches
		double totalCost = 0;
		std::cout << matches.size() << " Matches:" << std::endl;
		for(unsigned int i=0; i<matches.size(); i++){
			totalCost += values[matches[i].first][matches[i].second];
			std::cout << matches[i].first << " " << matches[i].second <<
			" " << values[matches[i].first][matches[i].second] << std::endl;
		}
	std::cout << totalCost << std::endl;
	#endif

	#ifdef VERBOSE
	  print_instance(I);
	  std::cout << "value: " << pageRankValue << std::endl;
  #endif

	rank_results_against_random(I, values, width, pageRankValue);

	return 0;
}
