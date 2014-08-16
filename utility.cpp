#include "utility.h"

// Utility Functions

void print_list(vector<double> &lst) {
  for (int i=0; i < lst.size(); i++) {
    cout << "[" << (i+1) << "](" << lst.at(i) << "), ";
    if ((i+1) % 5 == 0) cout << endl;
  }
  cout << endl;
}

void print_list(vector<int> &lst) {
  for (int i=0; i < lst.size(); i++) {
    cout << "[" << (i+1) << "](" << lst.at(i) << "), ";
    if ((i+1) % 5 == 0) cout << endl;
  }
  cout << endl;
}

double binary_to_decimal(string &bstring) {
  int nbits = bstring.length();
  int n = nbits-1;
  double total = 0;
  int bit = 0;
  for (int i=0; i < nbits; i++) {
    bit = bstring[i] == '1' ? 1 : 0;
    total +=  (double)bit * (double)pow((double)2, (double)n);
    n -= 1;
  }
  return total;
}

#ifdef __UTILITY_TEST__

int main(void) {
  string b = "10000000000";
  double f = binary_to_decimal(b);
  assert(f == 1024);

  return 0;
}

#endif

