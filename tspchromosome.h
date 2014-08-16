#include <iostream>
#include <ostream>
#include <string>
#include <cstdio>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <sstream>
#include <list>
#include "random.h"
#include "randombits.h"
#include "utility.h"
#include "chromosome.h"
#include <cassert>
using namespace std;

#ifndef __TSPCHROMOSOME__
#define __TSPCHROMOSOME__
 
class TSPChromosome: public Chromosome {
private:
  string s_tour;
  int ngenes;
  vector<int> tour;
  Random *random;
public:
  static vector<vector<double> > tsp_costs;

  TSPChromosome(int ngenes);
  TSPChromosome(string s_tour); 
  TSPChromosome(void) {};

  // ga operations
  void mutate(double mutate_prob);
  void crossover(Chromosome *chromo_mate);
  double evaluate(void);

  Chromosome* clone(void);
 
  string &repr(void); 

  static void InitializeCosts(const vector<vector<double> > cost_matrix); 

  void set_tour(vector<int> tour) { this->tour = tour; };
  vector<int>& get_tour(void) { return tour; };

  //~TSPChromosome(void) { cout << "delete me!" << endl; }
};



#endif
