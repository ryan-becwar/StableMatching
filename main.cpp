#include <iostream>
#include "regret_matching.h"
#include "PageRank.h"
#include "ordering_evaluator.h"

int main(){
  Instance I = read_instance();

  OrderingEvaluator evaluator(I, 100);

  vector<unsigned int> pagerankOrder = generate_pagerank_order(I);
  vector<unsigned int> regretOrder = regret_order(I);

  evaluator.evaluateOrder("pagerank", pagerankOrder);
  evaluator.evaluateOrder("regret", regretOrder);
  evaluator.printPlotData(0.1, 0);
}
