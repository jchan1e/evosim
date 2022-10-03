#ifndef WORLD_H
#define WORLD_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
//#include "agent.h"

class Creature;

using namespace std;

// world class
//
// contains grid of cells which wrap around at all 4 edges
// each cell contains some quantity of plant life (food), and the ID of up to 1 creature
// plant life follows a Conway-style game of life growth pattern, including an amount that triggers dispersing seeds randomly elsewhere on the map

struct Cell {
public:
  //int x_pos;
  //int y_pos;
  //int gridsize;
  int plant;
  int creature_id;
};

class World {
public:
  Cell** grid[2];
  int gridsize;
  vector<Creature> creatures;
  int n_creatures;
  int ping; // read from ping
  int pong; // write to pong
  int timestep;

  World(int size);
  ~World();

  void add_creature(float* genome, int num_connections);

  void save(char* filename);
  void save(ofstream *file);

  void advance();

  void update_creatures();
  void update_plants();
};

#endif
