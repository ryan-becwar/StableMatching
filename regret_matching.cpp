#include <iostream>
#include <vector>
#include "matrix.h"
#include "echo_instance.h"

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

  void findProjection(){
    const double weightDecayRate = 0.9;
    double weight = 1.0;
    double weightDivisor = 0, sum = 0;
    for(unsigned int i=1; i<connectionValues.size(); i++){
      double pointSlope = (connectionValues[i] - connectionValues[0]) / i;
      sum += pointSlope * weight;
      weightDivisor += weight;
    }

    projectionSlope = sum / weightDivisor;
  }

  std::vector<double> connectionValues;
  double projectionSlope;
};

int main(){
  return 0;
}
