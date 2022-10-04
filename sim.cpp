

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "agent.h"
#include "world.h"

using namespace std;


int main(int argc, char *argv[])
{
  if (argc < 3) {
    cout << "Usage: sim <boardsize> <population size> <num_steps> <outfilename>.sim <genome file 01> ...\n";
    return 0;
  }

  //int n_boardsize = 128;
  //if (argc == 5)
  int n_boardsize = atoi(argv[1]);
  int n_population = atoi(argv[2]);
  //if (argc >= 4)
  //  n_population = atoi(argv[argc-3]);
  int n_steps = atoi(argv[3]);

  // initialize
  World* W = new World(n_boardsize);
  // randomly seed plants around the board
  W->grid[0][n_boardsize/2][n_boardsize/2].plant = 1;
  W->grid[1][n_boardsize/2][n_boardsize/2].plant = 1;
  //for (int i=0; i < n_boardsize/2; ++i) {
  //  int x = rand() % n_boardsize;
  //  int y = rand() % n_boardsize;
  //  W->grid[0][x][y].plant = W->grid[0][x][y].plant + 1 % 10;
  //  W->grid[1][x][y].plant = W->grid[0][x][y].plant;
  //}

  // initialize creatures
  for (int i=0; i < n_population; ++i) {
    // read file
    ifstream infile (argv[5+i], ifstream::binary);
    int n_conns = 0;
    infile.seekg(0);
    infile.read((char*)&n_conns, sizeof(int));
    float genome[n_conns+13];
    infile.seekg(sizeof(int));
    infile.read((char*)&genome[0], 3*sizeof(float));
    infile.seekg(sizeof(int) + 3*sizeof(float));
    infile.read((char*)&genome[3], 10*sizeof(float));
    infile.seekg(sizeof(int) + 13*sizeof(float));
    infile.read((char*)&genome[13], 3*n_conns*sizeof(float));
    // add to world
    W->add_creature(&genome[0], n_conns);

    infile.close();
  }

  // start outfile
  ofstream file (argv[4],ios::out|ios::binary);
  if (!file.is_open()) {
    cerr << "Could not open file: " << argv[argc-1] << endl;
    return 1;
  }
  // begin with boardsize and first frame
  file.write((char*)&n_boardsize, sizeof(int));
  file.write((char*)&n_steps, sizeof(int));
  for (int i=0; i < n_boardsize; ++i) {
    file.write((char*)W->grid[W->ping][i], n_boardsize*sizeof(Cell));
  }

  // run the simulation
  for (int frame=1; frame < n_steps; ++frame) {
    //// update creatures
    //w->update_creatures();
    //// update plants
    //W->update_plants();
    // advance sim state
    W->advance();
    // write output as we go
    W->save(&file);
  }

  file.close();

  delete W;
}
