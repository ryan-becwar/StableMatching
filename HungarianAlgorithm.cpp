#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <limits>
#include <algorithm>
#include <set>
#include <utility>
#include <iterator>

using namespace std;
struct Node;
const double maxDist = numeric_limits<double>::infinity();

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

Node *UNASSIGNED = new Node();


void dijkstraComputePaths(Node *source, vector<Node *> nodes){
    int n = nodes.size();

    //reset minimum distance and previous node
    for(int i=0; i<n; i++){
        nodes[i]->dist = maxDist;
        nodes[i]->previous = UNASSIGNED;
    }
    source->dist = 0;

    //Que for adgacent previously unvisited nodes
    set<pair<double, Node*>> nodeQueue;
    nodeQueue.insert(make_pair(source->dist, source));

    while(!nodeQueue.empty()){
        double dist = nodeQueue.begin()->first;
        Node * u = nodeQueue.begin()->second;
        nodeQueue.erase(nodeQueue.begin());

        //for each edge exiting u
        vector<Edge *> &edges = u->edges;
        for(int i=0; i<edges.size(); i++){
            Node *v = edges[i]->p;
            double cost = edges[i]->cost;
            double distThroughU = dist + cost;
            if(distThroughU < v->dist){
                nodeQueue.erase(make_pair(v->dist, v));
                v->dist = distThroughU;
                v->previous = u;
                nodeQueue.insert(make_pair(v->dist, v));
            }
        }
    }

}

list<Node *> findShortestPath(Node *target, vector<Node *> nodes){
    list<Node *> path;
    for(;target != UNASSIGNED && target != NULL; target = target->previous){
        path.push_front(target);
    }
    return path;
}

void reversePaths(list<Node *> path, vector<Node *> nodes){

}
int main(int argc, char *argv[]){

    //Readin Data
    vector<vector<double>> costs;
    int nodeCount;

    ifstream fin;
    fin.open(argv[1]);

    fin >> nodeCount;

    //Read in cost table
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
    Node end = Node();
    Node start = Node();
    vector<Node> men;
    vector<Node> women;

    //Declare and add women nodes
    for(int i=0; i<nodeCount; i++){
        Node woman = Node();
        woman.edges.push_back(new Edge(&end, 0));
        women.push_back(woman);
    }

    //Declare and add man nodes
    for(int i=0; i<nodeCount; i++){
        Node man = Node();
        for(int j=0; j<nodeCount; j++){
            man.edges.push_back(new Edge(&women[j], costs[i][j]));
        }
        men.push_back(man);
        start.edges.push_back(new Edge(&man, 0));
    }

    vector<Node*> nodeList;
    nodeList.push_back(&start);
    for(int i=0; i<nodeCount; i++){
        nodeList.push_back(&men[i]);
    }
    for(int i=0; i<nodeCount; i++){
        nodeList.push_back(&women[i]);
    }
    nodeList.push_back(&end);

    dijkstraComputePaths(nodeList[0], nodeList);

    for(int i=0; i<nodeList.size(); i++){
        cout << i << ": " << nodeList[i]->dist << endl;
    }

    list<Node *> shortestPath = findShortestPath(&end, nodeList);



    return 0;
}