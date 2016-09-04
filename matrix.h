#include <iostream>
#include <vector>
#include <fstream>

void printMatrix(std::vector<std::vector<double> > a);
std::vector<std::vector<double> > multiply(std::vector<std::vector<double > > a, std::vector<std::vector<double > > b);
std::vector<std::vector<double> > scalarMult(std::vector<std::vector<double> > a, double x);
std::vector<std::vector<double> > transpose(std::vector<std::vector<double> > a);
std::vector<std::vector<double> > transitionMat(std::vector<std::vector<double> > costs);
std::vector<double> sumRows(std::vector<std::vector<double> > p);
