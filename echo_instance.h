#include <vector>
#include <iostream>
#include <map>
#include <string>

using namespace std;

typedef map<string, double> msd;

struct Node {
  int allocation;
  int size;
  msd attrs;
};

struct Edge {
  int start;
  int end;
  int allocation;
  int size;
  double value;
  msd attrs;


  bool operator<(const Edge& rhs){
    return value > rhs.value;
  }
};

struct Instance {
  vector<Node> lhsnodes;
  vector<Node> rhsnodes;
  vector<Edge> edges;
};

Instance read_instance(void);
void print_instance(Instance I);
double get_value(Instance I);