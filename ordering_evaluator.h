#include "matching_utilities.h"


class OrderingEvaluator {
public:
  OrderingEvaluator(Instance& I, unsigned int greedyCount);

  void evaluateOrder(std::string title, vector<unsigned int> const& order);
  void printPlotData(double noise, unsigned int instanceNum, double optVal);

  static default_random_engine& getRandomEngine(){
    static default_random_engine engine;
    return engine;
  }

private:
  void generateGreedyResults();

  Instance &I;
  unsigned int greedyCount;
  vector<double> greedyResults;
  double greedyMean;
  double greedyStdev;

  std::map<std::string, double> zScores;

  vector<vector<double> > values;
  unsigned long width;
};
