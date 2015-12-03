#include <iostream>
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


    /*
     * Should Generate tests cases and then run them
     */
    //Change to file output as needed
    ostream &out = cout;

    out << "Hungarian Algorithm(Optimal): " << endl;
    double optimalCost = runHungarian(argc, argv);
    out << "Total Cost: " << optimalCost << endl;

    out << "Greedy matching: " << endl;
    double greedyCost = runGreedy(argc, argv);
    out << "Total Cost: " << greedyCost << endl;

    printCost(28, optimalCost, greedyCost, out);

    return 0;
}
