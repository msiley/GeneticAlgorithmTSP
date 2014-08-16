#include "tspchromofactory.h"

vector<Chromosome*>* TSPChromoFactory::create_population(int pop_size) {
  vector<Chromosome*> *population = new vector<Chromosome*>();
  Chromosome *chromo;
  for (int i=0; i < pop_size; i++) {
    chromo = new TSPChromosome(ngenes);
    population->push_back(chromo);
  }
  return population;
}

