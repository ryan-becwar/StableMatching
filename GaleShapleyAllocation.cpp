#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void gsSelect(int manId);
bool gsPropose(int manId, int womanId);
void writeData(ostream& out);

class Person{
    public:
        vector<int> partners;
        vector<int> preferences;
        //number of matches the person can handle
        int size;
        //Current point of stepping down a man's preference list
        int selectionIndex;
        //state if a man has been matched in his current search
        bool matched;

        Person() {
            selectionIndex = 0;

        }

//Initialize the partner list to impossible undesirable matches
        void initializeMatches() {
            for(int i=0; i<size; i++){
                partners.push_back(preferences.size() + 1);
            }
        }

        void printData(ostream& out) {
            out << "Size: " << size << endl;
            out << "preferenes: ";
            for(int i=0; i<preferences.size(); i++){
                out << preferences[i] << " ";
            }
            out << endl << "matches: ";
            for(int i=0; i<partners.size(); i++){
                out << partners[i] << " ";
            }
            out << endl;
        }
};

vector<Person> readIn(ifstream& fin);

//Making these global is messy, but greatly simplifies references
vector<Person> men;
vector<Person> women;



int main(int argc, char *argv[]) {



    //Read in data
    ifstream fin;
    fin.open(argv[1]);

    men = readIn(fin);
    women = readIn(fin);


    //write out starting data
    writeData(cout);

    //Match men and women
    for(int i=0; i<men.size(); i++){
        gsSelect(i);
    }

    cout << endl << "After matching:" << endl;
    writeData(cout);

    return 0;
}

//Writes all data for all men and women
void writeData(ostream& out) {
    out << men.size() << endl;
    for(int i=0; i<men.size(); i++){
        out << "man " << i << ": " << endl;
        men[i].printData(out);
    }

    out << women.size() << endl;
    for(int i=0; i<women.size(); i++){
        out << "woman " << i << ": " << endl;
        women[i].printData(out);
    }
}

//Reads in one half of the bipartite graph of people from the given input stream
vector<Person> readIn(ifstream& fin) {
    vector<Person> people;
    int personCount, preferenceListLength;
    fin >> personCount >> preferenceListLength;
    for(int i=0; i<personCount; i++) {
        Person p;
        fin >> p.size;
        for(int j=0; j <preferenceListLength; j++){
            int preference;
            fin >> preference;
            p.preferences.push_back(preference);
        }
        p.initializeMatches();
        people.push_back(p);
    }

    return people;
}

//matching function for men, for single matching
void gsSelect(int manId){
    for(;men[manId].selectionIndex<women.size() && !men[manId].matched; men[manId].selectionIndex++) {
        if(gsPropose(manId, men[manId].selectionIndex)){
            men[manId].partners[0] = men[manId].selectionIndex;
            men[manId].matched = true;
        }
    }
}

//decision function for women, for single matching
bool gsPropose(int manId, int womanId) {
    int proposalRank = men.size()+1;
    int currentRank = women[womanId].partners[0];
    for(int i=0; i<women[womanId].preferences.size(); i++){
        if(manId == women[womanId].preferences[i]){
            proposalRank = i;
        }
    }
    if(proposalRank < currentRank) {
        women[womanId].partners[0] = manId;
        gsSelect(currentRank);
        return true;
    } else {
        return false;
    }
}