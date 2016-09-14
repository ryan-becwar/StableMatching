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
    std::cout << projectionSlope << std::endl;
  }

  std::vector<double> connectionValues;
  double projectionSlope;
};

vector<unsigned int> generate_regret_order(vector<vector<double> > values){
  vector<unsigned int> order;

  return order;
}
int main(int argc, char *argv[]){
  //Readin Data
  Instance I = read_instance();
  unsigned long width = (unsigned long) I.lhsnodes.size();

  //get the edge values in matrix
  vector<vector<double> > values = get_value_matrix(I);

  regret_projection projection;
  projection.setConnections(getColumn(values, 0));
  projection.findRayProjection();

  return 0;
}
