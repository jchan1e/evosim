#ifndef AGENT_H
#define AGENT_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
//#include "world.h"

class World;

using namespace std;

// agent/creature class
//

struct Connection {
public:
  int input;
  int output;
  float strength;
  Connection(int i, int o, float s) {
    input = i;
    output = o;
    strength = s;
  }
};

  // inputs: all float values [-1.0, 1.0]
  // 0 age [0.0, 1.0]
  // 1 random generator
  // 2 oscillator
  // 3 x position
  // 4 y position
  // 5 facing x
  // 6 facing y
  // 7 Last x movement
  // 8 Last y movement
  // 9 last move blocked
  // 10 blockage front
  // 11 blockage left/right
  // 12 plant value in front
  // 13 plant gradient front
  // 14 plant gradient left/right
  // 15 population density
  // 16 population gradient front
  // 17 population gradient left/right
  //
  // outputs: float value normalized by tanh
  //          [0.0, 1.0] treated as probability
  // 0 set oscillator period [0.0, 1.0] -> [2, 60] frames per rev
  // 1 eat plant (front)
  // 2 move random
  // 3 move forward
  // 4 move reverse
  // 5 move left
  // 6 move right
  // 7 rotate left
  // 8 rotate right
  // 9 attack forward (disabled for now)
struct Brain {
public:
  float sensors[18];
  float neurons[3];
  float actions[10];
  float b_neurons[3];
  float b_actions[10];
  vector<Connection> conns;
  float osc_freq;

  void eval();
};

class Creature {
public:
  World* w;
  Brain brain;
  int x, y;
  int d_x, d_y;
  int direction;
  float energy;
  int ID;

  Creature();
  Creature(World* W, float* genome, int num_conns, int id);
  Creature(World* W, int X, int Y, int D_X, int D_Y, float E, float* genome, int len_genome, int id);
  //~Creature();

  void set_dir();
  void eat_plant(int t_x, int t_y);

  void move_random();
  void move_forward();
  void move_reverse();
  void move_left();
  void move_right();
  void rotate_left();
  void rotate_right();

  void advance();

};

#endif
