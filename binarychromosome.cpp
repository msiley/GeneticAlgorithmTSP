#include "binarychromosome.h"
 

void print_list(vector<BinaryChromosome> &lst) {
  for (int i=0; i < lst.size(); i++) {
    cout << "[" << (i+1) << "](" << lst.at(i) << "), ";
    if ((i+1) % 5 == 0) cout << endl;
  }
  cout << endl;
}


string& BinaryChromosome::repr(void) {
  return get_bitstring();
}


Chromosome* BinaryChromosome::clone(void) {
  return new BinaryChromosome(get_bitstring());
}


// creates a chromosome randomly selecting the genes.
BinaryChromosome::BinaryChromosome(int nbits) {
  random = Random::Instance();
  RandomBits rand = RandomBits(); 
  bitstring = rand.make_random_bits(nbits);
  this->nbits = nbits;
  trace = false;
}

// creates a chromosome with a previously determined set of genes.
BinaryChromosome::BinaryChromosome(string bitstring) { 
  random = Random::Instance();
  this->bitstring = bitstring;
  this->nbits = bitstring.length();
  trace = false;
}

double BinaryChromosome::set_bitstring(string bitstring) { 
  this->bitstring = bitstring; 
  nbits = this->bitstring.length();
}

double BinaryChromosome::evaluate(void) {
  return binary_to_decimal(bitstring);
}

void BinaryChromosome::crossover(Chromosome *chromo_mate) {
  BinaryChromosome *mate = (BinaryChromosome*)chromo_mate;
  int pos = random->rand_int(this->nbits);
  string c_one = this->bitstring;
  string c_two = mate->get_bitstring();

  this->set_bitstring(c_one.substr(0, pos) + c_two.substr(pos, this->nbits-pos));
  mate->set_bitstring(c_two.substr(0, pos) + c_one.substr(pos, this->nbits-pos));
  
  if (trace) {
    cout << "Crossover Operations: Position = " << pos << endl;
    cout << "Parents v(" << c_one << ") and v(" << c_two << ")" << endl;
    cout << "Offspring:" << endl;

    string buffer;
    for (int i=0; i < pos; i++) buffer += " ";

    cout << "\tv(" << this << ")" << endl; 
    cout << "\t  " << buffer << "^" << endl;
    cout << "\tv(" << (*mate) << ")" << endl;
    cout << "\t  " << buffer << "^" << endl;
    cout << endl;
  }
}      

void BinaryChromosome::mutate(double mutate_prob) {
  string orig_bitstring = bitstring;
  bool mutated = false;
  vector<int> changed_bits;
  for (int i=0; i < this->nbits; i++) {
    if (random->unit_random() < mutate_prob) {
      mutated = true;
      if (bitstring[i] == '1') bitstring[i] = '0';
      else                     bitstring[i] = '1';
      if (trace) changed_bits.push_back(i);
    }
  }
  if (trace && mutated) {
    cout << "Mutation Operation for: v(" << orig_bitstring << ")" << endl;
    cout << "Bit Position(s) Changed: ";
    for (int i=0; i < changed_bits.size(); i++) 
      cout << changed_bits[i] << " ";
    cout << "Result = v(" << bitstring << ")" << endl;
  }
}


#ifdef __BINARYCHROMOSOME_TEST__

int main(void) {
  string orig_bc1 = "000000";
  BinaryChromosome *bc1 = new BinaryChromosome(orig_bc1);
  assert(bc1->repr() == "000000");
  string orig_bc2 = "111111";
  BinaryChromosome *bc2 = new BinaryChromosome(orig_bc2);
  
  assert(bc1->evaluate() == 0);

  bc1->crossover(bc2);

  assert(bc1->repr() != orig_bc1);
  assert(bc2->repr() != orig_bc2);


  Chromosome *bc3 = bc1->clone();
  assert(bc3->repr() == bc1->repr());

  return 0;
}

#endif








