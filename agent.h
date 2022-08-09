#ifndef AGENT_H
#define AGENT_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include "world.h"

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

struct Brain {
public:
  float inputs[10];
  float neurons[2];
  float outputs[10];
  vector<Connection> conns;
  //Brain(Connection* Conn_list, int n) {
  //}
};

class Creature {
public:
  World* w;
  Brain brain;
  int x, y;
  int d_x, d_y;
  float energy;

  Creature(World* W, float* genome) {
    w = W;
    //brain = B;
    energy = 100.0;
    // everything else roll randomly
  }

  Creature(World* W, int X, int Y, int D_X, int D_Y, float E, float* genome) {
    w = W;
    x = X;
    y = Y;
    d_x = D_X;
    d_y = D_Y;
    energy = E;
    //brain = B;
  }

  ~Creature() {
    // clean up pointers and whatnot
  }

  void move(int argument) {
    ;
  }
};

#endif
