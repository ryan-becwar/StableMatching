#include <iostream>
#include "ordering_evaluator.h"

int main(){
  Instance I = read_instance();

  OrderingEvaluator evaluator(I, 100);

  vector<unsigned int> testOrder(100);
  evaluator.evaluateOrder(testOrder);
}
