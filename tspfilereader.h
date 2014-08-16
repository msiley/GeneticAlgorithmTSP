#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <cassert>
using namespace std;

#ifndef __TSPFILEREADER__
#define __TSPFILEREADER__

class TSPFileReader {
private:
  string filename;
  vector<vector<double> > tour;  
public:
  TSPFileReader(const char *filename);
  TSPFileReader(string &filename);

  vector<vector<double> > &read(void); 
  void print_tour(void);
  
  static double distance(double x1, double y1, double x2, double  y2); 

};

#endif


