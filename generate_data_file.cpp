#include "generate_data_file.h"

default_random_engine randomEngine;

void assignLocationWeight(vector<node_data> &left, vector<node_data> &right, double locationWeight){
    vector<Point> leftPoints;
    vector<Point> rightPoints;
    //Std random number generation
    //static default_random_engine randomEngine;
    uniform_real_distribution<double> distribution(0.0, 1.0);

    //Assign random points
    for(unsigned int i=0; i<left.size(); i++){
        Point p;
        p.first = distribution(randomEngine);
        p.second = distribution(randomEngine);
        leftPoints.push_back(p);
    }

    for(unsigned int i=0; i<right.size(); i++){
        Point p;
        p.first = distribution(randomEngine);
        p.second = distribution(randomEngine);
        rightPoints.push_back(p);
    }

    //assign preference values based on distance
    for(unsigned int i=0; i<left.size(); i++){
        for(unsigned int j=0; j<right.size(); j++){
            double dist = sqrt(pow((rightPoints[j].first - leftPoints[i].first),2)
                               + pow((rightPoints[j].second - leftPoints[i].second),2));

            //Since this is value based matching, we need to give a close distance a high preference value
            //This function scales the value between 0 and 1, as the maximum possible distance between two points in the
            //1x1 square space is the square root of two
            double proximityValue = (sqrt(2) - dist) / sqrt(2);

            //weight the value and assign
            proximityValue *= locationWeight;
            left[i].preferenceList[j] += proximityValue;
            right[j].preferenceList[i] += proximityValue;
        }
    }

}

void assignGlobalRankWeight(vector<node_data> &left, vector<node_data> &right, double rankWeight){
    //Std random number generation
    //static default_random_engine randomEngine;
    uniform_real_distribution<double> distribution(0.0, rankWeight);


    vector<double> leftWeight, rightWeight;
    for(unsigned int i=0; i<left.size(); i++){
      leftWeight.push_back(distribution(randomEngine));
    }
    for(unsigned int i=0; i<right.size(); i++){
      rightWeight.push_back(distribution(randomEngine));
    }

    for(unsigned int i=0; i<left.size(); i++){
      for(unsigned int j=0; j<right.size(); j++){
        //left[i].preferenceList[j] += leftWeight[i] * rightWeight[j];
        //Change to this line to find min of global weights
        left[i].preferenceList[j] += min(leftWeight[i], rightWeight[j]);
      }
    }
}

void assignRandomWeight(vector<node_data> &left, vector<node_data> &right, double randomWeight){

    //static default_random_engine randomEngine;
    uniform_real_distribution<double> distribution(-.5, .5);

    for(unsigned int i=0; i<left.size(); i++){
        for(unsigned int j=0; j<right.size(); j++){
            double epsilon = distribution(randomEngine);
            epsilon *= randomWeight;
            left[i].preferenceList[j] += epsilon;
        }
    }

    for(unsigned int i=0; i<right.size(); i++){
        for(unsigned int j=0; j<left.size(); j++){
            double epsilon = distribution(randomEngine);
            epsilon *= randomWeight;
            right[i].preferenceList[j] += epsilon;
        }
    }
}

void write_node_data(ostream &out, vector<node_data> &nodes){
    out << nodes.size() << endl;
    for(unsigned int i=0; i<nodes.size(); i++){
        out << nodes[i].listLength << " ";
        for(int j=0; j<nodes[i].listLength; j++){
            out << nodes[i].preferenceList[j] << " ";
        }
        out << endl;
    }
}

//Handles actual generation
Instance generate_weighted_data(int lhsCount, int rhsCount, double locationWeight, double rankWeight, double randomWeight) {
    Instance I;
    vector<node_data> left, right;

    //lhs
    for(int i=0; i<lhsCount; i++){
        Node n;
        n.size = 1;
        n.allocation = 0;
        I.lhsnodes.push_back(n);
    }
    //rhs nodes
    for(int i=0; i<rhsCount; i++){
        Node n;
        n.size = 1;
        n.allocation = 0;
        I.rhsnodes.push_back(n);
    }
    //edges
    for(int i=0; i<lhsCount; i++){
        for(int j=0; j<rhsCount; j++){
            Edge e;
            e.allocation = 0;
            e.size = 1;
            e.value = 0.0;
            e.start = i;
            e.end = j;
            I.edges.push_back(e);
        }
    }

    //Set up data structures
    for(int i=0; i<lhsCount; i++){
        node_data m;
        m.init(lhsCount);
        left.push_back(m);

        node_data w;
        w.init(lhsCount);
        right.push_back(w);
    }

    //Currently, these functions assume a symmetrical matching
    assignLocationWeight(left, right, locationWeight);
    assignGlobalRankWeight(left, right, rankWeight);
    assignRandomWeight(left, right, randomWeight);

    //Gets data from the left and right structures and assigns it to Instance
    for(unsigned int i=0; i<I.edges.size(); i++){
        int l = I.edges[i].start;
        int r = I.edges[i].end;

        I.edges[i].value = left[l].preferenceList[r];
    }

    return I;
}

Instance get_location_instance(unsigned int leftSize, unsigned int rightSize, double noise){
  if(noise > 1) noise = 1;
  if(noise < 0) noise = 0;

  return(generate_weighted_data(leftSize, rightSize, 1 - noise, 0, noise));
}

Instance get_global_min_instance(unsigned int leftSize, unsigned int rightSize, double noise){
  if(noise > 1) noise = 1;
  if(noise < 0) noise = 0;

  return(generate_weighted_data(leftSize, rightSize, 0, 1 - noise, noise));
}
