#include "ordering_evaluator.h"

ordering_evaluator::ordering_evaluator(Instance& I, unsigned int greedyCount) :
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
  generate_greedy_results();

}

void ordering_evaluator::generate_greedy_results(){
  //random engine needed for random ordering
  default_random_engine& randomEngine = get_random_engine();

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

  //necessary for lambda capture list unless we wanted to pass all of this, which would be costly
  double tmpMean(greedyMean);
  //Creates a difference vector which contains the difference of each value from the mean
  vector<double> difference(greedyResults.size());
  std::transform(greedyResults.begin(), greedyResults.end(), difference.begin(), [tmpMean](double x) { return x - tmpMean; });

  double sq_sum = std::inner_product(difference.begin(), difference.end(), difference.begin(), 0.0);
  greedyStdev = std::sqrt(sq_sum / greedyResults.size());

}

//Find the value of a given order and compares it to the distribution of random orders
void ordering_evaluator::evaluate_order(std::string title, vector<unsigned int> const& order){
  //Compare order to greedy
  write_matches(I, find_matches(values, order, width));
  double result = get_value(I);

  double stdevsAbove = (result - greedyMean) / greedyStdev;
  zScores.insert(std::make_pair(title, stdevsAbove));
  rawScores.insert(std::make_pair(title, result));
}

//For compairing the strength of a known value to the greedy distribution, such as the global greedy or optimal score
void ordering_evaluator::evaluate_value(std::string title, double value) {
  double stdevsAbove = (value - greedyMean) / greedyStdev;
  zScores.insert(std::make_pair(title, stdevsAbove));
  rawScores.insert(std::make_pair(title, value));
}

void ordering_evaluator::print_plot_data(double noise, unsigned int instanceNum){
//  std::cout << width << "," << noise << "," << instanceNum << "," << greedyMean << "," << greedyStdev << ","
//    << zScores["pagerank"] << "," << zScores["regret"] << "," << zScores["optimal"] << std::endl;
  std::cout << width << "," << noise << "," << instanceNum << "," << greedyMean << "," << greedyStdev << ","
    << rawScores["pagerank"] << "," << rawScores["regret"] << "," << rawScores["optimal"] << std::endl;
}
