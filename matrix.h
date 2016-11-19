#include <iostream>
#include <vector>
#include <fstream>

void print_matrix(std::vector<std::vector<double> > a);
std::vector<std::vector<double> > multiply(std::vector<std::vector<double > > a, std::vector<std::vector<double > > b);
std::vector<std::vector<double> > scalar_mult(std::vector<std::vector<double> > a, double x);
std::vector<std::vector<double> > transpose(std::vector<std::vector<double> > a);
std::vector<std::vector<double> > transition_mat(std::vector<std::vector<double> > costs);
std::vector<double> sum_rows(std::vector<std::vector<double> > p);
std::vector<double> get_column(std::vector<std::vector<double> >& matrix, unsigned int index);
