#include "randombits.h"


RandomBits::RandomBits(void) {
  random = Random::Instance();
}

string RandomBits::make_random_bit(void) {
  if (random->unit_random() > 0.5) return "0";
  else                             return "1";
}

string RandomBits::make_random_bits(int nbits) {
  string bit_list = "";
  for (int i=0; i < nbits; i++)
    bit_list += this->make_random_bit();
  return bit_list;
}

#ifdef  __RANDOMBITS_TEST__

int main(void) {

  RandomBits rb = RandomBits();
  string bstring;
  for (int i=0; i<10; i++) {
    bstring = rb.make_random_bits(i);
    assert(bstring.length() == i);
  }

  return 0;
}

#endif

