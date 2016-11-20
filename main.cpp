#include <iostream>
#include "regret_matching.h"
#include "pagerank.h"
#include "ordering_evaluator.h"
#include "generate_data_file.h"
#include "build_lp.h"
#include "greedy_matching.h"

#define N 100
#define INSTANCE_COUNT 100
#define NOISE_INCREMENT 0.1

/*
Writes out data in following column format:
N noise_level instance_num greedy_mean greedy_stdev pagerank_zscore regret_zscore regret_regression_zscore global_greedy_value optimal_value
*/
int main(){
  //Instance I = read_instance();

  for(double noise = 0; noise <= 1.0; noise += NOISE_INCREMENT){
    for(unsigned int i=0; i<INSTANCE_COUNT; i++){
      Instance I = get_global_min_instance(N, noise);
      //Instance I = get_location_instance(N, noise);
      ordering_evaluator evaluator(I, N);
      vector<unsigned int> pagerankOrder = generate_pagerank_order(I);
      vector<unsigned int> regretOrder = regret_projection_order(I);
      vector<unsigned int> regretRegressionOrder = regret_regression_order(I);
      //double globalGreedyVal = global_greedy_value(I);
      double globalGreedyVal = 0;
      //double optVal = lp_opt_result(N, get_value_matrix(I));
      double optVal = 0;

      evaluator.evaluate_order("pagerank", pagerankOrder);
      evaluator.evaluate_order("regret", regretOrder);
      evaluator.evaluate_order("regretRegression", regretRegressionOrder);
      evaluator.print_plot_data(noise, i, globalGreedyVal, optVal);
    }
  }
}
