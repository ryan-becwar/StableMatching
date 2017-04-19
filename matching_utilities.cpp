#include "matching_utilities.h"

//using namespace std;

#define VERBOSE

void print_pid(vector<pid> p){
	for(unsigned int i=0; i<p.size(); i++){
		std::cout << p[i].first << " " << p[i].second <<std::endl;
	}
}



/*make
Principal eigenvector x (sorted into rightSort), corresponds to the flexibility of
matching for each element on the right side of the graph: higher value means higher
flexibility, so we want to greedily match right side elements with a low rightSort
value first

This essentially leads to a greedy matching solution, but done in the order of
the given vector

greedily matches based on value, not cost
*/
vector<pii> find_matches(vector<vector<double>> values, vector<unsigned int> order, unsigned long width) {
	vector<pii> matches;

	//Initialize available array to true
	vector<bool> available;
	for(unsigned int i=0; i<width; i++){
		available.push_back(true);
	}


	for(unsigned int i=0; i<order.size(); i++){
		int l = order[i];
		double maxVal = 0;
		int maxIndex = 0;
		for(unsigned int j=0; j<width; j++){
			if(available[j] && values[l][j] > maxVal) {
				maxVal = values[l][j];
				maxIndex = j;
			}
		}
		available[maxIndex] = false;
		pii p;
		p.first = l;
		p.second = maxIndex;
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
