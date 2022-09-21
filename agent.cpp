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
  //          [0.2, 0.8] treated as probability
  //          [0.0, 0.2] and [0.8, 1.0] rounded to 0 and 1
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
public:
  float sensors[18];
  float neurons[3];
  float actions[10];
  float b_neurons[3];
  float b_actions[10];
  vector<Connection> conns;
  //Brain(Connection* Conn_list, int n) {
  //  for (int i=0; i < n ; ++i) {
  //    conns.push_back(Conn_list[i]);
  //  }
  //}
  void eval() {
    // reset to biases
    for (int i=0; i < 3; ++i)
      neurons[i] = b_neurons[i];
    for (int i=0; i < 10; ++i)
      actions[i] = b_neurons[i];

    for (Connection C : conns) {
      // propagate in the order the connections are stored
      float* in_arr = &sensors[0];
      float* out_arr = &actions[0];
      int I = C.input;
      int O = C.output;
      float S = C.strength;
      // if I or O < 0, then it refers to a hidden layer neuron rather than a sensor or action
      if (I < 0) {
        I = -I - 1;
        in_arr = &neurons[0];
      }
      if (O < 0) {
        O = -O - 1;
        out_arr = &neurons[0];
      }
      out_arr[O] += in_arr[I] * S;
    }
  }
};

class Creature {
public:
  World* w;
  Brain brain;
  int x, y;
  int d_x, d_y;
  float energy;

  Creature(World* W, float* genome, int num_conns) {
    w = W;
    //brain = B;
    // biases first
    for (int i=0; i < 3; ++i) {
      brain.b_neurons[i] = genome[i];
    }
    for (int i=0; i < 10; ++i) {
      brain.b_actions[i] = genome[3+i];
    }
    for (int i=0; i < num_conns; ++i) {
      int I = int(genome[13+3*i]);
      int O = int(genome[13+3*i+1]);
      float V = genome[13+3*i+2];
      brain.conns.push_back(Connection(I,O,V));
    }
    energy = 100.0;
    // everything else roll randomly
  }

  Creature(World* W, int X, int Y, int D_X, int D_Y, float E, float* genome, int len_genome) {
    w = W;
    x = X;
    y = Y;
    d_x = D_X;
    d_y = D_Y;
    energy = E;
    //brain = B;
    for (int i=0; i < len_genome; ++i) {
      int I = int(genome[3*i]);
      int O = int(genome[3*i+1]);
      float V = genome[3*i+2];
      brain.conns.push_back(Connection(I,O,V));
    }
    energy = 100.0;
  }

  ~Creature() {
    // clean up pointers and whatnot
  }

  void move_f() {
  }
};

#endif
