#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include "chromosome.h"
#include "randombits.h"
#include "tspchromosome.h"
#include "chromosomefactory.h"
using namespace std;

#ifndef __TSPCHROMOFACTORY__
#define __TSPCHROMOFACTORY__

class TSPChromoFactory: public ChromosomeFactory {
private:
  int ngenes;
public:
  TSPChromoFactory(void) {};
  TSPChromoFactory(int ngenes) { this->ngenes = ngenes; };
  Chromosome create_chromosome(string s_tour) { return TSPChromosome(s_tour); };
  Chromosome create_chromosome(int ngenes) { return TSPChromosome(ngenes); };
  vector<Chromosome*> *create_population(int pop_size);
};

#endif
