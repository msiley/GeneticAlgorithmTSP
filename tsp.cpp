#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <sstream>
#include <fstream>
#include "tspchromosome.h"
#include "geneticalgorithm.h"
#include "tspchromofactory.h"
#include "chromosome.h"
#include "chromosomefactory.h"
#include "tspfilereader.h"
#include <ctime>
using namespace std;

void timestamp(void) {
  time_t rawtime;
  tm *ptm;
  time ( &rawtime );
  ptm = localtime ( &rawtime );
  printf ("%2d:%02d:%02d\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}

int main(int argc, char* argv[]) {

  //cout << "GA TSP" << endl;
  //cout << "Initializing Tour Costs " << endl;

  // test best tour is 1-2-3-4-5-6-7-8-9-10 fitness = 10
  const int ngenes = 10;
  double tour[ngenes][ngenes] = {
    {0, 1, 3, 4, 5, 6, 7, 8, 9, 1},
    {1, 0, 1, 4, 5, 6, 7, 8, 9, 10},
    {8, 1, 0, 1, 5, 6, 7, 8, 9, 10},
    {8, 2, 1, 0, 1, 6, 7, 8, 9, 10},
    {8, 2, 3, 1, 0, 1, 7, 8, 9, 10},
    {8, 2, 3, 4, 1, 0, 1, 8, 9, 10},
    {8, 2, 3, 4, 5, 1, 0, 1, 9, 10},
    {8, 2, 3, 4, 5, 6, 1, 0, 1, 10},
    {8, 2, 3, 4, 5, 6, 7, 1, 0, 1},
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 0}
  };

  vector<vector<double> > tour_table;
  for (int i=0; i<ngenes; i++) {
    vector<double> row;
    for (int j=0; j<ngenes; j++) {
      row.push_back(tour[i][j]);
    }
    tour_table.push_back(row);
  }


  /*

  // Uncomment below and comment test data above for use.
  if (argc != 2) {
    cout << "usage: run_tsp filename" << endl;
    exit(0);
  }

  TSPFileReader file_reader(argv[1]);
  vector<vector<double> > tour_table = file_reader.read();
  int ngenes = tour_table.size();

  //file_reader.print_tour();

  //exit(0);

  */

  TSPChromosome::InitializeCosts(tour_table);
  //cout << "Done" << endl;

  //cout << "NGenes: " << ngenes << endl;
  const int generations = 10000;
  int pop_size       = 250;
  double cross_prob  = 0.65;
  double mutate_prob = 0.015;

  cout << "Population Size: " << pop_size << endl;
  cout << "Cross Prob.: " << cross_prob << endl;
  cout << "Mutate Prob.: " << mutate_prob << endl;
  cout << "Generations: " << generations << endl;
  cout << "Tours: " << ngenes << endl << endl;

  //cout << "Creating ChromosomeFactory....";
  TSPChromoFactory *factory = new TSPChromoFactory(ngenes);
  //cout << "Done with Factory" << endl;
  
  GeneticAlgorithm gapop = GeneticAlgorithm(pop_size, cross_prob, mutate_prob, factory); 

  //cout << "GA Initialized and ready to run" << endl;
  gapop.turn_trace_off();
  //gapop.turn_trace_on();
  gapop.evaluate();
  int last_improvement = 0;
  TSPChromosome *winner;
  TSPChromosome *previous_winner = (TSPChromosome*)gapop.get_best_chromosome();
  double prev_eval = 0;
  double opt_tour = 10;
  int i=0;
  //for (int i=0; i < generations; i++) {

  timestamp();
  cout << "Best of Gen " << 0 << " v(" << previous_winner->repr() << ") -> " << previous_winner->evaluate() << endl;
  while (1) { //prev_eval == opt_tour) {
    gapop.select();
    gapop.crossover();
    gapop.mutate();
    gapop.evaluate();

    winner = (TSPChromosome*)gapop.get_best_chromosome();
    if (winner->evaluate() != prev_eval) {
      timestamp();
      cout << "Best of Gen " << (i+1) << " v(" << winner->repr() << ") -> " << winner->evaluate() << endl;
      prev_eval = winner->evaluate();
    }

    if (i % 1000 == 0) cout << i << endl;

    if (i == generations) break;

    if (abs(winner->evaluate()) == opt_tour) break;

    i++;
  }
  winner = (TSPChromosome*)gapop.get_best_chromosome();
  cout << "Winner" << " v(" << winner->repr() << ") -> " << winner->evaluate() << endl;

  return 0;
}




