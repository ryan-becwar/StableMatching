#include <vector>
#include <iostream>
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
      //DEBUG
      cerr << "s: " << s << endl;

  for (int i=0; i<Nlhs; i++) {
    cin >> ival;
    //DEBUG
    cerr << "ival: " << ival << endl;
    Node n;
    while (cin >> s && s != "node") {
      //DEBUG
      cerr << "s: " << s << endl;
      if (s == "size") cin >> n.size;
      else if (s == "allocation") cin >> n.allocation;
      else { cin >> dval; n.attrs[s] = dval;
        //DEBUG
        cerr << "s: " << s << "dval: " << dval << endl;
      }
    }
    //DEBUG
    cerr << "node " << i << " done\n";
    I.lhsnodes.push_back(n);    
  }

  for (int i=0; i<Nrhs; i++) {
    cin >> ival;
    Node n;
    while (cin >> s && s != "node") {
      if (s == "size") cin >> n.size;
      else if (s == "allocation") cin >> n.allocation;
      else { cin >> dval; n.attrs[s] = dval; }
      }
    I.rhsnodes.push_back(n);    
  }

  //readin edges
  for(int i=0; i<Nedges; i++) {

  }
  return I;
}

void print_instance(Instance I)
{
  cout << I.lhsnodes.size() << "\n";
  for (int i=0; i<I.lhsnodes.size(); i++) {
    cout << "node " << i;
    cout << " size " << I.lhsnodes[i].size;
    cout << " allocation " << I.lhsnodes[i].allocation;
    for (msd::iterator it = I.lhsnodes[i].attrs.begin(); 
	 it != I.lhsnodes[i].attrs.end(); it++)
      cout << " " << it->first << " " << it->second;
    cout << "\n";
  }

  cout << I.rhsnodes.size() << "\n";
  for (int i=0; i<I.rhsnodes.size(); i++) {
    cout << "node " << i;
    cout << " size " << I.rhsnodes[i].size;
    cout << " allocation " << I.rhsnodes[i].allocation;
    for (msd::iterator it = I.rhsnodes[i].attrs.begin(); 
	 it != I.rhsnodes[i].attrs.end(); it++)
      cout << " " << it->first << " " << it->second;
    cout << "\n";
  }

  cout << I.edges.size() << "\n";
  for(int i=0; i<I.edges.size(); i++){
    cout << "edge" << endl;
  }

}

int main(){
  Instance I = read_instance();
  print_instance(I);
}