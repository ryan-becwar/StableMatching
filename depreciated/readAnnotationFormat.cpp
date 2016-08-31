#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Node {
public:
	int size;
	double allocation;

	Node() {
		size = 1;
		allocation = 1;
	}
};

class Edge {
public:
	int left, right;
	double value;
	double allocation;

	Edge() {
		allocation = 1;
	}
};

int main(int argc, char* argv[]){

	vector<Node> lhs, rhs;
	int lsize, rsize;
	vector<Edge> edges;

	string s;
	ifstream fin;
	fin.open(argv[1]);

	Node n;

	fin >> lsize;
	fin >> s;
	for(int i=0; i< lsize; i++){
		while(fin >> s && s != "node"){
			
		}
	}



}