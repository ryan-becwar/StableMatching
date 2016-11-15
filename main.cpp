#include <iostream>
#include "regret_matching.h"
#include "PageRank.h"
#include "ordering_evaluator.h"
#include "GenerateDataFile.h"
#include "build_lp.h"

#define N 100
#define INSTANCE_COUNT 100
#define NOISE_INCREMENT 0.1

/*
Writes out data in following column format:
N noise_level instance_num greedy_mean greedy_stdev pagerank_zscore regret_zscore regret_regression_zscore optimal_value
*/
int main(){
  //Instance I = read_instance();

  for(double noise = 0; noise <= 1.0; noise += NOISE_INCREMENT){
    for(unsigned int i=0; i<INSTANCE_COUNT; i++){
      Instance I = getGlobalMinInstance(N, noise);
      //Instance I = getLocationInstance(N, noise);
      OrderingEvaluator evaluator(I, N);
      vector<unsigned int> pagerankOrder = generate_pagerank_order(I);
      vector<unsigned int> regretOrder = regret_projection_order(I);
      vector<unsigned int> regretRegressionOrder = regret_regression_order(I);
      //double optVal = lp_opt_result(N, get_value_matrix(I));
      double optVal = 0;

      evaluator.evaluateOrder("pagerank", pagerankOrder);
      evaluator.evaluateOrder("regret", regretOrder);
      evaluator.evaluateOrder("regretRegression", regretRegressionOrder);
      evaluator.printPlotData(noise, i, optVal);
    }
  }
}
