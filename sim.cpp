

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "world.h"

using namespace std;


int main(int argc, char *argv[])
{
  if (argc < 3) {
    cout << "Usage: sim [boardsize] <population size> <num_steps> <outfilename>.sim\n";
    return 0;
  }

  int n_boardsize = 128;
  if (argc == 5)
    n_boardsize = atoi(argv[argc-4]);
  //int n_population = atoii(argv[argc-3]);
  //if (argc >= 4)
  //  n_population = atoi(argv[argc-3]);
  int n_steps = atoi(argv[argc-2]);

  // initialize
  World* W = new World(n_boardsize);
  // randomly seed plants around the board
  W->grid[0][n_boardsize/2][n_boardsize/2].plant = 3;
  W->grid[1][n_boardsize/2][n_boardsize/2].plant = 3;
  //for (int i=0; i < n_boardsize/2; ++i) {
  //  int x = rand() % n_boardsize;
  //  int y = rand() % n_boardsize;
  //  W->grid[0][x][y].plant = W->grid[0][x][y].plant + 1 % 10;
  //  W->grid[1][x][y].plant = W->grid[0][x][y].plant;
  //}

  // start outfile
  ofstream file (argv[argc-1],ios::out|ios::binary);
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
    // update creatures
    // update plants
    W->update_plants(0);
    // write output as we go
    W->save(&file);
  }

  file.close();

  delete W;
}
