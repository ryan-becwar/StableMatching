#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int L, R;
double **cost;

int main(void)
{
  int n;
  cin >> n;
  L = R = n;
  
  cost = new double *[L];
  for (int i=0; i<L; i++) {
    cost[i] = new double[R];
    cin >> n;
    for (int j=0; j<R; j++) cin >> cost[i][j];
  }

  ofstream fout ("foo.lp");
  
  fout << "Minimize";
  for (int i=0; i<L; i++)
    for (int j=0; j<R; j++)
      fout << ((cost[i][j]>=0) ? " +" : " ")
	   << cost[i][j] << "x" << i << "x" << j;
  fout << "\nSubject to:\n";

  for (int i=0; i<L; i++) {
    for (int j=0; j<R; j++)
      fout << ((j>0)?" +":" ") << "x" << i << "x" << j;
    fout << " = 1\n";
  }
  
  for (int j=0; j<R; j++) {
    for (int i=0; i<L; i++) 
      fout << ((i>0)?" +":" ") << "x" << i << "x" << j;
    fout << " = 1\n";
  }
  
  fout << "End\n";

  fout.close();

  ofstream cmdout ("commands.txt");
  cmdout << "read foo.lp\n";
  cmdout << "opt\n";
  cmdout << "display solution variables x*\n";
  cmdout.close();

  system ("cplex < commands.txt | grep '^x' | tr 'x' ' ' > results.txt");
  
  ifstream results("results.txt");
  double total = 0.0;
  for (int i=0; i<n; i++) {
    int a, b;
    double amt;
    results >> a >> b >> amt;
    //    cout << amt << " assigned from " << a << " to " << b << "\n";
    total += amt * cost[a][b];
  }
  results.close();

  cout << "Total cost of optimal solution: " << total << "\n";

  return 0;
}
