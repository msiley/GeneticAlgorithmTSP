#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include "chromosome.h"
using namespace std;

#ifndef __CHROMOSOMEFACTORY__
#define __CHROMOSOMEFACTORY__

class ChromosomeFactory {
private:
  int nbits;
public:
  ChromosomeFactory(void) {};
  ChromosomeFactory(int nbits) { this->nbits = nbits; };
  virtual Chromosome create_chromosome(string bitstring) { return Chromosome(bitstring); };
  virtual Chromosome create_chromosome(int nbits) { return Chromosome(nbits); };
  virtual vector<Chromosome*> *create_population(int pop_size) {};
};

#endif
