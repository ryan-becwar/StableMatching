#include "generate_data_file.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cmath>

using namespace std;

typedef pair<double, double> Point;

typedef struct Person {
    int listLength;
    vector<double> preferenceList;

    void init(int listLength){
        this->listLength = listLength;
        for(int i=0; i<listLength; i++){
            preferenceList.push_back(0.0);
        }
    }
} Person;

void generateRandomData(ofstream &out, int personCount){
    out << personCount << endl;
    for(int i=0; i<personCount; i++) {

        //Build random allocation size
        int size = (rand() % personCount) + 1;

        //Build random preference list
        vector<int> prefList;
        for(int k=0; k <personCount; k++)
            prefList.push_back(k);
        random_shuffle(prefList.begin(), prefList.end());

        //write out data for line
        out << size << " ";
        for(int j=0; j <personCount; j++) {
            out << prefList[j] << " ";
        }
        out << endl;
    }
}


void assignLocationWeight(vector<Person> &men, vector<Person> &women, double locationWeight){
    vector<Point> menPoints;
    vector<Point> womenPoints;
    //Std random number generation
    default_random_engine randomEngine;
    uniform_real_distribution<double> distribution(0.0, 1.0);

    //Assign random points
    for(int i=0; i<men.size(); i++){
        Point p;
        p.first = distribution(randomEngine);
        p.second = distribution(randomEngine);
        menPoints.push_back(p);
    }

    for(int i=0; i<women.size(); i++){
        Point p;
        p.first = distribution(randomEngine);
        p.second = distribution(randomEngine);
        womenPoints.push_back(p);
    }

    //assign preference values based on distance
    for(int i=0; i<men.size(); i++){
        for(int j=0; j<women.size(); j++){
            double dist = sqrt(pow((womenPoints[j].first - menPoints[i].first),2)
                               + pow((womenPoints[j].second - menPoints[i].second),2));

            //Since this is value based matching, we need to give a close distance a high preference value
            //This function scales the value between 0 and 1, as the maximum possible distance between two points in the
            //1x1 square space is the square root of two
            double proximityValue = (sqrt(2) - dist) / sqrt(2);

            //weight the value and assign
            proximityValue *= locationWeight;
            men[i].preferenceList[j] += proximityValue;
            women[j].preferenceList[i] += proximityValue;
        }
    }

}

void assignGlobalRankWeight(vector<Person> &men, vector<Person> &women, double rankWeight){
    //Std random number generation
    default_random_engine randomEngine;
    uniform_real_distribution<double> distribution(0.0, 1.0);

    //value men, assign women's preference weights
    for(int i=0; i<men.size(); i++){
        double globalRank = distribution(randomEngine);
        globalRank *= rankWeight;
        for(int j=0; j<women.size(); j++){
            women[j].preferenceList[i] += globalRank;
        }
    }

    //value women, assign men's preference weights
    for(int i=0; i<women.size(); i++){
        double globalRank = distribution(randomEngine);
        globalRank *= rankWeight;
        for(int j=0; j<men.size(); j++){
            men[j].preferenceList[i] += globalRank;
        }
    }
}

void assignRandomWeight(vector<Person> &men, vector<Person> &women, double randomWeight){

    default_random_engine randomEngine;
    uniform_real_distribution<double> distribution(-.5, .5);

    for(int i=0; i<men.size(); i++){
        for(int j=0; j<women.size(); j++){
            double epsilon = distribution(randomEngine);
            epsilon *= randomWeight;
            men[i].preferenceList[j] += epsilon;
        }
    }

    for(int i=0; i<women.size(); i++){
        for(int j=0; j<men.size(); j++){
            double epsilon = distribution(randomEngine);
            epsilon *= randomWeight;
            women[i].preferenceList[j] += epsilon;
        }
    }
}

void writePersonListData(ofstream &out, vector<Person> &people){
    out << people.size() << endl;
    for(int i=0; i<people.size(); i++){
        out << people[i].listLength << " ";
        for(int j=0; j<people[i].listLength; j++){
            out << people[i].preferenceList[j] << " ";
        }
        out << endl;
    }
}

void generate_weighted_data(ofstream &out, int personCount, double locationWeight, double rankWeight, double randomWeight) {
    vector<Person> men;
    vector<Person> women;

    //Set up lists
    for(int i=0; i<personCount; i++){
        Person m;
        m.init(personCount);
        men.push_back(m);

        Person w;
        w.init(personCount);
        women.push_back(w);
    }

    assignLocationWeight(men, women, locationWeight);
    assignGlobalRankWeight(men, women, rankWeight);
    assignRandomWeight(men, women, randomWeight);

    //Write out data
    writePersonListData(out, men);
    out << endl;
    writePersonListData(out, women);

}

//Arguments: <personCount> <file output> <location weight> <global rank weight> <random weight>
int main(int argc, char *argv[]){

    if(!(argc == 3 || argc == 6)){
        cerr << "Usage: generateDataFile.o <personCount> <file output> [<location weight> <global rank weight> <random weight>]\n";
        return 0;
    }

    srand(unsigned(time(0)));

    int personCount;
    personCount = atoi(argv[1]);

    ofstream out;
    out.open(argv[2]);

    double locationWeight, globalRankWeight, randomWeight;
    if(argc == 6){
        locationWeight = atof(argv[3]);
        globalRankWeight = atof(argv[4]);
        randomWeight = atof(argv[5]);
    } else {
        locationWeight = 1;
        globalRankWeight = 1;
        randomWeight = 1;
    }

    /*
    generateRandomData(out, personCount);
    out << endl;
    generateRandomData(out, personCount);
    */

    generate_weighted_data(out, personCount, locationWeight, globalRankWeight, randomWeight);

    out.close();
    return 0;
}
