#include "ordering_evaluator.h"

OrderingEvaluator::OrderingEvaluator(Instance& I, unsigned int greedyCount) : I(I), greedyCount(greedyCount), greedyResults(), values(), width() {

  values = get_value_matrix(I);
  width = values.size();
  generateGreedyResults();

}

void OrderingEvaluator::generateGreedyResults(){
  //random engine needed for random ordering
  default_random_engine randomEngine;

  for(unsigned int i=0; i<greedyCount; i++){
    //generate random order vector
    vector<unsigned int> randomOrder;
    for(unsigned int j=0; j<width; j++){
      randomOrder.push_back(j);
    }

    std::shuffle(std::begin(randomOrder), std::end(randomOrder), randomEngine);


    //calculate value of matching based on this order
    vector<pii> randomMatches = find_matches(values, randomOrder, width);
    //Write back matches to the instance
    write_matches(I, randomMatches);
    greedyResults.push_back(get_value(I));
  }

  std::sort(greedyResults.begin(), greedyResults.end(), std::greater<double>());

  for(unsigned int i=0; i<greedyResults.size(); i++){
  	std::cout << greedyResults[i] << std::endl;
  }
}

void OrderingEvaluator::evaluateOrder(vector<unsigned int> const& order){
  double sum = std::accumulate(greedyResults.begin(), greedyResults.end(), 0);
  double mean = sum / greedyResults.size();
  double stdev;

  vector<double> difference(greedyResults.size());
  std::transform(greedyResults.begin(), greedyResults.end(), difference.begin(), [mean](double x) { return x - mean; });

  double sq_sum = std::inner_product(difference.begin(), difference.end(), difference.begin(), 0.0);
  stdev = std::sqrt(sq_sum / greedyResults.size());


  //Compare order to greedy
  write_matches(I, find_matches(values, order, width));
  double result = get_value(I);

  double stdevsAbove = (result - mean) / stdev;

  unsigned int rank;
  for(rank = 0; rank < greedyCount &&
  	result < greedyResults[rank]; rank++);


  std::cout<< "order" << std::endl << "Mean: " << mean << " Stdev: " << stdev << std::endl;
  std::cout << "Rank: " << rank << " out of " << greedyResults.size() << std::endl;
  std::cout << "Value: " << result << " Stdev's above mean: " << stdevsAbove << std::endl;
}
