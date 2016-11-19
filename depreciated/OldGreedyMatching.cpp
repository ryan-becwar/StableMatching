#include "greedy_matching.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

typedef pair<int, int> pii;

typedef struct Matching {
    int man;
    int woman;
    double cost;

    bool operator<(const Matching& rhs){
        return cost < rhs.cost;
    }
} Matching;


double runGreedyLong(int argc, char *argv[]) {

    unsigned long personCount;
    vector<Matching> matches;
    vector<Matching> finalMatches;

    //Reading in data
    ifstream fin;
    fin.open(argv[1]);

    fin >> personCount;
    for(int i=0; i<personCount; i++){
        int connectionCount;
        fin >> connectionCount;
        for(int j=0; j<connectionCount; j++){
            Matching match = Matching();
            match.man = i;
            match.woman = j;
            fin >> match.cost;
            matches.push_back(match);
        }
    }


    //Sort the list of matches by their cost.  Operator overloading handles this
    sort(matches.begin(), matches.end());

    //To keep track of already matched individuals
    vector<bool> menFree(personCount, false);
    vector<bool> womenFree(personCount, false);
    for(int i=0; i<matches.size(); i++){
        if(!menFree[matches[i].man] && !womenFree[matches[i].woman]){
            menFree[matches[i].man] = true;
            womenFree[matches[i].woman] = true;
            finalMatches.push_back(matches[i]);
        }
    }

    double totalCost = 0;
    for(int i=0; i<finalMatches.size(); i++){
        totalCost += finalMatches[i].cost;
        cout << finalMatches[i].man << " " << finalMatches[i].woman << " ";
        cout << finalMatches[i].cost << endl;
    }
    //cout << "Total Cost: " << totalCost << endl << "Average Costs: " << totalCost / finalMatches.size() << endl;

    return totalCost;
}

double runGreedy(string dataFile) {

    unsigned long personCount;
    vector<Matching> matches;
    vector<Matching> finalMatches;

    //Reading in data
    ifstream fin;
    fin.open(dataFile);

    fin >> personCount;
    for(int i=0; i<personCount; i++){
        int connectionCount;
        fin >> connectionCount;
        for(int j=0; j<connectionCount; j++){
            Matching match = Matching();
            match.man = i;
            match.woman = j;
            fin >> match.cost;
            matches.push_back(match);
        }
    }


    //Sort the list of matches by their cost.  Operator overloading handles this
    sort(matches.begin(), matches.end());

    //To keep track of already matched individuals
    vector<bool> menFree(personCount, false);
    vector<bool> womenFree(personCount, false);
    for(int i=0; i<matches.size(); i++){
        if(!menFree[matches[i].man] && !womenFree[matches[i].woman]){
            menFree[matches[i].man] = true;
            womenFree[matches[i].woman] = true;
            finalMatches.push_back(matches[i]);
        }
    }

    double totalCost = 0;
    for(int i=0; i<finalMatches.size(); i++){
        totalCost += finalMatches[i].cost;
        cout << finalMatches[i].man << " " << finalMatches[i].woman << " ";
        cout << finalMatches[i].cost << endl;
    }
    //cout << "Total Cost: " << totalCost << endl << "Average Costs: " << totalCost / finalMatches.size() << endl;

    return totalCost;
}