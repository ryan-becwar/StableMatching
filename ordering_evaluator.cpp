#include "ordering_evaluator.h"

OrderingEvaluator::OrderingEvaluator(Instance& I, unsigned int greedyCount) :
  I(I),
  greedyCount(greedyCount),
  greedyResults(),
  greedyMean(),
  greedyStdev(),
  zScores(),
  values(),
  width() {

  values = get_value_matrix(I);
  width = values.size();
  generateGreedyResults();

}

void OrderingEvaluator::generateGreedyResults(){
  //random engine needed for random ordering
  default_random_engine& randomEngine = getRandomEngine();

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


  //Calculate mean and stdev of greedy results
  double sum = std::accumulate(greedyResults.begin(), greedyResults.end(), 0);
  greedyMean = sum / greedyResults.size();

  vector<double> difference(greedyResults.size());
  //necessary for lambda capture list unless we wanted to pass all of this, which would be costly
  double tmpMean(greedyMean);
  std::transform(greedyResults.begin(), greedyResults.end(), difference.begin(), [tmpMean](double x) { return x - tmpMean; });

  double sq_sum = std::inner_product(difference.begin(), difference.end(), difference.begin(), 0.0);
  greedyStdev = std::sqrt(sq_sum / greedyResults.size());

}

void OrderingEvaluator::evaluateOrder(std::string title, vector<unsigned int> const& order){
  //Compare order to greedy
  write_matches(I, find_matches(values, order, width));
  double result = get_value(I);

  double stdevsAbove = (result - greedyMean) / greedyStdev;
  zScores.insert(std::make_pair(title, stdevsAbove));


  /*
  std::sort(greedyResults.begin(), greedyResults.end(), std::greater<double>());
  for(unsigned int i=0; i<greedyResults.size(); i++){
  	std::cout << greedyResults[i] << std::endl;
  }
  unsigned int rank;
  for(rank = 0; rank < greedyCount &&
  	result < greedyResults[rank]; rank++);


  std::cout<< "order" << std::endl << "Mean: " << greedyMean << " Stdev: " << greedyStdev << std::endl;
  std::cout << "Rank: " << rank << " out of " << greedyResults.size() << std::endl;
  std::cout << "Value: " << result << " Stdev's above mean: " << stdevsAbove << std::endl;
  */
}

void OrderingEvaluator::printPlotData(double noise, unsigned int instanceNum){
  std::cout << width << " " << noise << " " << instanceNum << " " << greedyMean << " " << greedyStdev << " " << zScores["pagerank"] << " " << zScores["regret"] << std::endl;
}
