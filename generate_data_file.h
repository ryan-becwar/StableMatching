#ifndef STABLEMATCHING_GENERATEDATAFILE_H
#define STABLEMATCHING_GENERATEDATAFILE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cmath>
#include "echo_instance.h"

using namespace std;

typedef pair<double, double> Point;

typedef struct node_data {
    int listLength;
    vector<double> preferenceList;

    void init(int listLength){
        this->listLength = listLength;
        for(int i=0; i<listLength; i++){
            preferenceList.push_back(0.0);
        }
    }
} node_data;

void write_node_data(ostream &out, vector<node_data> &nodes);
Instance generate_weighted_data(int lhsCount, int rhsCount, double locationWeight, double rankWeight, double randomWeight);
Instance get_location_instance(unsigned int width, double noise);
Instance get_global_min_instance(unsigned int width, double noise);


#endif //STABLEMATCHING_GENERATEDATAFILE_H
