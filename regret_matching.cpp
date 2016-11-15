#include "regret_matching.h"
/*
In a regret based matching, we prioritize matches where a node has much worse
matching prospects among its lower priority matches.

To evaluate this, best fit line is drawn (soon to be expanded to a quadratic)
*/

void getStats(std::vector<double>& set, std::vector<double>& difference, double& mean, double& stDev);

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

  //Find the fitting line of the form y = a + bx, where b is the projectionSlope
  void findLinearRegression(){
    double xStdev, yStdev, xMean, yMean, r = 0;
    std::vector<double> xVals;
    std::vector<double> xDifference(connectionValues.size());
    std::vector<double> yDifference(connectionValues.size());

    xVals.reserve(connectionValues.size());
    for(unsigned int i=0; i<connectionValues.size(); i++){
      xVals.push_back((double)i);
    }


    getStats(xVals, xDifference, xMean, xStdev);
    getStats(connectionValues, yDifference, yMean, yStdev);

    //Calculate r value
    //r = sum(x diff * y diff) / xstdev * ystdev
    double diffSum = 0;
    for(unsigned int i=0; i<connectionValues.size(); i++){
      diffSum += xDifference[i] * yDifference[i];
    }
    r = diffSum / (xStdev * yStdev);

    projectionSlope = r * (yStdev / xStdev);
    //a = yMean - projectionSlope * xMean;

    /*
    std::transform(set.begin(), set.end(), difference.begin(), [mean](double x) { return x - mean; });
    //necessary for lambda capture list unless we wanted to pass all of this, which would be costly
    double tmpMean(yMean);
    //Creates a difference vector which contains the difference of each value from the mean
    vector<double> yDifference(greedyResults.size());
    std::transform(yResults.begin(), yResults.end(), yDifference.begin(), [tmpMean](double x) { return x - tmpMean; });

    double ySqSum = std::inner_product(yDifference.begin(), yDifference.end(), yDifference.begin(), 0.0);
    yStdev = std::sqrt(ySqSum / connectionValues.size());
    */
  }

  std::vector<double> connectionValues;
  double projectionSlope;
};

void getStats(std::vector<double>& set, std::vector<double>& difference, double& mean, double& stDev){

  mean = std::accumulate(set.begin(), set.end(), 0) / set.size();

  //Creates a difference vector which contains the difference of each value from the mean
  std::transform(set.begin(), set.end(), difference.begin(), [mean](double x) { return x - mean; });

  double sqSum = std::inner_product(difference.begin(), difference.end(), difference.begin(), 0.0);
  stDev = std::sqrt(sqSum / set.size());
}

//Generates an ordering given a vector of projection slopes
std::vector<unsigned int> generate_regret__order(std::vector<double> &projectionSlopes){
  std::vector<pid> slopeSort;
  for(unsigned int i=0; i<projectionSlopes.size(); i++){
    pid pair;
    pair.first = i;
    pair.second = projectionSlopes[i];
    slopeSort.push_back(pair);
  }

  std::sort(slopeSort.begin(), slopeSort.end(), sort_paird());
  std::reverse(slopeSort.begin(), slopeSort.end());

  std::vector<unsigned int> order;
  for(unsigned int i=0; i<slopeSort.size(); i++){
    order.push_back(slopeSort[i].first);
  }

  return order;
}

std::vector<unsigned int> regret_projection_order(Instance& I){
  unsigned long width = (unsigned long) I.lhsnodes.size();

  //get the edge values in matrix
  std::vector<std::vector<double> > values = get_value_matrix(I);

  std::vector<double> projectionSlopes;
  //TODO, make width represent row size
  for(int i=0; i<width; i++){
    regret_projection projection;
    projection.setConnections(getColumn(values, i));
    projection.findRayProjection();
    projectionSlopes.push_back(projection.projectionSlope);
  }

  std::vector<unsigned int> order = generate_regret__order(projectionSlopes);

  return order;
}

std::vector<unsigned int> regret_regression_order(Instance& I){
  unsigned long width = (unsigned long) I.lhsnodes.size();

  //get the edge values in matrix
  std::vector<std::vector<double> > values = get_value_matrix(I);

  std::vector<double> projectionSlopes;
  //TODO, make width represent row size
  for(int i=0; i<width; i++){
    regret_projection projection;
    projection.setConnections(getColumn(values, i));
    projection.findLinearRegression();
    projectionSlopes.push_back(projection.projectionSlope);
  }

  std::vector<unsigned int> order = generate_regret__order(projectionSlopes);

  return order;
}
