#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include "random.h"
#include "randombits.h"
#include "utility.h"
#include "chromosome.h"
#include <cassert>
using namespace std;

#ifndef __BINARYCHROMOSOME__
#define __BINARYCHROMOSOME__
 
class BinaryChromosome: public Chromosome {
private:
  string bitstring;
  int nbits;
  Random *random;
public:
  BinaryChromosome(int nbits);
  BinaryChromosome(string bitstring);
  BinaryChromosome(void) {};

  // ga operations
  void mutate(double mutate_prob);
  void crossover(Chromosome *chromo_mate);
  double evaluate(void);

  // access
  string &get_bitstring(void) { return bitstring; };
  double set_bitstring(string bitstring);

  Chromosome* clone(void);
 
  string &repr(void); 

  
};


#endif
