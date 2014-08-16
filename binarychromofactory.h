#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include "chromosome.h"
#include "randombits.h"
#include "binarychromosome.h"
#include "chromosomefactory.h"
using namespace std;

#ifndef __BINARYCHROMOFACTORY__
#define __BINARYCHROMOFACTORY__

class BinaryChromoFactory: public ChromosomeFactory {
private:
  int nbits;
  RandomBits rand_bits;
public:
  BinaryChromoFactory(void) {};
  BinaryChromoFactory(int nbits) { this->nbits = nbits; };
  Chromosome create_chromosome(string bitstring) { return Chromosome(bitstring); };
  Chromosome create_chromosome(int nbits) { return Chromosome(nbits); };
  vector<Chromosome*> *create_population(int pop_size);
};

#endif
