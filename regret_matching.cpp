#include "regret_matching.h"
/*
In a regret based matching, we prioritize matches where a node has much worse
matching prospects among its lower priority matches.

To evaluate this, best fit line is drawn (soon to be expanded to a quadratic)
*/

class regret_projection {
public:
  void setConnections(std::vector<double> values){
    std::sort(values.begin(), values.end(), std::greater<double>());
    connectionValues = values;
  }

  /*
  Estimates the regret by calculating the average slope of ray to points
  drawn from the initial point
  */
  void findRayProjection(){
    const double weightDecayRate = 0.9;
    double weight = 1.0;
    double weightDivisor = 0, sum = 0;
    for(unsigned int i=1; i<connectionValues.size(); i++){
      double pointSlope = (connectionValues[i] - connectionValues[0]) / i;
      sum += pointSlope * weight;
      weightDivisor += weight;
      weight *= weightDecayRate;
    }

    projectionSlope = sum / weightDivisor;
    //std::cout << projectionSlope << std::endl;
  }

  std::vector<double> connectionValues;
  double projectionSlope;
};

std::vector<unsigned int> generate_regret_order(std::vector<regret_projection> &projections){
  std::vector<pid> slopeSort;
  for(unsigned int i=0; i<projections.size(); i++){
    pid pair;
    pair.first = i;
    pair.second = projections[i].projectionSlope;
    slopeSort.push_back(pair);
  }

  std::sort(slopeSort.begin(), slopeSort.end(), sort_paird());
  std::reverse(slopeSort.begin(), slopeSort.end());

  /*
  for(unsigned int i=0; i<slopeSort.size(); i++){
    std::cout << slopeSort[i].second << std::endl;
  }
  */

  std::vector<unsigned int> order;
  for(unsigned int i=0; i<slopeSort.size(); i++){
    order.push_back(slopeSort[i].first);
  }

  return order;
}

std::vector<unsigned int> regret_order(Instance& I){
  unsigned long width = (unsigned long) I.lhsnodes.size();

  //get the edge values in matrix
  std::vector<std::vector<double> > values = get_value_matrix(I);

  std::vector<regret_projection> projections;
  //TODO, make width represent row size
  for(int i=0; i<width; i++){
    regret_projection projection;
    projection.setConnections(getColumn(values, i));
    projection.findRayProjection();
    projections.push_back(projection);
  }

  std::vector<unsigned int> order = generate_regret_order(projections);

  /*
  std::vector<pii> matches = find_matches(values, order, width);
  write_matches(I, matches);
  std::cout << "Regret matching value: " << get_value(I) << std::endl;
  */
  
  /*
  regret_projection projection;
  projection.setConnections(getColumn(values, 0));
  projection.findRayProjection();
  */


  return order;
}
