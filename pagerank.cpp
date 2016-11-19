#include "pagerank.h"

#define LARGE 10000000
#define PAGERANK_EXP_VALUE .8
#define PAGERANK_STEP_COUNT 10


/*
Using an algorithm loosely modeled after the google pagerank algorith, we can
determine the priority with which we should match each node on the left side
of the matching based off of its centrality.  This generates a ranked order
based on that priority
*/
std::vector<unsigned int> generate_pagerank_order(Instance& I){
	std::vector<std::vector<double> > values = get_value_matrix(I);
	unsigned int width = values.size();

	std::vector<std::vector<double> > p = transition_mat(values);
	std::vector<std::vector<double> > ppt = multiply(p, transpose(p));

	//Vector to be repeatedly multiplied by ppt
	//Stored as a matrix for compliance with multiplication functions
	std::vector<std::vector<double> > x;
	for(unsigned int i=0; i<width; i++){
		std::vector<double> row;
		row.push_back(1);
		x.push_back(row);
	}

	#ifdef VERBOSE
		std::cout << "p:" << std::endl;
		print_matrix(p);

		std::cout << "ppt:" << std::endl;
		print_matrix(ppt);

		std::cout << "x\n";
		print_matrix(x);
	#endif

	for(int i=0; i<PAGERANK_STEP_COUNT; i++){
		x = scalar_mult(x, PAGERANK_EXP_VALUE);
		x = multiply(ppt, x);
		#ifdef VERBOSE
		printf("Iteration %d\n", i);
		print_matrix(x);
		#endif
	}

	//Create std::vector to sort elements in x based off of transition probability
	std::vector<pid> rightSort;
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

	std::vector<unsigned int> generatedOrder;
	for(unsigned int i=0; i<rightSort.size(); i++){
		generatedOrder.push_back(rightSort[i].first);
	}

	#ifdef VERBOSE
		std::cout << "rightSort: \n";
		print_pid(rightSort);
	#endif

	return generatedOrder;
}
