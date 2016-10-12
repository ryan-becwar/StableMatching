#include "ordering_evaluator.h"

OrderingEvaluator::OrderingEvaluator(Instance& I, unsigned int greedyCount) : I(I), greedyCount(greedyCount), greedyResults() {
  generateGreedyResults();
}

void OrderingEvaluator::generateGreedyResults(){

}

void OrderingEvaluator::evaluateOrder(vector<unsigned int> const& order){
  std::cout<< "order" << std::endl;
}
