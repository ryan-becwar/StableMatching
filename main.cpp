#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include "regret_matching.h"
#include "pagerank.h"
#include "ordering_evaluator.h"
#include "generate_data_file.h"
#include "build_lp.h"
#include "greedy_matching.h"

#define N 100
#define INSTANCE_COUNT 5
#define NOISE_TIERS 10
#define GREEDYCOUNT 10

/*
USAGE:
./main [-opt] [<input_data_path.csv>]
If optimal flag is present, the program will attempt to calculate the optimal value via CPLEX.  If CPLEX is not installed, the program will fail.
If no input data path is supplied, the program will generate data sets independently.

Writes out data in following column format:
N noise_level instance_num greedy_mean greedy_stdev pagerank_zscore regret_zscore regret_regression_zscore optimal_zscore
*/

string format_algorithm_output(Instance I, vector<unsigned int> order, string outPath){
  write_matches(I, find_matches(get_value_matrix(I), order, I.rhsnodes.size()));
  double result = get_value(I);
  vector<vector<unsigned int> > matches = get_allocation_matrix(I);

  stringstream ss;
  ss << result << endl;
  for(unsigned int i=0; i<order.size(); i++){
    ss << order[i];
    if(i+1 != order.size())
      ss << ", ";
  }
  ss << endl;

  for(unsigned int i=0; i<matches.size(); i++){
    for(unsigned int j=0; j<matches[i].size(); j++){
      ss << matches[i][j];
      if(i+1 != matches[i].size())
        ss << ", ";
    }
    ss << endl;
  }

  std::ofstream out;
  out.open(outPath);
  out << ss.rdbuf();
  out.close();

  return ss.str();
}

string format_algorithm_output_opt(vector<vector<double> > mat, double result, string outPath){
  vector<vector<unsigned int> > matches;
  vector<unsigned int> order;

  for(unsigned int i=0; i<mat.size(); i++){
    vector<unsigned int> row;
    for(unsigned int j=0; j<mat[0].size(); j++){
      row.push_back(0);
    }
    matches.push_back(row);
    order.push_back(i);
  }

  ifstream results("results.txt");
  for (int i=0; i<min(L,R); i++) {
    int a, b;
    double amt;
    results >> a >> b >> amt;
    matches[a][b] = 1;
    //    cout << amt << " assigned from " << a << " to " << b << "\n";
  }
  results.close();


  stringstream ss;
  ss << result << endl;
  for(unsigned int i=0; i<order.size(); i++){
    ss << order[i];
    if(i+1 != order.size())
      ss << ", ";
  }
  ss << endl;

  for(unsigned int i=0; i<matches.size(); i++){
    for(unsigned int j=0; j<matches[i].size(); j++){
      ss << matches[i][j];
      if(i+1 != matches[i].size())
        ss << ", ";
    }
    ss << endl;
  }

}

//Reads in a csv with a predefined data set and evaluates algorithms on it
void process_real_data(string path, bool opt){
  Instance I = read_csv_instance(path);
  ordering_evaluator evaluator(I, GREEDYCOUNT);
  vector<unsigned int> pagerankOrder = generate_pagerank_order(I);
  vector<unsigned int> regretOrder = regret_projection_order(I);
  vector<unsigned int> regretRegressionOrder = regret_projection_order(I);
  vector<unsigned int> globalGreedyOrder = global_greedy_order(I);
  //double globalGreedyVal = global_greedy_value(I);
  //double globalGreedyVal = 0;
  string outDir = "output/kaiser/";

  double optVal;
  if(opt){
    optVal = lp_opt_result(I.lhsnodes.size(), I.rhsnodes.size(), get_value_matrix(I));
    format_algorithm_output_opt(get_value_matrix(I), optVal, outDir + "optimal.csv");
  } else {
    optVal = 0;
  }

  format_algorithm_output(I, regretOrder, outDir + "regret.csv");
  format_algorithm_output(I, pagerankOrder, outDir + "pagerank.csv");
  format_algorithm_output(I, globalGreedyOrder, outDir + "greedy.csv");

  evaluator.evaluate_order("pagerank", pagerankOrder);
  evaluator.evaluate_order("regret", regretOrder);
  evaluator.evaluate_order("regretRegression", regretRegressionOrder);
  evaluator.evaluate_order("globalGreedy", globalGreedyOrder);
  evaluator.evaluate_value("optimal", optVal);

  evaluator.print_plot_data(0.0, 0);
}

int main(int argc, char *argv[]){
  //Instance I = read_instance();

  bool opt = false, realData = false;
  string realPath;

  for(int i=1; i<argc; i++){
    if(argv[i][0] == '-'){
      if(strcmp(argv[i], "-opt") == 0){
        opt = true;
      }
    } else {
      realData = true;
      realPath = argv[i];
    }
  }

  if(realData){
    process_real_data(realPath, opt);
  } else {
    std::cout << "width,noise,instance number, greedy mean,greedy stdev,pagerank_value,regret_value,optimal value\n";

    for(double noise = 0; noise <= 1.0; noise += 1.0/NOISE_TIERS){
      for(unsigned int i=0; i<INSTANCE_COUNT; i++){
        //Instance I = get_global_min_instance(N, N, noise);
        Instance I = get_location_instance(N, N, noise);
        ordering_evaluator evaluator(I, GREEDYCOUNT);
        vector<unsigned int> pagerankOrder = generate_pagerank_order(I);
        vector<unsigned int> regretOrder = regret_projection_order(I);
        vector<unsigned int> regretRegressionOrder = regret_projection_order(I);
        vector<unsigned int> globalGreedyOrder = global_greedy_order(I);
        //double globalGreedyVal = global_greedy_value(I);
        //double globalGreedyVal = 0;

        double optVal;
        if(opt){
          optVal = lp_opt_result(N, N, get_value_matrix(I));
        } else {
          optVal = 0;
        }


        //Grabbing learning values psuedocode:
        /*
        Write instance to file

        system call (python < instance > order.txt)

        read order.txt into vector<unsigned int>

        evaluator,evaluater_order("learning", learningOrder);
        */

        evaluator.evaluate_order("pagerank", pagerankOrder);
        evaluator.evaluate_order("regret", regretOrder);
        evaluator.evaluate_order("regretRegression", regretRegressionOrder);
        evaluator.evaluate_order("globalGreedy", globalGreedyOrder);
        evaluator.evaluate_value("optimal", optVal);

        evaluator.print_plot_data(noise, i);
      }
    }
  }

  //Instance I = get_global_min_instance(15, 60, 0.2);
  //print_instance_csv(I);


}
