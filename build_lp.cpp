#include "build_lp.h"
#include <algorithm>

double **cost;

double lp_opt_result(int l, int r, vector<vector< double> > cost)
{
  int L = l, R = r;

  ofstream fout ("foo.lp");

  fout << "Maximize";
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
  for (int i=0; i<min(L,R); i++) {
    int a, b;
    double amt;
    results >> a >> b >> amt;
    //    cout << amt << " assigned from " << a << " to " << b << "\n";
    total += amt * cost[a][b];
  }
  results.close();

  return total;
}
