#include "HungarianAlgorithm.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;
struct Node;

typedef struct Edge {
    Node *p;
    double cost;

    Edge(Node *node, double edgeCost){
         p = node;
         cost = edgeCost;
    }
}Edge;

typedef struct Node {
    double dist;
    Node *previous;
    vector<Edge *> edges;
}Node;

void dijkstraComputePaths(){

}

int main(int argc, char *argv[]){

    //Readin Data
    vector<vector<double>> costs;
    int nodeCount;

    ifstream fin;
    fin.open(argv[1]);

    fin >> nodeCount;

    for(int i=0; i<nodeCount; i++){
        int connectionCount;
        fin >> connectionCount;
        vector<double> connectionCosts;
        for(int j=0; j<connectionCount; j++){
            double cost;
            fin >> cost;
            connectionCosts.push_back(cost);
        }
        costs.push_back(connectionCosts);
    }

    //Build graph structure
    Node end;
    Node start;
    vector<Node> men;
    vector<Node> women;

    //Declare and add women nodes
    for(int i=0; i<nodeCount; i++){
        Node woman;
        woman.edges.push_back(new Edge(&end, 0));
        women.push_back(woman);
    }

    //Declare and add man nodes
    for(int i=0; i<nodeCount; i++){
        Node man;
        for(int j=0; j<nodeCount; j++){
            man.edges.push_back(new Edge(&women[j], costs[i][j]));
        }
        men.push_back(man);
        start.edges.push_back(new Edge(&man, 0));
    }




    return 0;
}