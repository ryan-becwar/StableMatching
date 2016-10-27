#include <iostream>
#include "regret_matching.h"
#include "PageRank.h"
#include "ordering_evaluator.h"
#include "GenerateDataFile.h"

#define N 100
#define INSTANCE_COUNT 100
#define NOISE_INCREMENT 0.1

/*
Writes out data in following column format:
N noise_level instance_num greedy_mean greedy_stdev pagerank_zscore regret_zscore
*/
int main(){
  //Instance I = read_instance();

  for(double noise = 0; noise <= 1.0; noise += NOISE_INCREMENT){
    for(unsigned int i=0; i<INSTANCE_COUNT; i++){
      Instance I = getGlobalMinInstance(N, noise);
      //Instance I = getLocationInstance(N, noise);
      OrderingEvaluator evaluator(I, N);
      vector<unsigned int> pagerankOrder = generate_pagerank_order(I);
      vector<unsigned int> regretOrder = regret_order(I);

      evaluator.evaluateOrder("pagerank", pagerankOrder);
      evaluator.evaluateOrder("regret", regretOrder);
      evaluator.printPlotData(noise, i);
    }
  }
}
