#include "random.h"
using namespace std;

/* 
Random - a Singleton class used to generate various types of psuedo-random numbers.
*/

Random* Random::pinstance = 0;

Random* Random::Instance(void) {
  if (pinstance == 0) 
    pinstance = new Random; 
  return pinstance; 
}

Random::Random(void) {
  time_t tt = time(NULL);
  struct tm *t = localtime(&tt);
  srand(t->tm_sec);
  this->unit_random();
}

// returns a random number between 0 and 1.
double Random::unit_random(void) {
  return (double)rand() / (double)RAND_MAX;
}

// returns a random number between [0, stop)
int Random::rand_int(int stop) { // stop is exclusive. starts with 0 ends with stop-1.
  if (stop == 0) return 0;
  double r = this->unit_random() * stop-1;
  int r_int = (int)r;
  return (r - r_int) > 0.5 ? r_int + 1: r_int;
}

Random::~Random(void) {}

#ifdef __RANDOM_TEST__

int main(void) {
  Random *r = Random::Instance();
  for (int i=0; i<1000; i++) 
    assert(0 <= r->rand_int(5) <= 5);

  return 0;
}


#endif
