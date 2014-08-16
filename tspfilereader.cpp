#include "tspfilereader.h"


TSPFileReader::TSPFileReader(const char *filename) {
  this->filename = string(filename);
}


TSPFileReader::TSPFileReader(string &filename) {
  this->filename = filename;
}

double TSPFileReader::distance(double x1, double y1, double x2, double  y2) {
  double xd = x1 - x2;
  double yd = y1 - y2;
  return sqrt(xd*xd + yd*yd);
}


vector<vector<double> > &TSPFileReader::read(void) {

  vector<vector<double> > coords;

  ifstream fin(filename.c_str());
  istringstream ins;
  string buffer;
  double x, y;
  int i;
  bool data_started = false;
  while (getline(fin, buffer)) {
    if (buffer == "EOF") break;
    if (data_started) {
      ins.str(buffer);
      ins >> i >> x >> y; 
      ins.clear();
        
      //cout << i << " " << x << " : " << y << endl;

      vector<double> row;
      row.push_back(x);
      row.push_back(y);
      coords.push_back(row);
      buffer = "";
    }
    if (!data_started && buffer == "NODE_COORD_SECTION") 
      data_started = true;

  }
  fin.close();

  /*
  cout << endl << endl;
  for (int i=0; i<coords.size(); i++) {
      cout << i << " " << coords[i][0] << " : " << coords[i][1] << endl;
  }
  */

  int ncities = coords.size();
  double x1, y1, x2, y2;
  double dist;
  double xd, yd;
  for (int i=0; i<ncities; i++) {
    x1 = coords[i][0];
    y1 = coords[i][1];
    vector<double> row;
    for (int j=0; j<ncities;  j++) {
      x2 = coords[j][0];
      y2 = coords[j][1];
      if (x1 == x2 && y1 == y2) 
        row.push_back(0);
      else {
        //double dist2 = distance(x1, y1, x2, y2);
        //dist = distance(x1, y1, x2, y2);
        //cout << dist << endl;
        xd = x1 - x2;
        yd = y1 - y2;
        dist = sqrt(xd*xd + yd*yd);
        //cout << dist << endl;
        row.push_back(dist);
      }
    }
    tour.push_back(row);    
  }
      
  return tour;

}

void TSPFileReader::print_tour(void) {
  int ncities = tour.size();
  cout << "NCities: " << ncities << endl;
  double cost;
  for (int i=0; i<ncities; i++) {
    cout << "[";
    for (int j=0; j<ncities;  j++) {
      cost = tour[i][j];
      cout << cost;
      if (j != ncities-1) cout << ", ";
    }
    cout << "]" << endl;
  }
}







