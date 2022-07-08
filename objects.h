#include "stdGL.h"

#ifndef objects_h
#define objects_h

#define Sin(x) (sin(x * 3.1415927/180))
#define Cos(x) (cos(x * 3.1415927/180))


//Vertex Polar-Cartesian tranformation function
void VertexC(float th, float ph);

//without the coloration
void VertexS(float th, float ph);

//without the spherical normal
void Vertex(float th, float ph);

//Normal Polar-Cartesian transformation and summation function
void pNormal(float th1, float ph1, float th2, float ph2, float th3, float ph3);

//Sphere function nabbed from ex8 and slightly modified
void ball(float x, float y, float z,
             float s);

//Sphere function nabbed from ex8 and slightly modified
void sphere(float x, float y, float z,
                float r,
                float s);

void torus(float x, float y, float z,
              float r,
              float s);

void cube(float x, float y, float z,
             float r,
             float s);


void octahedron(float x, float y, float z,
                      float r,
                      float s);


void dodecahedron(float x, float y, float z,
                      float r,
                      float s);


void icosahedron(float x, float y, float z,
                      float r,
                      float s);

#endif
