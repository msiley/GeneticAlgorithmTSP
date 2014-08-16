#include "tspchromosome.h"

vector<vector<double> > TSPChromosome::tsp_costs = vector<vector<double> >();

void print_list(vector<TSPChromosome> &lst) {
  for (int i=0; i < lst.size(); i++) {
    cout << "[" << (i+1) << "](" << lst.at(i) << "), ";
    if ((i+1) % 5 == 0) cout << endl;
  }
  cout << endl;
}


string &TSPChromosome::repr(void) {
  ostringstream buffer;
  s_tour = "";
  for (int i=0; i<ngenes; i++) {
    buffer << tour[i];
    if (i != ngenes-1) buffer << "-";
  }
  s_tour = buffer.str();
  return s_tour;
}


Chromosome* TSPChromosome::clone(void) {
  return new TSPChromosome(repr());
}

// creates a chromosome randomly selecting the genes.
TSPChromosome::TSPChromosome(int ngenes) {
  this->ngenes = ngenes;
  random = Random::Instance();
  tour = vector<int>(ngenes);
  vector<int> indexes;
  // init list with genes possibilities.
  for (int i=0; i<ngenes; i++) 
    indexes.push_back(i);
  
  // randomize tour 
  int idx;
  vector<int>::iterator iter;
  for (int i=0; i<ngenes; i++) {
    iter = indexes.begin();
    idx = random->rand_int(indexes.size());
    tour[i] = indexes[idx];
    iter += idx;
    indexes.erase(iter);
  }

  trace = false;
}

// creates a chromosome with a previously determined set of genes.
TSPChromosome::TSPChromosome(string s_tour) { 
  string buffer;
  int len = s_tour.length();
  for (int i=0; i<len; i++) {
    if (s_tour[i] == '-' || i == len-1) {
      if (i == len-1) buffer += s_tour[i];
      tour.push_back(atoi(buffer.c_str()));
      buffer = "";
    }
    else {
      buffer += s_tour[i];
    }
  }

  trace = false;
  random = Random::Instance();
  ngenes = tour.size();
}

void TSPChromosome::InitializeCosts(const vector<vector<double> > cost_matrix) {
  tsp_costs = cost_matrix;
}

double TSPChromosome::evaluate(void) {
  double cost = 0;
  for (int i=0; i<(ngenes-1); i++) {
     cost += tsp_costs[tour[i]][tour[i+1]];
  }
  cost += tsp_costs[0][ngenes-1];
  if (trace) cout << "Evaluate " << repr() << " -> " << cost << endl;

  return -cost;
}


// OX crossover 
// Tour representation is Path. e.g. (1, 5, 6) equals edges (1, 5) (5 6) (6 1)
void TSPChromosome::crossover(Chromosome *chromo_mate) {
  TSPChromosome *mate = (TSPChromosome*)chromo_mate;
  vector<int> mate_tour = mate->get_tour(); 

  // 1) Choose 2 random cut points.
  int pos_one = random->rand_int(ngenes);
  int pos_two = random->rand_int(ngenes);
  while (pos_one == pos_two) 
    pos_two = random->rand_int(ngenes);
  
  int start_pos, stop_pos; // stop pos is inclusive 
  if (pos_one > pos_two) {
    start_pos = pos_two;
    stop_pos = pos_one;
  }
  else {
    start_pos = pos_one;
    stop_pos = pos_two;
  }

  if (trace) {
    cout << "**** OX Crossover ****" << endl;
    cout << "Parent 1: " + repr() << endl;  
    cout << "Parent 2: " + mate->repr() << endl;  
    cout << "1) Choose 2 random cut points: " << start_pos << " - " << stop_pos << endl;
  }

  // 2) Copy each section between the cut points 
  /*  e.g.
    p1 = (1 2 3 | 4 5 6 7 | 8 9)  // parents
    p2 = (4 5 2 | 1 8 7 6 | 9 3)

    c1 = (x x x | 4 5 6 7 | x x)  // children
    c2 = (x x x | 1 8 7 6 | x x) 
  */

  vector<int> child_one(ngenes, -1); // -1 indicates unfilled values. 
  vector<int> child_two(ngenes, -1);
  for (int i=start_pos; i<=stop_pos; i++) {
    child_one[i] = tour.at(i);
    child_two[i] = mate_tour.at(i);
  }
  
  if (trace) {
    cout << "2) Copy each section between the cut points." << endl;
    ostringstream c1_buffer, c2_buffer;
    for (int i=0; i<ngenes; i++) {  // XXX at some point make this a function.
      c1_buffer << child_one[i] << " ";
      c2_buffer << child_two[i] << " ";
    }
    cout << "Partial Offspring 1: (" << c1_buffer.str() << ")" << endl;
    cout << "Partial Offspring 2: (" << c2_buffer.str() << ")" << endl;
  } 

  /* 3) Starting with the opposite parent after the cut point.  Collect the non-duplicate genes
      and enter them into the child.
      e.g.
    p1 = (1 2 3 | 4 5 6 7 | 8 9)  // parents
    p2 = (4 5 2 | 1 8 7 6 | 9 3)

    c1 = (x x x | 4 5 6 7 | x x)  // children
    c2 = (x x x | 1 8 7 6 | x x) 
     
    for c1 use p2 = (9 3 2 1 8)
    for c2 use p1 = (9 2 3 4 5)
 
  */  

  // round up candidtes from the parents that don't occur in the children.
  int gene_p1, gene_p2;

  int idx = stop_pos+1;
  if (idx == ngenes) idx = 0;

  bool c1_dup = false;
  bool c2_dup = false;
  vector<int> p1_candidates; 
  vector<int> p2_candidates; 
  for (int i=0; i<ngenes; i++) {
    c1_dup = false;
    c2_dup = false;

    gene_p1 = tour.at(idx);
    gene_p2 = mate_tour.at(idx);         

    for (int j=start_pos; j<=stop_pos; j++) {
      if (!c1_dup && gene_p1 == child_two[j]) c1_dup = true;
      if (!c2_dup && gene_p2 == child_one[j]) c2_dup = true;
    }
    
    if (!c1_dup) p1_candidates.push_back(gene_p1);
    if (!c2_dup) p2_candidates.push_back(gene_p2);
    
    idx++;
    if (idx == ngenes) idx = 0;
  }  

  if (trace) {
    cout << "3) Starting with the opposite parent after the cut point.  Collect the non-duplicate genes "
            "and enter them into the child. " << endl;
    ostringstream c1_buffer, c2_buffer;
    for (int i=0; i<p1_candidates.size(); i++) {
      c1_buffer << p1_candidates[i] << " ";
      c2_buffer << p2_candidates[i] << " ";
    }
    cout << "Parent 1 Candidates: (" << c1_buffer.str() << ")" << endl;
    cout << "Parent 2 Candidates: (" << c2_buffer.str() << ")" << endl;
  } 

  idx = stop_pos+1;
  if (idx == ngenes) idx = 0;

  for (int i=0; i<p1_candidates.size(); i++) {
    child_one[idx] = p2_candidates[i];      
    child_two[idx] = p1_candidates[i];      

    idx++;
    if (idx == ngenes) idx = 0;
  }

  if (trace) {
    ostringstream c1_buffer, c2_buffer;
    for (int i=0; i<ngenes; i++) {
      c1_buffer << child_one[i] << " ";
      c2_buffer << child_two[i] << " ";
    }
    cout << "Offpring 1: (" << c1_buffer.str() << ")" << endl;
    cout << "Offspring 2: (" << c2_buffer.str() << ")" << endl;
    cout << endl;
  } 


  set_tour(child_one);
  mate->set_tour(child_two);

}      

void TSPChromosome::mutate(double mutate_prob) {
  
  vector<int> indices;
  vector<int> excluded_idxs;
  for (int i=0; i<ngenes; i++) {
    if (random->unit_random() < mutate_prob) indices.push_back(i);
    else                                     excluded_idxs.push_back(i);
  }
     
  if (indices.size() % 2 != 0) { // need to even out pairs of indexes.
    int r_idx = random->rand_int(excluded_idxs.size());
    indices.push_back(r_idx);
  }

  if (trace && indices.size() > 0) {
    cout << ">> Mutation for (" << repr() << ")" << endl;
  }
    
  // swap genes
  int i=0;
  int tmp_gene;
  while (i<indices.size()) {
    if (trace) cout << ">> Mutate Swaping - ";
    tmp_gene = tour[indices[i]];
    if (trace) cout << tmp_gene;
    tour[indices[i]] = tour[indices[i+1]];
    if (trace) cout << " with " << tour[indices[i+1]] << endl;
    tour[indices[i+1]] = tmp_gene;
    i += 2;
  }
  if (trace && indices.size()) cout << ">> Mutation Result (" << repr() << ")" << endl << endl;
  

}


#ifdef __TSPCHROMOSOME_TEST__

int main(void) {
  /*
  TSPChromosome tsp(5);
  cout << tsp.repr() << endl;
  tsp = TSPChromosome(13);
  cout << tsp.repr() << endl;

  TSPChromosome tsp2("1-2-3-4");
  cout << tsp2.repr() << endl;
  tsp2 = TSPChromosome("4-2-13-1-5-6-7-8-9-10");
  cout << tsp2.repr() << endl;
  */
  
  TSPChromosome *tsp3 = new TSPChromosome(10);
  TSPChromosome *tsp4 = new TSPChromosome(10);

  //cout << "TSP (Parent) 3: " << tsp3->repr() << endl;
  //cout << "TSP (Parent) 4: " << tsp4->repr() << endl;

  tsp3->turn_trace_on();
  tsp3->crossover(tsp4);  

  //cout << "TSP (Offspring) 3: " << tsp3->repr() << endl;
  //cout << "TSP (Offspring) 4: " << tsp4->repr() << endl;

  return 0;
}


#endif





