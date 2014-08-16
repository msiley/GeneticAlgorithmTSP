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
using namespace std;

#ifndef __GENETICALGORITHM__
#define __GENETICALGORITHM__

class GeneticAlgorithm {
protected:
  vector<Chromosome*> *population;
  ChromosomeFactory *factory;
  int pop_size;
  double cross_prob;
  double mutate_prob;
  Chromosome *best_chromosome;
  double last_fit;
  bool trace;
  Random *random;
public:
  GeneticAlgorithm(int pop_size, double cross_prob, double mutate_prob, ChromosomeFactory *factory);
  GeneticAlgorithm(void) {};
  ~GeneticAlgorithm(void);
  vector<Chromosome*> *get_population(void);
  virtual void select(void); // this is really the only standard method.
  virtual void crossover(void);
  virtual void mutate(void);
  virtual void evaluate(void);

  Chromosome *get_best_chromosome(void) { return best_chromosome; };
  void turn_trace_off(void);
  void turn_trace_on(void);
  void print_population(void);

/*
  Chromosome *get_best_chromosome(void) { return best_chromosome; };
  void turn_trace_off(void);
  void turn_trace_on(void);
  void print_population(void);
*/
};


#endif
