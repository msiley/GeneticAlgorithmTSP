#include "binarychromofactory.h"

vector<Chromosome*>* BinaryChromoFactory::create_population(int pop_size) {
  vector<Chromosome*> *population = new vector<Chromosome*>();
  string bitstring;
  Chromosome *chromo;
  RandomBits rand_bits;
  for (int i=0; i < pop_size; i++) {
    cout << "pop" << i << endl;
    bitstring = rand_bits.make_random_bits(nbits);
    chromo = new BinaryChromosome(bitstring);
    population->push_back(chromo);
  }
  return population;
}

