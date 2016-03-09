#include "GenerateDataFile.h"
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


void assignLocationWeight(vector<NodeData> &left, vector<NodeData> &right, double locationWeight){
    vector<Point> leftPoints;
    vector<Point> rightPoints;
    //Std random number generation
    default_random_engine randomEngine;
    uniform_real_distribution<double> distribution(0.0, 1.0);

    //Assign random points
    for(int i=0; i<left.size(); i++){
        Point p;
        p.first = distribution(randomEngine);
        p.second = distribution(randomEngine);
        leftPoints.push_back(p);
    }

    for(int i=0; i<right.size(); i++){
        Point p;
        p.first = distribution(randomEngine);
        p.second = distribution(randomEngine);
        rightPoints.push_back(p);
    }

    //assign preference values based on distance
    for(int i=0; i<left.size(); i++){
        for(int j=0; j<right.size(); j++){
            double dist = sqrt(pow((rightPoints[j].first - leftPoints[i].first),2)
                               + pow((rightPoints[j].second - leftPoints[i].second),2));

            //Since this is value based matching, we need to give a close distance a high preference value
            //This function scales the value between 0 and 1, as the maximum possible distance between two points in the
            //1x1 square space is the square root of two
            double proximityValue = (sqrt(2) - dist) / sqrt(2);

            //weight the value and assign
            proximityValue *= locationWeight;
            left[i].preferenceList[j] += proximityValue;
            right[j].preferenceList[i] += proximityValue;
        }
    }

}

void assignGlobalRankWeight(vector<NodeData> &left, vector<NodeData> &right, double rankWeight){
    //Std random number generation
    default_random_engine randomEngine;
    uniform_real_distribution<double> distribution(0.0, 1.0);

    //value left, assign right's preference weights
    for(int i=0; i<left.size(); i++){
        double globalRank = distribution(randomEngine);
        globalRank *= rankWeight;
        for(int j=0; j<right.size(); j++){
            right[j].preferenceList[i] += globalRank;
        }
    }

    //value right, assign left's preference weights
    for(int i=0; i<right.size(); i++){
        double globalRank = distribution(randomEngine);
        globalRank *= rankWeight;
        for(int j=0; j<left.size(); j++){
            left[j].preferenceList[i] += globalRank;
        }
    }
}

void assignRandomWeight(vector<NodeData> &left, vector<NodeData> &right, double randomWeight){

    default_random_engine randomEngine;
    uniform_real_distribution<double> distribution(-.5, .5);

    for(int i=0; i<left.size(); i++){
        for(int j=0; j<right.size(); j++){
            double epsilon = distribution(randomEngine);
            epsilon *= randomWeight;
            left[i].preferenceList[j] += epsilon;
        }
    }

    for(int i=0; i<right.size(); i++){
        for(int j=0; j<left.size(); j++){
            double epsilon = distribution(randomEngine);
            epsilon *= randomWeight;
            right[i].preferenceList[j] += epsilon;
        }
    }
}

void writeNodeData(ostream &out, vector<NodeData> &nodes){
    out << nodes.size() << endl;
    for(int i=0; i<nodes.size(); i++){
        out << nodes[i].listLength << " ";
        for(int j=0; j<nodes[i].listLength; j++){
            out << nodes[i].preferenceList[j] << " ";
        }
        out << endl;
    }
}

//Handles actual generation
Instance generateWeightedData(ostream &out, int lhsCount, int rhsCount, double locationWeight, double rankWeight, double randomWeight) {
    Instance I;
    vector<NodeData> left, right;

    //lhs
    for(int i=0; i<lhsCount; i++){
        Node n;
        n.size = 1;
        n.allocation = 1;
        I.lhsnodes.push_back(n);
    }
    //rhs nodes
    for(int i=0; i<rhsCount; i++){
        Node n;
        n.size = 1;
        n.allocation = 1;
        I.rhsnodes.push_back(n);
    }
    //edges
    for(int i=0; i<lhsCount; i++){
        for(int j=0; j<rhsCount; j++){
            Edge e;
            e.allocation = 1;
            e.capacity = 1;
            e.attrs["value"] = 0.0;
            e.start = i;
            e.end = j;
            I.edges.push_back(e);
        }
    }

    //Set up data structures
    for(int i=0; i<lhsCount; i++){
        NodeData m;
        m.init(lhsCount);
        left.push_back(m);

        NodeData w;
        w.init(lhsCount);
        right.push_back(w);
    }

    //Currently, these functions assume a symmetrical matching
    assignLocationWeight(left, right, locationWeight);
    assignGlobalRankWeight(left, right, rankWeight);
    assignRandomWeight(left, right, randomWeight);

    //Gets data from the left and right structures and assigns it to Instance
    for(int i=0; i<I.edges.size(); i++){
        int l = I.edges[i].start;
        int r = I.edges[i].end;

        I.edges[i].attrs["value"] = left[l].preferenceList[r];
    }

    /*
    print_instance(I);

    //Write out data
    writeNodeData(out, left);
    out << endl;
    writeNodeData(out, right);
    */

    return I;
}

//Arguments: <lhsCount> <rhsCount> <file output> <location weight> <global rank weight> <random weight>
int main(int argc, char *argv[]){

    if(!(argc == 4 || argc == 7 || argc == 3 || argc == 6)){
        cerr << "Usage: generateDataFile.o <lhsCount> <rhsCount> [<location weight> <global rank weight> <random weight>] <file output> \n";
        return 0;
    }

    srand(unsigned(time(0)));

    int lhsCount, rhsCount;
    lhsCount = atoi(argv[1]);
    rhsCount = atoi(argv[2]);

    ostream *out;
    out = &cout;

    ofstream fout;
    if(argc == 4 || argc == 7){
        fout.open(argv[argc-1]);
        out = &fout;
    }

    double locationWeight, globalRankWeight, randomWeight;
    if(argc == 7 || argc == 6){
        locationWeight = atof(argv[3]);
        globalRankWeight = atof(argv[4]);
        randomWeight = atof(argv[5]);
    } else {
        locationWeight = 1;
        globalRankWeight = 1;
        randomWeight = 1;
    }

    /*
    generateRandomData(out, lhsCount);
    out << endl;
    generateRandomData(out, lhsCount);
    */

    Instance I = generateWeightedData(*out, lhsCount, rhsCount, locationWeight,
        globalRankWeight, randomWeight);
    print_instance(I);

    //out.close();
    return 0;
}
