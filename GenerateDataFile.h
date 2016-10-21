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

typedef struct NodeData {
    int listLength;
    vector<double> preferenceList;

    void init(int listLength){
        this->listLength = listLength;
        for(int i=0; i<listLength; i++){
            preferenceList.push_back(0.0);
        }
    }
} NodeData;

void writeNodeData(ostream &out, vector<NodeData> &nodes);
Instance generateWeightedData(int lhsCount, int rhsCount, double locationWeight, double rankWeight, double randomWeight);

#endif //STABLEMATCHING_GENERATEDATAFILE_H
