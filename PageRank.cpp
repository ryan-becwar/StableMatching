#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include "matrix.h"
#include "echo_instance.h"

using namespace std;

#define LARGE 10000000
#define PAGERANK_EXP_VALUE .8
#define PAGERANK_STEP_COUNT 10

#define VERBOSE


//Used to sort by first index of matches, so sorts ascending
typedef pair<int, int> pii;
struct sort_pairi{
	bool operator()(const pii &left, const pii &right){
		return left.first < right.first;
	}
};

//Used to sort value of doulbe, so sorts descending
typedef pair<int, double> pid;
struct sort_paird{
	bool operator()(const pid &left, const pid &right){
		return left.second > right.second;
	}
};

void print_pid(vector<pid> p){
	for(unsigned int i=0; i<p.size(); i++){
		cout << p[i].first << " " << p[i].second <<endl;
	}
}

void rank_results_against_random(Instance& I, vector<vector<double>> values,
	unsigned int width, double pageRankValue);

/*
Principal eigenvector x (sorted into rightSort), corresponds to the flexibility of
matching for each element on the right side of the graph: higher value means higher
flexibility, so we want to greedily match right side elements with a low rightSort
value first

This essentially leads to a greedy matching solution, but done in the order of
the given vector
*/
vector<pii> find_matches(vector<vector<double>> values, vector<unsigned int> order, unsigned long width) {
	vector<pii> matches;

	//Initialize available array to true
	vector<bool> available;
	for(unsigned int i=0; i<order.size(); i++){
		available.push_back(true);
	}


	for(unsigned int i=0; i<order.size(); i++){
		int r = order[i];
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

	return matches;
}
/*
	saves matches state from the matches vector to the provided Instance variable
*/
void write_matches(Instance& I, vector<pii> matches){
	//0 out previous allocation status
	for(unsigned int i=0; i<I.edges.size(); i++){
		I.edges[i].allocation = 0;
	}
	for(unsigned int i=0; i<I.lhsnodes.size(); i++){
		I.lhsnodes[i].allocation = 0;
	}	for(unsigned int i=0; i<I.rhsnodes.size(); i++){
		I.rhsnodes[i].allocation = 0;
	}

	for(unsigned int i=0; i<matches.size(); i++){
		//Currently will fail of edges are out of order, sorting of edges may be needed to prevent
		int index = (matches[i].first * I.rhsnodes.size()) + matches[i].second;
		I.edges[index].allocation++;
		I.lhsnodes[I.edges[index].start].allocation++;
		I.rhsnodes[I.edges[index].end].allocation++;
	}
}


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
		cout << "ppt:" << endl;
		printMatrix(ppt);

		cout << "x\n";
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
		cout << "rightSort: \n";
		print_pid(rightSort);
	#endif


	return generatedOrder;
}

int main(int argc, char *argv[]){

	//Readin Data
	Instance I = read_instance();
	unsigned long width = (unsigned long) I.lhsnodes.size();

	//Initialize values matrix to correct size
	vector<vector<double> > values;
	for(unsigned int i=0; i< I.lhsnodes.size(); i++){
		vector<double> connectionCosts;
		connectionCosts.resize(I.rhsnodes.size());
		values.push_back(connectionCosts);
	}

	//Assign values to values matrix from Instance edges
	for(unsigned int i=0; i<I.edges.size(); i++){
		values[I.edges[i].start][I.edges[i].end] = I.edges[i].value;
	}

	//find the greedy order that we obtain from the pagerank algorithm
	vector<unsigned int> pagerankOrder = generate_pagerank_order(values, width);

	vector<pii> matches = find_matches(values, pagerankOrder, width);
	//Write back matches to the instance
	write_matches(I, matches);

	double pageRankValue = get_value(I);

	#ifdef VERBOSE
		vector<vector<double> > vT = transpose(values);
		vector<vector<double> > vvT = multiply(values, vT);
		printMatrix(vT);
		printMatrix(vvT);

		//write out information about matches
		double totalCost = 0;
		cout << matches.size() << " Matches:" << endl;
		for(unsigned int i=0; i<matches.size(); i++){
			totalCost += values[matches[i].first][matches[i].second];
			cout << matches[i].first << " " << matches[i].second <<
			" " << values[matches[i].first][matches[i].second] << endl;
		}
	cout << totalCost << endl;
	#endif

	#ifdef VERBOSE
	  print_instance(I);
	  cout << "value: " << pageRankValue << endl;
  #endif

	rank_results_against_random(I, values, width, pageRankValue);

	return 0;
}

void rank_results_against_random(Instance& I, vector<vector<double>> values,
	unsigned int width, double pageRankValue) {
  //random engine needed for random ordering
  default_random_engine randomEngine;

  const unsigned int greedyCount = 100;
  vector<double> greedyValueResults;
  for(unsigned int i=0; i<greedyCount; i++){
  	//generate random order vector
  	vector<unsigned int> randomOrder;
  	for(unsigned int j=0; j<width; j++){
  		randomOrder.push_back(j);
  	}

  	std::shuffle(std::begin(randomOrder), std::end(randomOrder), randomEngine);


  	//calculate value of matching based on this order
  	vector<pii> randomMatches = find_matches(values, randomOrder, width);
		//Write back matches to the instance
		write_matches(I, randomMatches);
		greedyValueResults.push_back(get_value(I));
  }

  std::sort(greedyValueResults.begin(), greedyValueResults.end());

  #ifdef VERBOSE
  for(unsigned int i=0; i<greedyValueResults.size(); i++){
  	cout << greedyValueResults[i] << endl;
  }
  #endif

  //TODO: implement this as a binary search, written like this quickly
  //Find how the smart value compares to random greedy values
  unsigned int pageRankValueRank;
  for(pageRankValueRank = 0; pageRankValueRank < greedyCount &&
  	pageRankValue > greedyValueResults[pageRankValueRank]; pageRankValueRank++);

  cout << "matching order determined using pagerank algorithm ranked " << pageRankValueRank <<
	" out of " << greedyCount << " random orderings." << endl;
}
