#ifndef ECHO_INSTANCE__h
#define ECHO_INSTANCE__h
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>

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


  bool operator<(const Edge& rhs) const{
    return value > rhs.value;
  }
};

struct Instance {
  vector<Node> lhsnodes;
  vector<Node> rhsnodes;
  vector<Edge> edges;
};

struct order_edge{
  bool operator()(const Edge &left, const Edge &right){
    if(left.start == right.start){
      return(left.end < right.end);
    }
    return left.start < right.start;
  }
};

Instance read_instance(void);
Instance read_csv_instance(string path);
void print_instance(Instance &I);
void print_instance_csv(Instance &I);
double get_value(Instance &I);
void order_edges(Instance &I);
vector<vector<double> > get_value_matrix(Instance &I);
vector<vector<unsigned int> > get_allocation_matrix(Instance& I);

#endif
