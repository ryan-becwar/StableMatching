#include <iostream>
#include <fstream>

using namespace std;

int menCount, womenCount;
int menListLength, womenListLength;

typedef struct Person {
    int partner;
    int * preferences;
    int selectionIndex;

    Person(int preferenceListLength) {
        preferences = new int [preferenceListLength];
    }
} Person;

int matchMan(Person *men, Person *women, int manIndex) {
    for(int i=0; i<menListLength; i++) {

    }
}

void outputMatches(Person *group) {
    cout << "writing group" << endl;
    for(int i=0; i<menCount; i++) {
        cout << i;

        for(int j=0; j<menListLength; j++) {
            cout << group[i].preferences[j];
        }
        cout << endl;
    }
}

int main(int argc, char *argv[]) {

    Person *men;
    Person *women;

    //Read in data
    ifstream fin;
    fin.open(argv[1]);

    fin >> menCount >> menListLength;
    men = new Person(menCount);
    for(int i=0; i<menCount; i++) {
        Person *p = new Person(menListLength);
        for(int j=0; j<menListLength; j++) {
            fin >> p->preferences[j];
        }
        men[i] = *p;
    }

    cout << menCount << menListLength << endl;
    fin >> womenCount >> womenListLength;
    women = new Person(womenCount);
    for(int i=0; i<menCount; i++) {
        Person *p = new Person(womenListLength);
        for(int j=0; j<womenListLength; j++) {
            fin >> p->preferences[j];
        }
        women[i] = *p;
    }

    outputMatches(men);
    outputMatches(women);

    return 0;
}


