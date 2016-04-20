#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void printMatrix(vector<vector<double> > a);
vector<vector<double> > multiply(vector<vector<double > > a, vector<vector<double > > b);
vector<vector<double> > scalarMult(vector<vector<double> > a, double x);
vector<vector<double> > transpose(vector<vector<double> > a);
vector<vector<double> > transitionMat(vector<vector<double> > costs);
vector<double> sumRows(vector<vector<double> > p);

