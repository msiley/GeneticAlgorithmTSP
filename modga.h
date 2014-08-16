#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include "random.h"
#include "chromosome.h"
#include "chromosomefactory.h"
#include "tspchromosome.h"
#include "geneticalgorithm.h"
#include <algorithm>
using namespace std;

#ifndef __MODGA__
#define __MODGA__

class ModGA: public GeneticAlgorithm {
private:
  int select_cutoff;
  vector<Chromosome*> *crossover_ready;
  vector<Chromosome*> *mutation_ready;
public:
  ModGA(int pop_size, double cutoff_percent, double mutate_prob, ChromosomeFactory *factory);
  ~ModGA(void);
  void select(void); 
  void crossover(void);
  void mutate(void);
};


#endif
