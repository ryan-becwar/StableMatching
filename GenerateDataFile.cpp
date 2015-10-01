#include "GenerateDataFile.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>

using namespace std;

void generateData(ofstream& out, int personCount){
    out << personCount << " " << personCount << endl;
    for(int i=0; i<personCount; i++) {

        //Build random allocation size
        int size = (rand() % personCount) + 1;

        //Build random preference list
        vector<int> prefList;
        for(int i=0; i<personCount; i++)
            prefList.push_back(i);
        random_shuffle(prefList.begin(), prefList.end());

        //write out data for line
        out << size << " ";
        for(int i=0; i<personCount; i++) {
            out << prefList[i] << " ";
        }
        out << endl;
    }
}

//Arguments: <personCount> <file output>
int main(int argc, char *argv[]){

    srand(unsigned(time(0)));

    int personCount;
    personCount = atoi(argv[1]);

    ofstream out;
    out.open(argv[2]);

    generateData(out, personCount);
    out << endl;
    generateData(out, personCount);

    out.close();
    return 0;
}
