#include "geneticalgorithm.h"

void GeneticAlgorithm::turn_trace_off(void) {
  trace = false;
  Chromosome *chromo;
  for (int i=0; i < population->size(); i++) {
    chromo = population->at(i);
    chromo->turn_trace_off();
  }
}

void GeneticAlgorithm::turn_trace_on(void) {
  cout << "Starting Trace " << endl;
  trace = true;
  Chromosome *chromo;
  for (int i=0; i < population->size(); i++) {
    chromo = population->at(i);
    chromo->turn_trace_on();
  }
}

GeneticAlgorithm::GeneticAlgorithm(int pop_size, double cross_prob, double mutate_prob, ChromosomeFactory *factory) {
  this->factory = factory;
  this->pop_size = pop_size;
  this->cross_prob = cross_prob;
  this->mutate_prob = mutate_prob;

  trace = true;

  random = Random::Instance();

  population = factory->create_population(this->pop_size);
  //if (trace) cout << "Population Created" << endl;

  // init best... or else evaluate may not work properly.
  Chromosome *chromo = population->at(0); 
  last_fit = chromo->evaluate();
  best_chromosome = chromo->clone();
  
}

GeneticAlgorithm::~GeneticAlgorithm(void) {
}

vector<Chromosome*>* GeneticAlgorithm::get_population(void) {
  return this->population;
}

void GeneticAlgorithm::print_population(void) {
  for (int i=0; i < population->size(); i++) {
    cout << "[" << (i+1) << "](" << population->at(i)->repr() << "), ";
    if ((i+1) % 5 == 0) cout << endl;
  }
  cout << endl;
}

void GeneticAlgorithm::evaluate(void) {
  if (trace) { // defer to chromosome
    cout << endl;
    cout << "Evaluating" << endl;
    print_population();
    cout << endl;
  }
  double fit_value; 
  Chromosome *chromo;
  for (int i = 0; i < pop_size; i++) {
    chromo = population->at(i);
    fit_value = chromo->evaluate();
    if (fit_value > last_fit) {
      delete best_chromosome;
      best_chromosome = chromo->clone();
      last_fit = fit_value;
    }
  }
  if (trace) cout << "Done Evaluating" << endl;
}

/*
Selection Process 
1) Calculate fitness value for each chromosome f[i] = eval(v[i])
2) Find the total fitness of the population F = Sum(v)
3) Calculate the probablity of selection p[i] = f[i]/F
4) Calculate cumulative probability.  q[i] = Sum(p[j], i) 
5) Generate a Random float from 0..1
6) if r < q[i] select v[0] else select ith chromosome such that q[i-1] < r <= q[i]
*/
void GeneticAlgorithm::select(void) {
  if (trace) {
    cout << "1) Calculate fitness value for each chromosome f[i] = eval(v[i])" << endl;
    cout << "2) Find the total fitness of the population F = Sum(v)" << endl;
  }
  vector<double> pop_fitness;
  double total_fitness = 0;
  double fit_value;
  Chromosome *chromo;
  for (int i = 0; i < pop_size; i++) {
    chromo = population->at(i);
    fit_value = chromo->evaluate();
    if (trace) { cout << "v(" << (*chromo) << ") -> " << fit_value << endl; }
    total_fitness += fit_value;
    pop_fitness.push_back(fit_value);
  }
  

  if (trace) {
    cout << "Population Fitness: " << endl;
    print_list(pop_fitness);
    cout << endl;

    cout << "Total Fitness: " << total_fitness << endl;
    cout << endl;

    cout << "3) Calculate the probablity of selection p[i] = f[i]/F" << endl;
  }

  vector<double> pop_prob_selection;
  for (int i=0; i < pop_size; i++) {
    pop_prob_selection.push_back(pop_fitness[i]/total_fitness);
  }

  if (trace) {
    cout << "Probability of Selection: " << endl;
    print_list(pop_prob_selection);
    cout << endl;
    cout << "4) Calculate cumulative probability.  q[i] = Sum(p[j], i)" << endl;
  }

  vector<double> cumulative;
  double total = 0;
  for (int i=0; i < pop_size; i++) {
    total += pop_prob_selection[i];
    cumulative.push_back(total);
  }

  if (trace) {
    cout << "Cumulative Probability: " << endl;
    print_list(cumulative);
    cout << endl;

    cout << "5) Generate a Random r float from 0..1" << endl;
    cout << "6) if r < q[0] select v[0] else select ith chromosome such that q[i-1] < r <= q[i]" << endl;
  }

  double r, cprob;
  int nchromo;
  double first_prob = cumulative[0];
  Chromosome *first_chromo = population->at(0);


  vector<Chromosome*> next_generation; 
  for (int n=0; n < pop_size; n++) {
    r = random->unit_random();
    if (r < first_prob) { // special case of first chromo (see Rule 6).
      next_generation.push_back(first_chromo->clone());
      nchromo = 0;
    }
    else {
      // search for cumulative prob to attain next chromosome.
      for (int i=1; i < pop_size; i++) { // skip first chromo.
        cprob = cumulative[i];
        if (r <= cprob) {
          chromo = population->at(i);
          next_generation.push_back(chromo->clone());
          nchromo = i;
          break;
        }
      }
    }

    if (trace) {
      cout << "v(" << (n+1) << ") r = " << r << "[" << nchromo << "](" << (*population->at(nchromo)) << ")" << endl;
    }
  }

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

void GeneticAlgorithm::crossover(void) {
  // Make cross_prob a object variable;
  if (trace) {
    cout << "For each chromosome" << endl;
    cout << "1) Generate a random float r from 0..1" << endl;
    cout << "2) if r < Pc select chromosome for crossover." << endl;
  }

  vector<int> candidate_indexes;
  vector<int> excluded_indexes;
  double r;

  for (int i=0; i < pop_size; i++) {
      r = random->unit_random();
      if (r < cross_prob) {
        candidate_indexes.push_back(i);
      }
      else {                     
        excluded_indexes.push_back(i);
      }
  }

  // Note: additional info.
  // if an odd number of chromosomes are chosen
  // then a chromosome is added or removed at random.
  // i.e. generate a random float of > .5 then add else remove.
  // if a chromosome is chosen then it to is at random.

  int idx, r_int;
  bool list_is_odd = candidate_indexes.size() % 2 != 0;
  bool have_exclusions = excluded_indexes.size() != 0; // uneven number of cities.
  if (list_is_odd) { 
    if (random->unit_random() > 0.5 && have_exclusions) {// add a randomly chosen chromosome.
      idx = random->rand_int(excluded_indexes.size());
      candidate_indexes.push_back(excluded_indexes[idx]); 
    }
    else { // remove a chromosome randomly.
      idx = random->rand_int(candidate_indexes.size());
      vector<int>::iterator iter = candidate_indexes.begin();
      iter += idx;
      candidate_indexes.erase(iter);
    }
  }

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
      c_one_idx = candidate_indexes[i++];
      c_two_idx = candidate_indexes[i++];

      c_one = population->at(c_one_idx);
      c_two = population->at(c_two_idx);
  
      if (trace) {
        cout << "Crossing Chromosomes [" << c_one_idx+1 << "] and [" << c_two_idx+1 << "]" << endl;
      }

      c_one->crossover(c_two);
    }
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
void GeneticAlgorithm::mutate(void) {
  if (trace) {
    cout << "Mutation" << endl;
    cout << "for each bit" << endl;
    cout << "1) generate a random float from 0..1" << endl;
    cout << "2) if r < Pm then mutate that bit" << endl;
  }
  
  double r;
  int nbit = 0;
  int bit;
  Chromosome *chromo;
  for (int i=0; i < pop_size; i++) {
    chromo = population->at(i);
    if (trace) cout << "Mutate possible for [" << i << "]" << endl;
    chromo->mutate(mutate_prob);
  }
  if (trace) {
    cout << endl;
    cout << "Population after Mutation" << endl;
    print_population();
  }
}

