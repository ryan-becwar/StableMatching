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

  vector<double> connectionValues;

}
