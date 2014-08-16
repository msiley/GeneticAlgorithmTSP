#include <string>
#include <iostream>
using namespace std;


#ifndef __CHROMOSOME__
#define __CHROMOSOME__

 
class Chromosome {
protected:
  bool trace;
  
public:
  Chromosome(int genes) {};
  Chromosome(string bitstring) {};
  Chromosome(void) {};

  // ga operations
  virtual void mutate(double mutate_prob) {};
  virtual void crossover(Chromosome *mate) {};
  virtual double evaluate(void) { return 0; };
  virtual string &repr(void) {};

  virtual Chromosome* clone(void) {};

  void turn_trace_on(void) { trace = true; };
  void turn_trace_off(void) { trace = false; };

  virtual ~Chromosome(void) {};
};

ostream& operator<<(ostream& os, Chromosome &chromo); 


#endif
