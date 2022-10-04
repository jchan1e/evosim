
#include "agent.h"
#include "world.h"

using namespace std;

// agent/creature class
//

//struct Brain {
//  // inputs: all float values [-1.0, 1.0]
//  // 0 age [0.0, 1.0]
//  // 1 random generator
//  // 2 oscillator
//  // 3 x position
//  // 4 y position
//  // 5 facing x
//  // 6 facing y
//  // 7 Last x movement
//  // 8 Last y movement
//  // 9 last move blocked
//  // 10 blockage front
//  // 11 blockage left/right
//  // 12 plant value in front
//  // 13 plant gradient front
//  // 14 plant gradient left/right
//  // 15 population density
//  // 16 population gradient front
//  // 17 population gradient left/right
//  //
//  // outputs: float value normalized by tanh
//  //          [0.2, 0.8] treated as probability
//  //          [0.0, 0.2] and [0.8, 1.0] rounded to 0 and 1
//  // 0 set oscillator period [0.0, 1.0] -> [2, 60] frames per rev
//  // 1 eat plant (front)
//  // 2 move random
//  // 3 move forward
//  // 4 move reverse
//  // 5 move left
//  // 6 move right
//  // 7 rotate left
//  // 8 rotate right
//  // 9 attack forward (disabled for now)
//};

void Brain::eval() {
  // reset to biases
  for (int i=0; i < 3; ++i)
    neurons[i] = b_neurons[i];
  for (int i=0; i < 10; ++i)
    actions[i] = b_actions[i];

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
  osc_freq = min(max(actions[0] * M_PI, 0.0166), 1.0);
}

Creature::Creature(World* W, float* genome, int num_conns, int id) {
  w = W;
  energy = 100.0;
  ID = id;
  x = rand()%W->gridsize;
  y = rand()%W->gridsize;
  direction = rand()%8;
  set_dir();
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
}

Creature::Creature(World* W, int X, int Y, int D_X, int D_Y, float E, float* genome, int num_comms, int id) {
  w = W;
  x = X;
  y = Y;
  d_x = D_X;
  d_y = D_Y;
  energy = E;
  ID = id;
  //brain = B;
  for (int i=0; i < 3; ++i) {
    brain.b_neurons[i] = genome[i];
  }
  for (int i=0; i < 10; ++i) {
    brain.b_actions[i] = genome[3+i];
  }
  for (int i=0; i < num_comms; ++i) {
    int I = int(genome[3*i]);
    int O = int(genome[3*i+1]);
    float V = genome[3*i+2];
    brain.conns.push_back(Connection(I,O,V));
  }
}

//~Creature() {
//  // clean up pointers and whatnot
//}

void Creature::set_dir() {
  switch(direction) {
    case 0:
      d_x = 0;
      d_y = 1;
      break;
    case 1:
      d_x = 1;
      d_y = 1;
      break;
    case 2:
      d_x = 1;
      d_y = 0;
      break;
    case 3:
      d_x = 1;
      d_y = -1;
      break;
    case 4:
      d_x = 0;
      d_y = -1;
      break;
    case 5:
      d_x = -1;
      d_y = -1;
      break;
    case 6:
      d_x = -1;
      d_y = 0;
      break;
    case 7:
      d_x = -1;
      d_y = 1;
      break;
    default:
      d_x = 0;
      d_y = 1;
      break;
  }
}


void Creature::eat_plant(int t_x, int t_y) {
  if (abs(t_x-x) <= 1 && abs(t_y-y) <= 1 && w->grid[1][x][y].plant > 0) {
    w->grid[1][x][y].plant -= 1;
    energy += 10.0;
  }
}

void Creature::move_random() {
  int dir = rand()%4;
  switch(dir) {
    case 0:
      move_forward();
      break;
    case 1:
      move_reverse();
      break;
    case 2:
      move_left();
      break;
    case 3:
      move_right();
      break;
  }
}

void Creature::move_forward() {
  if (0 < x+d_x && x+d_x < w->gridsize
   && 0 < y+d_y && y+d_y < w->gridsize
   && w->grid[1][x+d_x][y+d_y].creature_id == -1) {
    w->grid[1][x+d_x][y+d_y].creature_id = ID;
    w->grid[1][x][y].creature_id = -1;
    x += d_x;
    y += d_y;
    brain.sensors[9] = 0.0; // last move not blocked
  }
  else {
    brain.sensors[9] = 1.0; // last move blocked
  }
  brain.sensors[7] = (float)d_x;
  brain.sensors[8] = (float)d_y;
}

void Creature::move_reverse() {
  d_x = -d_x;
  d_y = -d_y;
  move_forward();
  d_x = -d_x;
  d_y = -d_y;
}

void Creature::move_left() {
  direction = (direction+8-2)%8;
  set_dir();
  move_forward();
  direction = (direction+8+2)%8;
  set_dir();
}

void Creature::move_right() {
  direction = (direction+8+2)%8;
  set_dir();
  move_forward();
  direction = (direction+8-2)%8;
  set_dir();
}

void Creature::rotate_left() {
  direction = (direction+8-1)%8;
  set_dir();
}

void Creature::rotate_right() {
  direction = (direction+8+1)%8;
  set_dir();
}

// 90* right : x+dy, y-dx
// 90* left  : x-dy, y+dx

void Creature::advance() {
  // set brain inputs
  brain.sensors[0]  = w->timestep / 10000; // for now sim duration is 10,000 steps
  brain.sensors[1]  = (float)rand() / RAND_MAX;
  brain.sensors[2]  = sin(w->timestep * brain.osc_freq);
  brain.sensors[3]  = (float)x / w->gridsize;
  brain.sensors[4]  = (float)y / w->gridsize;
  brain.sensors[5]  = (float)d_x;
  brain.sensors[6]  = (float)d_y;
  //brain.sensors[7]  = // set during movement
  //brain.sensors[8]  = // set during movement
  //brain.sensors[9]  = // set during movement
  if (   0 <= x+d_x && x+d_x < w->gridsize
      && 0 <= y+d_y && y+d_y < w->gridsize
      && w->grid[w->ping][x+d_x][y+d_y].creature_id == -1)
    brain.sensors[10] = 0.0;
  else
    brain.sensors[10] = 1.0;
  if (   0 <= x+d_y && x+d_y < w->gridsize
      && 0 <= x-d_y && x-d_y < w->gridsize
      && 0 <= y+d_x && y+d_x < w->gridsize
      && 0 <= y-d_x && y-d_x < w->gridsize
      && w->grid[w->ping][x+d_y][y-d_x].creature_id == -1
      && w->grid[w->ping][x-d_y][y+d_x].creature_id == -1)
    brain.sensors[11] = 0.0;
  else
    brain.sensors[11] = 1.0;
  brain.sensors[12] = w->grid[w->ping][x+d_x][y+d_y].plant / 9.0;
  int grid[9];
  int grad0, grad1, grad2, grad3;
  for (int ix=-1; ix < 2; ++ix) {
    for (int iy=-1; iy < 2; ++iy) {
      if (0 <= x+ix && x+ix < w->gridsize && 0 <= y+iy && y+iy < w->gridsize) {
        grid[(iy+1)*3 + (ix+1)] = w->grid[w->ping][x+ix][y+iy].plant;
      }
    }
  }
  grad0 = grid[0] + grid[1] + grid[2] - grid[6] - grid[7] - grid[8];
  grad1 = grid[1] + grid[2] + grid[5] - grid[3] - grid[6] - grid[7];
  grad2 = grid[2] + grid[5] + grid[8] - grid[0] - grid[3] - grid[6];
  grad3 = grid[5] + grid[7] + grid[8] - grid[0] - grid[1] - grid[3];
  switch(direction) {
    case 0:
      brain.sensors[13] = grad0/54.0;
      brain.sensors[14] = grad2/54.0;
      break;
    case 4:
      brain.sensors[13] = -grad0/54.0;
      brain.sensors[14] = -grad2/54.0;
      break;
    case 1:
      brain.sensors[13] = grad1/54.0;
      brain.sensors[14] = grad3/54.0;
      break;
    case 5:
      brain.sensors[13] = -grad1/54.0;
      brain.sensors[14] = -grad3/54.0;
      break;
    case 2:
      brain.sensors[13] = grad2/54.0;
      brain.sensors[14] = -grad0/54.0;
      break;
    case 6:
      brain.sensors[13] = -grad2/54.0;
      brain.sensors[14] = grad0/54.0;
      break;
    case 3:
      brain.sensors[13] = grad3/54.0;
      brain.sensors[14] = -grad1/54.0;
      break;
    case 7:
      brain.sensors[13] = -grad3/54.0;
      brain.sensors[14] = grad1/54.0;
      break;
  }
  for (int ix=-1; ix < 2; ++ix) {
    for (int iy=-1; iy < 2; ++iy) {
      if (0 <= x+ix && x+ix < w->gridsize && 0 <= y+iy && y+iy < w->gridsize) {
        grid[(iy+1)*3 + (ix+1)] = w->grid[w->ping][x+ix][y+iy].creature_id >= 0 ? 1 : 0;
      }
    }
  }
  grad0 = grid[0] + grid[1] + grid[2] - grid[6] - grid[7] - grid[8];
  grad1 = grid[1] + grid[2] + grid[5] - grid[3] - grid[6] - grid[7];
  grad2 = grid[2] + grid[5] + grid[8] - grid[0] - grid[3] - grid[6];
  grad3 = grid[5] + grid[7] + grid[8] - grid[0] - grid[1] - grid[3];

  brain.sensors[15] = (grid[0] + grid[1] + grid[2] + grid[3] + grid[5] + grid[6] + grid[7] + grid[8])/8.0;
  switch(direction) {
    case 0:
      brain.sensors[16] = grad0/6.0;
      brain.sensors[17] = grad2/6.0;
      break;
    case 4:
      brain.sensors[16] = -grad0/6.0;
      brain.sensors[17] = -grad2/6.0;
      break;
    case 1:
      brain.sensors[16] = grad1/6.0;
      brain.sensors[17] = grad3/6.0;
      break;
    case 5:
      brain.sensors[16] = -grad1/6.0;
      brain.sensors[17] = -grad3/6.0;
      break;
    case 2:
      brain.sensors[16] = grad2/6.0;
      brain.sensors[17] = -grad0/6.0;
      break;
    case 6:
      brain.sensors[16] = -grad2/6.0;
      brain.sensors[17] = grad0/6.0;
      break;
    case 3:
      brain.sensors[16] = grad3/6.0;
      brain.sensors[17] = -grad1/6.0;
      break;
    case 7:
      brain.sensors[16] = -grad3/6.0;
      brain.sensors[17] = grad1/6.0;
      break;
  }

  brain.eval();

  // check brain outputs, determine actions to be taken
  // 0 set oscillator period [0.0, 1.0] -> [2, 60] frames per rev
  // 1 eat plant (front)
  // 2 move random
  // 3 move forward
  // 4 move reverse
  // 5 move left
  // 6 move right
  // 7 rotate left
  // 8 rotate right

  // Find highest value among action neurons
  int action_id = 1;
  for (int i=2; i < 9; ++i) {
    if (brain.actions[i] > brain.actions[action_id])
      action_id = i;
  }
  switch(action_id) {
    case 1:
      eat_plant(x+d_x,y+d_y);
      break;
    case 2:
      move_random();
      break;
    case 3:
      move_forward();
      break;
    case 4:
      move_reverse();
      break;
    case 5:
      move_left();
      break;
    case 6:
      move_right();
      break;
    case 7:
      rotate_left();
      break;
    case 8:
      rotate_right();
      break;
  }
}


