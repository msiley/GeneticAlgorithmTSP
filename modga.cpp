#include "modga.h"


ModGA::ModGA(int pop_size, double cutoff_percent, double mutate_prob, ChromosomeFactory *factory) {
  this->factory = factory;
  this->pop_size = pop_size;
  this->mutate_prob = mutate_prob;

  trace = true;

  random = Random::Instance();

  population = factory->create_population(this->pop_size);

  // init best... or else evaluate may not work properly.
  Chromosome *chromo = population->at(0); 
  last_fit = chromo->evaluate();
  best_chromosome = chromo->clone();

  select_cutoff = (int)(cutoff_percent * pop_size);

  mutation_ready = new vector<Chromosome*>();
  crossover_ready = new vector<Chromosome*>();

}

ModGA::~ModGA(void) {
  // XXX kill population.... eventually.
}


bool chromosome_cmp(Chromosome *c1, Chromosome *c2) { // descending sort.
  return c1->evaluate() > c2->evaluate();
}

void ModGA::select(void) {
  if (trace) {
    cout << "1) Sort Chromosomes by Fitness (higher to lower)" << endl;
    cout << "2) Seperate chromosomes for genetic operations that are at or above specified cutoff point." << endl;
    cout << "3) Mark each 'higher end' chromosome for either crossover or mutation."  << endl;
    cout << "There will be select_cutoff parents and select_cutoff children "
            "therefore to fill out the population there will be pop_size - select_cutoff "
            "needed from the non parental population." << endl;
  }


  sort(population->begin(), population->end(), chromosome_cmp); 


  vector<Chromosome*> next_generation;
  Chromosome *chromo;
  double r;
  for (int i=0; i<select_cutoff; i++) {
    r = random->unit_random();
    chromo = population->at(i);
    if (r < mutate_prob) 
      mutation_ready->push_back(chromo->clone());
    else
      crossover_ready->push_back(chromo->clone());
    next_generation.push_back(chromo->clone()); // make sure parents enter next-gen population.
  }

  // if there is an odd number of crossover chromosomes pop one onto mutation.
  int xsize = crossover_ready->size();
  if (xsize  % 2 != 0) {
    chromo = crossover_ready->at(xsize-1);
    crossover_ready->pop_back();
    mutation_ready->push_back(chromo);
  }
    
    
  
  // there will be select_cutoff parents and select_cutoff children 
  // therefore to fill out the population there will be pop_size - select_cutoff 
  // needed from the non parental population.

  for (int i=select_cutoff*2; i<pop_size; i++) 
    next_generation.push_back(population->at(i)->clone());
  
  population->swap(next_generation);
  for (int i=0; i<pop_size; i++) delete next_generation.at(i);

  if (trace) {
    cout << endl;
    print_population();
    cout << endl;
  }


}



// Crossover Process
// Pc = probablity of crossover
//   1) Generate a random float r from 0..1 
//   2) if r < Pc select chromosome for crossover.
//   3) mate selected chromosomes randomly

// for each pair of chromosomes
//   pos = a random integer from 0 to length-1
//   pos is the crossover point.

// Note: if an odd number of chromosomes are chosen
// then a chromosome is added or removed at random.
// i.e. generate a random float of > .5 then add else remove.
// if a chromosome is chosen then it to is at random.

void ModGA::crossover(void) {
  // Make cross_prob a object variable;
  if (trace) {
    cout << "Crossover" << endl;
  }

  vector<int> candidate_indexes;
  for (int i=0; i < crossover_ready->size(); i++) 
    candidate_indexes.push_back(i);


  if (trace) {
    cout << "3) mate selected chromosomes randomly" << endl;
  }


  int ncandidates = candidate_indexes.size();
  if (ncandidates > 0) {

    // this will make ranomization much easier.
    vector<int> indexes;
    for (int i=0; i<candidate_indexes.size(); i++) 
      indexes.push_back(candidate_indexes[i]);

    candidate_indexes.clear(); // we will re-use this below.

    // Randomize indexes.
    int size = indexes.size();
    int cnt = 0;
    while (cnt < size) {
      int r_idx = random->rand_int(indexes.size());
      candidate_indexes.push_back(indexes[r_idx]);

      // erase used candidate
      vector<int>::iterator iter = indexes.begin();
      iter += r_idx;
      indexes.erase(iter);
      cnt++;
    }

    if (trace) {
      print_list(candidate_indexes);
    }

    int i = 0;
    int c_one_idx, c_two_idx, pos;
    string orig_one, orig_two;
    Chromosome *c_one, *c_two;
    while (i < ncandidates) {
      //cout << "i = " << i << endl;
      c_one_idx = candidate_indexes[i++];
      //cout << "i = " << i << endl;
      c_two_idx = candidate_indexes[i++];

      //cout << "C1 idx = " << c_one_idx << " C2 idx = " << c_two_idx << endl;
      c_one = crossover_ready->at(c_one_idx);
      c_two = crossover_ready->at(c_two_idx);
  
      //cout << "before trace " << endl;
      if (trace) {
        cout << i << " " << ncandidates << " Crossing Chromosomes [" << c_one_idx+1 << "] and [" << c_two_idx+1 << "]" << endl;
      }

      c_one->crossover(c_two);
      //cout << "expost X" << endl;

      population->push_back(c_one->clone());
      population->push_back(c_two->clone());
      
      delete c_one;
      delete c_two;
    }
    crossover_ready->clear(); // XXX This will control population size.
    //if (trace) cout << "DONE Xing over" << endl;
  }
  else {
    if (trace) {
      cout << "No Crossover" << endl;
    }
  }
  if (trace) {
    cout << "Population After Crossover" << endl;
    print_population();
  }
}

/*
 Mutation Process
 Pm = probability of mutation
 Each bit has an equal chance to undergo mutation.
 for each bit
   1) generate a random float from 0..1
   2) if r < Pm then mutate that bit
*/
void ModGA::mutate(void) {
  if (trace) {
    cout << "**Mutation**" << endl;
  }
  
  double r;
  int nbit = 0;
  int bit;
  Chromosome *chromo;
  for (int i=0; i < mutation_ready->size(); i++) {
    chromo = mutation_ready->at(i);
    if (trace) cout << "Mutating: " << chromo->repr() << endl;
    chromo->mutate(1);
    population->push_back(chromo->clone());
    delete chromo;
  }
  mutation_ready->clear(); 
  
  if (trace) {
    cout << endl;
    cout << "Population after Mutation" << endl;
    print_population();
  }
}

