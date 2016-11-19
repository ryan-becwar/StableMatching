#include "matching_utilities.h"


class ordering_evaluator {
public:
  ordering_evaluator(Instance& I, unsigned int greedyCount);

  void evaluate_order(std::string title, vector<unsigned int> const& order);
  void print_plot_data(double noise, unsigned int instanceNum, double optVal);

  static default_random_engine& get_random_engine(){
    static default_random_engine engine;
    return engine;
  }

private:
  void generate_greedy_results();

  Instance &I;
  unsigned int greedyCount;
  vector<double> greedyResults;
  double greedyMean;
  double greedyStdev;

  std::map<std::string, double> zScores;

  vector<vector<double> > values;
  unsigned long width;
};
