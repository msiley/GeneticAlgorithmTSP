#include "chromosome.h"

ostream& operator<<(ostream& os, Chromosome &chromo) {
  os << chromo.repr();
  return os;
}

