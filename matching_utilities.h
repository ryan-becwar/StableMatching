#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include "matrix.h"
#include "echo_instance.h"

using namespace std;

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

void print_pid(vector<pid> p);

vector<pii> find_matches(vector<vector<double>> values, vector<unsigned int> order, unsigned long width);
void write_matches(Instance& I, vector<pii> matches);
void rank_results_against_random(Instance& I, vector<vector<double>> values,
	unsigned int width, double pageRankValue);
