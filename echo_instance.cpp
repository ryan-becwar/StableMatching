#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include "echo_instance.h"

using namespace std;


Instance read_instance(void)
{
  Instance I;
  int Nlhs, Nrhs, Nedges;
  int ival;
  string s;
  double dval;

  cin >> Nlhs >> Nrhs >> Nedges >> s;

  for (int i=0; i<Nlhs; i++) {
    cin >> ival;
    Node n;
    while (cin >> s && s != "node") {
      if (s == "size") cin >> n.size;
      else if (s == "allocation") cin >> n.allocation;
      else { cin >> dval; n.attrs[s] = dval;
      }
    }
    I.lhsnodes.push_back(n);
  }

  for (int i=0; i<Nrhs; i++) {
    cin >> ival;
    Node n;
    while (cin >> s && s != "node" && s != "edge") {
      if (s == "size") cin >> n.size;
      else if (s == "allocation") cin >> n.allocation;
      else { cin >> dval; n.attrs[s] = dval; }
    }
    I.rhsnodes.push_back(n);
  }

  //readin edges
  for(int i=0; i<Nedges; i++) {
    Edge e;
    cin >> e.start >> e.end;
    while(cin >> s && s != "edge") {
      if (s == "size") cin >> e.size;
      else if (s == "allocation") cin >> e.allocation;
      else if (s == "value") cin >> e.value;
      else { cin >> dval; e.attrs[s] = dval; }
    }
    I.edges.push_back(e);
  }
  return I;
}

Instance read_csv_instance(string path){
  Instance I;
  std::ifstream file(path);
  vector<vector<double> > contents;
  std::string::size_type size;

  vector<double> lineResult;
  do {
    string line, cell;
    std::getline(file, line);
    std::stringstream lineStream(line);

    lineResult.clear();
    while(std::getline(lineStream, cell, ','))
      lineResult.push_back(std::stod(cell,&size));

    //event of trailing comma at end of line
    //if (!lineStream && cell.empty())
    //  lineResult.push_back("");
    if(!lineResult.empty())
      contents.push_back(lineResult);
  } while(!lineResult.empty());

  file.close();

  unsigned int Nlhs = contents.size();
  unsigned int Nrhs = contents[0].size();

  for(unsigned int i=0; i<Nlhs; i++){
    Node n;
    n.size = 1;
    n.allocation = 0;
    I.lhsnodes.push_back(n);
  }
  for(unsigned int i=0; i<Nrhs; i++){
    Node n;
    n.size = 1;
    n.allocation = 0;
    I.rhsnodes.push_back(n);
  }

  for(unsigned int i = 0; i<Nlhs; i++){
    for(unsigned int j=0; j<Nrhs; j++){
      Edge e;
      e.start = i;
      e.end = j;
      e.size = 1;
      e.allocation = 0;
      e.value = contents[i][j];

      I.edges.push_back(e);
    }
  }

  return I;
}

void print_instance(Instance &I)
{
  cout << I.lhsnodes.size() << " " << I.rhsnodes.size() << " " << I.edges.size() << "\n";
  for (unsigned int i=0; i<I.lhsnodes.size(); i++) {
    cout << "node " << i;
    cout << " size " << I.lhsnodes[i].size;
    cout << " allocation " << I.lhsnodes[i].allocation;
    for (msd::iterator it = I.lhsnodes[i].attrs.begin();
	 it != I.lhsnodes[i].attrs.end(); it++)
      cout << " " << it->first << " " << it->second;
    cout << "\n";
  }
  cout << endl;

  for (unsigned int i=0; i<I.rhsnodes.size(); i++) {
    cout << "node " << i;
    cout << " size " << I.rhsnodes[i].size;
    cout << " allocation " << I.rhsnodes[i].allocation;
    for (msd::iterator it = I.rhsnodes[i].attrs.begin();
	 it != I.rhsnodes[i].attrs.end(); it++)
      cout << " " << it->first << " " << it->second;
    cout << "\n";
  }
  cout << endl;

  for(unsigned int i=0; i<I.edges.size(); i++){
    cout << "edge " << I.edges[i].start << " " << I.edges[i].end;
    cout << " allocation " << I.edges[i].allocation;
    cout << " size " << I.edges[i].size;
    cout << " value " << I.edges[i].value;
    for (msd::iterator it = I.edges[i].attrs.begin();
      it != I.edges[i].attrs.end(); it++)
      cout << " " << it->first << " " << it->second;
    cout << endl;
  }

}

void print_instance_csv(Instance &I){
  vector<vector<double> > mat = get_value_matrix(I);

  for(unsigned int i=0; i<mat.size(); i++){
    for(unsigned int j=0; j<mat[i].size(); j++){
      cout << mat[i][j];
      if(j+1 < mat[i].size())
        cout << ",";
    }
    cout << endl;
  }
}

double get_value(Instance &I){
  double sum = 0;
  for(unsigned int i=0; i<I.edges.size(); i++){
    sum += I.edges[i].value * I.edges[i].allocation;
  }

  return sum;
}

void order_edges(Instance &I){
  std::sort(I.edges.begin(), I.edges.end(), order_edge());
}

//Returns the value attribute of all edges in the form of a lhs x rhs matrix
vector<vector<double> > get_value_matrix(Instance& I){
  //Initialize values matrix to correct size
  vector<vector<double> > values;
  for(unsigned int i=0; i< I.lhsnodes.size(); i++){
    vector<double> connectionCosts;
    connectionCosts.resize(I.rhsnodes.size());
    values.push_back(connectionCosts);
  }

  //Assign values to values matrix from Instance edges
  for(unsigned int i=0; i<I.edges.size(); i++){
    values[I.edges[i].start][I.edges[i].end] = I.edges[i].value;
	}

  return values;
}

vector<vector<unsigned int> > get_allocation_matrix(Instance& I){
  vector<vector<unsigned int> > values;
  for(unsigned int i = 0; i<I.lhsnodes.size(); i++){
    vector<unsigned int> rowValues;
    rowValues.resize(I.rhsnodes.size());
    values.push_back(rowValues);
  }

  for(unsigned int i=0; i<I.edges.size(); i++){
    values[I.edges[i].start][I.edges[i].end] = I.edges[i].allocation;
  }

  return values;
}
