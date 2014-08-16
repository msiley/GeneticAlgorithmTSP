#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include "random.h"
using namespace std;

#ifndef __RANDOMBITS__
#define __RANDOMBITS__

class RandomBits: public Random {
private:
  Random *random;
public:
  RandomBits(void);
  string make_random_bit(void); // returns a random "0" or "1" (yep in string form).
  string make_random_bits(int nbits); // returns a string of random bits.
};

#endif
