#include "matching_utilities.h"


class OrderingEvaluator {
public:
  OrderingEvaluator(Instance& I, unsigned int greedyCount);

  void generateGreedyResults();
  void evaluateOrder(vector<unsigned int> const& order);

private:
  Instance &I;
  unsigned int greedyCount;
  vector<double> greedyResults;

};
