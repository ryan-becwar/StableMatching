#include <iostream>
#include "HungarianAlgorithm.h"
#include "GreedyMatching.h"

using namespace std;

int main(int argc, char* argv[]){

    //Change to file output as needed
    ostream &out = cout;

    out << "Greedy matching: " << endl;
    runGreedy(argc, argv);

    out << "Hungarian Algorithm(Optimal): " << endl;
    runHungarian(argc, argv);

    return 0;
}
