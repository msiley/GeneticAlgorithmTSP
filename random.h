#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
using namespace std;

/* 
Random - a Singleton class used to generate various types of psuedo-random numbers.
*/

#ifndef __RANDOM__
#define __RANDOM__

class Random { // Singleton
public:
    static Random* Instance(); // use this instead of constructor.
    double unit_random(void); // returns a float between [0, 1]
    int rand_int(int); // returns an integer between [0, stop)
    ~Random(void); // may need to use reference counting for destructor.
protected:
    Random(void);
private:
    static Random* pinstance;
};

#endif
