#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include "binarychromosome.h"
#include "geneticalgorithm.h"
#include "chromosomefactory.h"
#include "chromosome.h"
#include "binarychromofactory.h"
using namespace std;

/*
Genetic Algorithm Outline

begin 
  t = 0
  initialize P(t)
  evaluate P(t)
  while (not termination condition) do
  begin
    t = t + 1
    select P(t) from P(t-1)
    crossover P(t)
    mutate P(t)
    evaluate P(T)
  end
end

*/

// P = n!/(n-r)!

int main(void) {
  BinaryChromoFactory *factory = new BinaryChromoFactory(20);
  GeneticAlgorithm gapop = GeneticAlgorithm(20, 0.45, 0.02, factory); 
  gapop.turn_trace_off();
  //gapop.turn_trace_on();
  gapop.evaluate();
  int last_improvement = 0;
  // GA should be an iterator.
  Chromosome *last_winner, *winner;
  cout << "Starting GA" << endl;
  for (int i=0; i < 100000000; i++) {
    //if (i == 990) gapop.turn_trace_on();
    gapop.select();
    gapop.crossover();
    gapop.mutate();
    gapop.evaluate();
    winner = gapop.get_best_chromosome();
    cout << "Best of Gen " << (i+1) << " v(" << (*winner) << ") -> " << winner->evaluate() << endl;

    if (i == 0) {
      last_winner = winner;
    }
    else {
      if (last_winner->evaluate() == winner->evaluate()) {
        ++last_improvement; // increment for no improvement.
        //cout << last_improvement << endl;
      }
      else {
        last_winner = winner;
        last_improvement = 0; // reset there was an improvement.
      }
    }
  
    if (last_improvement > 10000) {
      cout << "No More Improvement" << endl;
      break;
    }

  }


  winner = gapop.get_best_chromosome();
  cout << "Winner" << " v(" << (*winner) << ") -> " << winner->evaluate() << endl;

  return 0;
}


// 
// Random Solution
// 
/*
def random_solution(nbits, pop_size, generations, fitness):

  // Parameters
  total_individuals = pop_size * generations
  random_population = init_population(total_individuals, nbits)
  random_solutions = [fitness(chromosome) for chromosome in random_population]
  random_fitness_with_chromo = zip(random_solutions, random_population)
  random_fitness_with_chromo.sort(lambda a, b: cmp(a[0], b[0]))
  random_fitness_with_chromo.reverse()
  best_solution = random_fitness_with_chromo[0]

  return best_solution[1]

*/


