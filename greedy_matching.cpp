#include "greedy_matching.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "echo_instance.h"

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


void greedy(Instance I){
    cout << "Greedy Matching" << endl;

    sort(I.edges.begin(), I.edges.end());

    for(unsigned int i=0; i<I.edges.size(); i++){
        int start = I.edges[i].start;
        int end = I.edges[i].end;
        while(I.lhsnodes[start].allocation < I.lhsnodes[start].size
            && I.rhsnodes[end].allocation < I.rhsnodes[end].size
            && I.edges[i].allocation < I.edges[i].size){
            I.lhsnodes[start].allocation++;
            I.rhsnodes[end].allocation++;
            I.edges[i].allocation++;
        }
    }

    order_edges(I);
    print_instance(I);

    cout << "value: " << get_value(I) << endl;
}

int main(int argc, char* argv[]){
  Instance I = read_instance();

  greedy(I);

  return 0;
}
