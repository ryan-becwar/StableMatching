#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void print_matrix(vector<vector<double> > a);
vector<vector<double> > multiply(vector<vector<double > > a, vector<vector<double > > b);
vector<vector<double> > scalar_mult(vector<vector<double> > a, double x);
vector<vector<double> > transpose(vector<vector<double> > a);
vector<vector<double> > transition_mat(vector<vector<double> > costs);
vector<double> sum_rows(vector<vector<double> > p);

