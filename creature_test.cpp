
#include <fstream>
#include <iostream>
#include "world.h"
#include "agent.h"

int main(int argc, char *argv[]) {
  World W(128);
  ifstream infile(argv[1], ifstream::binary);
  int n_conns = 0;
  infile.seekg(0);
  infile.read((char*)&n_conns, sizeof(int));
  float genome[13+3*n_conns];
  infile.seekg(sizeof(int));
  infile.read((char*)&genome[0], 3*sizeof(float));
  infile.seekg(sizeof(int) + 3*sizeof(float));
  infile.read((char*)&genome[3], 10*sizeof(float));
  infile.seekg(sizeof(int) + 13*sizeof(float));
  infile.read((char*)&genome[13], 3*n_conns*sizeof(float));

  infile.close();

  cout << "genome values:\n";
  cout << n_conns << endl;
  for (int i=0; i < 13; ++i) {
    cout << i << " " << genome[i] << endl;
  }
  for (int i=13; i < 13+n_conns*3; i += 3) {
    cout << (i-13)/3 << " " << genome[i] << " " << genome[i+1] << " " << genome[i+2] << endl;
  }

  //W.add_creature(&genome[0], n_conns);
}
