#ifndef WORLD_H
#define WORLD_H

#include <cstdlib>
#include <iostream>
//#include "agent.h"

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
  int ping;
  int pong;

  World(int size) {
    gridsize = size;
    ping = 0;
    pong = 1;
    grid[ping] = new Cell*[gridsize];
    grid[pong] = new Cell*[gridsize];
    for (int i=0; i < gridsize; ++i) {
      grid[ping][i] = new Cell[gridsize];
      grid[pong][i] = new Cell[gridsize];
      for (int j=0; j < gridsize; ++j) {
        grid[ping][i][j].plant = 0; // rand() % 10;
        grid[pong][i][j].plant = grid[ping][i][j].plant;
      }
    }
  }

  ~World() {
    for (int i=0; i < gridsize; ++i) {
      delete grid[ping][i];
      delete grid[pong][i];
    }
    delete grid[ping];
    delete grid[pong];
  }

  void save(char* filename) {
    ofstream outfile(filename, ofstream::binary);
    if (!outfile.is_open())
      cerr << "could not open file: " << filename << endl;
    else {
      // griddsize
      outfile.write((char*)&gridsize, sizeof(int));
      // creatures
      //outfile.write((char*)&n_creatures, sizeof(int));
      //outfile.write((char*)&creatures[0], n_creatures*sizeof(Creature));
      // grid state
      for (int i=0; i < gridsize; ++i) {
        outfile.write((char*)grid[ping][i], gridsize*sizeof(Cell));
      }
      outfile.close();
    }
  }

  void save(ofstream *file) {
    // this function is intended for progressive saves in simulation mode
    // it assumes the array size headers were already written
    if (!file->is_open())
      cerr << "could not open file\n";
    else {
      // creatures
      //file->write((char*&creatures[0], n_creatures*sizeof(Creature));
      // grid state
      for (int i=0; i < gridsize; ++i) {
        file->write((char*)grid[ping][i], gridsize*sizeof(Cell));
      }
    }
  }
//  void update_plants(int input) {
//    for (int i=0; i < gridsize; ++i) {
//      for (int j=0; j < gridsize; ++j) {
//        //Cell* C = &grid[ping][i][j];
//        Cell* C_1 = &grid[pong][i][j];
//        C_1->plant = ((i+j)/2)%10;
//      }
//    }
//    ping = pong;
//    pong = 1 - pong;
//  }

  void update_plants() {
    for (int i=0; i < gridsize; ++i) {
      for (int j=0; j < gridsize; ++j) {
        grid[pong][i][j].plant = grid[ping][i][j].plant;
      }
    }
    for (int i=0; i < gridsize; ++i) {
      for (int j=0; j < gridsize; ++j) {
        Cell* C = &grid[ping][i][j];
        Cell* C_1 = &grid[pong][i][j];
        if (rand()%8 != 0) {
          //C_1->plant = C->plant;
          continue;
        }
        else {
          // plant level 1-3 = grass, grows whether there are neighbors or not and seeds nearby cells
          if (C->plant >= 1  && C->plant <= 3) {
            if (C->plant < 3) {
              C_1->plant = C->plant + 1;
            }
            else { // C->plant == 3
              // check neighbors to see if it can be upgraded to a bush
              int adj = 0;
              //if (grid[ping][(i-2+gridsize)%gridsize][(j-2+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i-2+gridsize)%gridsize][(j-1+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i-2+gridsize)%gridsize][(j+0+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i-2+gridsize)%gridsize][(j+1+gridsize)%gridsize].plant == 3) adj++;
              //if (grid[ping][(i-2+gridsize)%gridsize][(j+2+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i-1+gridsize)%gridsize][(j-2+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i-1+gridsize)%gridsize][(j-1+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i-1+gridsize)%gridsize][(j+0+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i-1+gridsize)%gridsize][(j+1+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i-1+gridsize)%gridsize][(j+2+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i+0+gridsize)%gridsize][(j-2+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i+0+gridsize)%gridsize][(j-1+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i+0+gridsize)%gridsize][(j+1+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i+0+gridsize)%gridsize][(j+2+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i+1+gridsize)%gridsize][(j-2+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i+1+gridsize)%gridsize][(j-1+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i+1+gridsize)%gridsize][(j+0+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i+1+gridsize)%gridsize][(j+1+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i+1+gridsize)%gridsize][(j+2+gridsize)%gridsize].plant == 3) adj++;
              //if (grid[ping][(i+2+gridsize)%gridsize][(j-2+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i+2+gridsize)%gridsize][(j-1+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i+2+gridsize)%gridsize][(j+0+gridsize)%gridsize].plant == 3) adj++;
              if (grid[ping][(i+2+gridsize)%gridsize][(j+1+gridsize)%gridsize].plant == 3) adj++;
              //if (grid[ping][(i+2+gridsize)%gridsize][(j+2+gridsize)%gridsize].plant == 3) adj++;
              if (adj == 20) { // if exactly 20 adjacent neighbors, bushes can spawn
                C_1->plant = C->plant + 1;
              }
              else { // otherwise seed nearby cell
                int s_i = ((i-2 + rand()%5)+gridsize)%gridsize;
                int s_j = ((j-2 + rand()%5)+gridsize)%gridsize;
                if (grid[ping][s_i][s_j].plant < 1)
                  grid[pong][s_i][s_j].plant = 1;
              }
            }
          }

          // plant level 4-6 = bush
          else if (C->plant >= 4 && C->plant <= 6) {
            bool grow = false;
            // check neighbors for conway conditions
            int adj = 0;
            if (grid[ping][(i-1+gridsize)%gridsize][(j-1+gridsize)%gridsize].plant > 3) adj++;
            if (grid[ping][(i-1+gridsize)%gridsize][(j+0+gridsize)%gridsize].plant > 3) adj++;
            if (grid[ping][(i-1+gridsize)%gridsize][(j+1+gridsize)%gridsize].plant > 3) adj++;
            if (grid[ping][(i+0+gridsize)%gridsize][(j-1+gridsize)%gridsize].plant > 3) adj++;
            if (grid[ping][(i+0+gridsize)%gridsize][(j+1+gridsize)%gridsize].plant > 3) adj++;
            if (grid[ping][(i+1+gridsize)%gridsize][(j-1+gridsize)%gridsize].plant > 3) adj++;
            if (grid[ping][(i+1+gridsize)%gridsize][(j+0+gridsize)%gridsize].plant > 3) adj++;
            if (grid[ping][(i+1+gridsize)%gridsize][(j+1+gridsize)%gridsize].plant > 3) adj++;
            //if (adj == 2 || adj == 3) {
            if (adj < 2) {
              grow = true;
            }
            if (grow) {
              if (C->plant < 6) {
                C_1->plant = C->plant + 1;
              }
              else { //c->plant == 6
                // check for nearby trees
                int t_adj = 0;
                for (int ii = i-4; ii <= i+4; ++ii) {
                  for (int jj = j-4; jj <= j+4; ++jj) {
                    if (abs(ii-i) + abs(jj-j) < 6) {
                      if (grid[ping][(ii+gridsize)%gridsize][(jj+gridsize)%gridsize].plant > 5) t_adj++;
                    }
                  }
                }
                if (t_adj > 2) {
                  // seed random cell within distance 3
                  int s_i = (i-3 + rand()%7+gridsize)%gridsize;
                  int s_j = (j-3 + rand()%7+gridsize)%gridsize;
                  if (grid[ping][s_i][s_j].plant == 0)
                    grid[pong][s_i][s_j].plant = 1;
                }
                else {
                  // upgrade to tree
                  C_1->plant += 1;
                }
              }
            }
            else if (adj > 4) { // die if overcrowded
              C_1->plant = C->plant - 1;
            }
            else { // otherwise, no change
              C_1->plant = C->plant;
            }
          }

          // plant level 7-9 = tree, can only grow if < 3 other trees nearby
          else if (C->plant >= 7 && C->plant <= 9) {
            int adj = -1; // avoid counting self
            for (int ii = i-4; ii <= i+4; ++ii) {
              for (int jj = j-4; jj <= j+4; ++jj) {
                if (abs(ii-i) + abs(jj-j) < 6) {
                  if (grid[ping][(ii+gridsize)%gridsize][(jj+gridsize)%gridsize].plant > 6) adj++;
                }
              }
            }
            if (adj < 2) { // can grow if 1 or fewer other trees nearby
              C_1->plant = C->plant + 1;
            }
            else if (adj > 4) { // actively dies if 4 or more nearby neighbors
              C_1->plant = C->plant - 1;
            }
          }
        }
      }
    }
    for (int i=0; i < gridsize; ++i) {
      for (int j=0; j < gridsize; ++j) {
        if (grid[pong][i][j].plant > 9)
          grid[pong][i][j].plant = 9;
      }
    }
    ping = pong;
    pong = 1 - pong;
  }
};

#endif
