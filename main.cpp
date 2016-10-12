#include <iostream>
#include "regret_matching.h"
#include "PageRank.h"
#include "ordering_evaluator.h"

int main(){
  Instance I = read_instance();

  OrderingEvaluator evaluator(I, 100);

  vector<unsigned int> testOrder(100);

  vector<unsigned int> regretOrder = regret_order(I);
  evaluator.evaluateOrder(regretOrder);

  vector<unsigned int> pagerankOrder = generate_pagerank_order(I);
  evaluator.evaluateOrder(pagerankOrder);
}
