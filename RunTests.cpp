#include <iostream>
#include <fstream>
#include <string>
#include "HungarianAlgorithm.h"
#include "GreedyMatching.h"

using namespace std;

void printCost(double cost, double optimalCost, double greedyCost, ostream &out){
    double optimalRatio = cost / optimalCost;
    double greedyGap = greedyCost - optimalCost;
    double greedyImprove = (greedyGap - (cost - optimalCost)) / greedyGap;

    out << "Percent of optimal: " << optimalRatio * 100 << "%\n";
    out << "Percent improvement from greedy: " << greedyImprove * 100 << "%\n";
}

int main(int argc, char* argv[]){

    //Generate dataset output file
    string dataNameBase = "datasets/testData";
    int dataFileNum = 0;
    string dataFileName = dataNameBase.append(to_string(dataFileNum));

    while(std::ifstream(dataFileName)){
        dataFileNum++;
        dataFileName = dataNameBase.append(to_string(dataFileNum));
    }
    ofstream dataStream(dataFileName);

    //Setup log output
    string logNameBase = "logs/testLog";
    int logFileNum = 0;
    string logFileName = logNameBase.append(to_string(logFileNum));

    //As long as there is a file with the given file name
    while(std::ifstream(logFileName)){
        logFileNum++;
        logFileName = logNameBase.append(to_string(logFileNum));
    }

    ofstream logStream(logFileName);

    ostream &out = logStream;

    out << "Hungarian Algorithm(Optimal): " << endl;
    double optimalCost = runHungarian(argc, argv);
    out << "Total Cost: " << optimalCost << endl;

    out << "Greedy matching: " << endl;
    double greedyCost = runGreedy(argc, argv);
    out << "Total Cost: " << greedyCost << endl;

    printCost(28, optimalCost, greedyCost, out);

    return 0;
}
