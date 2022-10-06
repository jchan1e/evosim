//#ifndef STDGL_H
//#define STDGL_H
//#include <cstdlib>
//#include <cstdio>
//#include <cmath>
//#ifdef __APPLE__
//#include <OpenGL/glu.h>
//#else
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glext.h>
////#include <GL/glew.h>
//#endif
//#endif
#include "stdGL.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "objects.h"
#include "world.h"
#include "agent.h"

using namespace std;

//GLOBAL VARIABLES//
//running or not
bool quit = false;
//bool gameOver =false;

//View Angles
double th = 0;
double ph = 45;
double dth = 0;
double dph = 0;
//Window Size
int w = 1920;
int h = 1080;

//eye position and orientation
double ex = 0;
double ey = 0;
double ez = 0;

double vx = 0;
double vy = 0;
double vz = 0;
double zoom = 48;
double dzoom = 0;

//lighting arrays
float Ambient[4];
float Diffuse[4];
float Specular[4];
float shininess[1];
float dullness[1];
float LightPos[4];
float ltheta = 0.0;

//Shaders
int shader = 0;
//int filter = 0;
//int blend  = 0;
//unsigned int img, frame;
int id;

//SDL Window/OpenGL Context
SDL_Window* window = NULL;
SDL_GLContext context;

//Timing
int ff = 32; //milliseconds per frame
int t_r = 0;
int t_dr = 0;
int t_oldr = 0;
int Pause = 1;
int frames = 0;
bool reverse = false;

//Simulation
int n_creatures;
int n_frames;
int n_boardsize;
//Creature* creatures = NULL;
World* W = NULL;
Cell*** sim = NULL;
int** directions = NULL;
float** energies = NULL;
int step = 0;

////////////////////
//functions that are called ahead of when they're defined
//because C
void reshape(int width, int height);
void keyboard(const Uint8* state);

// Connect and put score in database
//void dbInsert(string name);
//std::stringstream dbGetScores();
//////// SDL Init Function ////////

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        cerr << "SDL failed to initialize: " << SDL_GetError() << endl;
        success = false;
    }

    window = SDL_CreateWindow("EvoSim", 0,0 , w,h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        cerr << "SDL failed to create a window: " << SDL_GetError() << endl;
        success = false;
    }

    context = SDL_GL_CreateContext(window);
    if (context == NULL)
    {
        cerr << "SDL failed to create OpenGL context: " << SDL_GetError() << endl;
        success = false;
    }

    //Vsync
    if (SDL_GL_SetSwapInterval(1) < 0)
    {
        cerr << "SDL could not set Vsync: " << SDL_GetError() << endl;
//        success = false;
    }

    return success;
}

///////////////////////////////////


void display()
{
    const Uint8* state = SDL_GetKeyboardState(NULL);
    keyboard(state);

    //adjust the eye position
    th += dth;
    ph += dph;
    zoom = zoom<2.0?2.0:zoom+dzoom;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    reshape(w,h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //view angle
    ex = Sin(-th)*Cos(ph)*zoom;
    ey = Sin(ph)*zoom;
    ez = Cos(-th)*Cos(ph)*zoom;

    gluLookAt(ex,ey,ez , 0,0,0 , 0,Cos(ph),0);

    //////////Lighting//////////

    // Light position and rendered marker (unlit)

    // lighting colors/types
    Ambient[0] = 0.50; Ambient[1] = 0.52; Ambient[2] = 0.55; Ambient[3] = 1.0;
    Diffuse[0] = 0.65; Diffuse[1] = 0.65; Diffuse[2] = 0.60; Diffuse[3] = 1.0;
    Specular[0] = 0.9; Specular[1] = 0.9; Specular[2] = 0.7; Specular[3] = 1.0;
    shininess[0] = 512;
    dullness[0] = 16;

    // normally normalize normals
    glEnable(GL_NORMALIZE);

    // enable lighting
    glEnable(GL_LIGHTING);

    // set light model with viewer location for specular lights
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // enable the light and position it
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPos);


    ///////////////////////////

    float white[] = {1.0, 1.0, 1.0, 1.0};
    float nonwhite[] = {0.1, 0.1, 0.0, 1.0};
    float emission[] = {0.0, 0.0, 0.0, 1.0};

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    // Use PerPixel Lighting Shader
    glUseProgram(shader);

    // Draw All The Stuff
    //emission[0] = -0.05; emission[1] = -0.05; emission[2] = -0.05;
    //glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1, 1);

    glColor3f(0.1, 0.1, 0.1);
    glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    glVertex3f(-n_boardsize/2.0-1.5, -0.0, -n_boardsize/2.0-1.5);
    glVertex3f(-n_boardsize/2.0-1.5, -0.0,  n_boardsize/2.0+0.5);
    glVertex3f( n_boardsize/2.0+0.5, -0.0,  n_boardsize/2.0+0.5);
    glVertex3f( n_boardsize/2.0+0.5, -0.0, -n_boardsize/2.0-1.5);
    glEnd();

    glDisable(GL_POLYGON_OFFSET_FILL);

    for (int i=0; i < n_boardsize; i += 1)
    {
      for (int j=0; j < n_boardsize; j += 1)
      {
        // draw plants

        int plant = W->grid[0][i][j].plant;
        float x = i-n_boardsize/2;
        float y = 0.0;
        float z = j-n_boardsize/2;

        float r = 0.5;
        float g = 0.5;
        float b = 0.4;

        if (plant == 1)
        {
          r = 0.9;
          g = 0.8;
          b = 0.3;
        }
        else if (plant == 2)
        {
          r = 0.75;
          g = 0.8;
          b = 0.2;
        }
        else if (plant >= 3)
        {
          r = 0.6;
          g = 0.8;
          b = 0.1;
        }

        if (plant > 0) {
          glMaterialfv(GL_FRONT, GL_SPECULAR, nonwhite);
          glMaterialfv(GL_FRONT, GL_SHININESS, dullness);
        }
        else {
          glMaterialfv(GL_FRONT, GL_SPECULAR, white);
          glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
        }

        glColor3f(r,g,b);
        glNormal3f(0,1,0);
        glBegin(GL_QUADS);
        glVertex3f(x-0.5, 0.0, z-0.5);
        glVertex3f(x-0.5, 0.0, z+0.5);
        glVertex3f(x+0.5, 0.0, z+0.5);
        glVertex3f(x+0.5, 0.0, z-0.5);
        glEnd();

        if (plant > 3)
        {
          if (plant == 4)
          {
            glColor3f(0.4, 0.6, 0.05);
            cube(x, y, z, 0, 0.25);
          }
          else if (plant == 5)
          {
            glColor3f(0.35, 0.6, 0.05);
            cube(x, y, z, 0, 0.3125);
          }
          else if (plant == 6)
          {
            glColor3f(0.3, 0.6, 0.05);
            cube(x, y, z, 0, 0.375);
          }
          else if (plant == 7)
          {
            y = 0.625;
            glPushMatrix();
            glScalef(0.125, 0.5, 0.125);
            glColor3f(0.30, 0.25, 0.15);
            cube(x/0.125, y, z/0.125, 0, y);
            glPopMatrix();
            glColor3f(0.35, 0.5, 0.0);
            cube(x, y*1.25, z, 0, 0.25);
          }
          else if (plant == 8)
          {
            y = 0.8125;
            glPushMatrix();
            glScalef(0.15625, 0.5, 0.15625);
            glColor3f(0.30, 0.25, 0.15);
            cube(x/0.15625, y, z/0.15625, 0, y);
            glPopMatrix();
            glColor3f(0.30, 0.5, 0.0);
            cube(x, y*1.25, z, 0, 0.3125);
          }
          else if (plant == 9)
          {
            y = 1.0;
            glPushMatrix();
            glScalef(0.1875, 0.5, 0.1875);
            glColor3f(0.30, 0.25, 0.15);
            cube(x/0.1875, y, z/0.1875, 0, y);
            glPopMatrix();
            glColor3f(0.25, 0.5, 0.0);
            cube(x, y*1.25, z, 0, 0.4375);
          }
          //else
          //  cout << "this code should not run (plant >9): (" << i << ", " << j << ")\t" << plant << endl;
        }

        // draw creatures
        int creature_id = W->grid[0][i][j].creature_id;
        if (creature_id >= 0) {
          glMaterialfv(GL_FRONT, GL_SPECULAR, nonwhite);
          glMaterialfv(GL_FRONT, GL_SHININESS, dullness);
          // color head by energy
          float R_e = 0.75;
          float G_e = 0.75;
          float B_e = 0.75;
          // color body by genetics
          float R_g = 0.7;
          float G_g = 0.5;
          float B_g = 0.3;

          // body
          glColor3f(R_g, G_g, B_g);
          ball(x, y, z, 0.35);

          // head
          // octahedron or cone or something facing forward
          glColor3f(R_e, G_e, B_e);
          glPushMatrix();
          glTranslatef(x, y, z);
          glRotatef(-45*directions[step][creature_id], 0,1,0);
          octahedron(0.0, 0.0, 0.25, 0.0, 0.25);
          glPopMatrix();
        }

        //cout << x << "\t" << y << "\t" << z << endl;
        //ball(x,y,z, 0.5);
        //glColor3f(0.1,0.1,0.1);
      }
    }

    //cout << endl;

    glFlush();
    SDL_GL_SwapWindow(window);
}

void physics()
{

    if (!Pause)
    {
        // move the light
        //ltheta += M_PI/180;
        //ltheta = fmod(ltheta, 2*M_PI);
        //LightPos[0] = 4.5*sin(ltheta);
        //LightPos[2] = 4.5*cos(ltheta);

        // advance to the next frame
        if (!reverse)
        {
            if (step < n_frames-1)
                ++step;
        }
        else
        {
            if (step > 0)
                --step;
        }
        // update world state
        W->grid[0] = &sim[step][0];
    }
}

// this function stolen from 4229 class examples
char* ReadText(char* file)
{
    int n;
    char* buffer;
    FILE* f = fopen(file,"r");
    if (!f) {cerr << "Cannot open text file " << file << endl; quit = true;}
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    rewind(f);
    buffer = (char*) malloc(n+1);
    if (!buffer) {cerr << "Cannot allocate " << n+1 << " bytes for text file " << file << endl; quit = true;}
    int h = fread(buffer, n, 1, f);
    if (h != 1) {cerr << h << " Cannot read " << n << " bytes for text file " << file << endl; quit = true;}
    buffer[n] = 0;
    fclose(f);
    return buffer;
}

// this function stolen from 4229 class examples
int CreateShader(GLenum type, char* file)
{
    // Create the shader
    int shader = glCreateShader(type);
    // Load source code from file
    char* source = ReadText(file);
    glShaderSource(shader, 1, (const char**) &source, NULL);
    free(source);
    // Compile the shader
    fprintf(stderr, "Compile %s\n", file);
    glCompileShader(shader);
    // Return name (int)
    return shader;
}

// this function stolen (mostly) from 4229 class examples
int CreateShaderProg(char* VertFile, char* FragFile)
{
    // Create program
    int prog = glCreateProgram();
    // Create and compile vertex and fragment shaders
    int vert, frag;
    if (VertFile) vert = CreateShader(GL_VERTEX_SHADER,  VertFile);
    if (FragFile) frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
    // Attach vertex and fragment shaders
    if (VertFile) glAttachShader(prog,vert);
    if (FragFile) glAttachShader(prog,frag);
    // Link Program
    glLinkProgram(prog);
    // Return name (int)
    return prog;
}

void reshape(int width, int height)
{
    w = width;
    h = height;
    //new aspect ratio
    double w2h = (height > 0) ? (double)width/height : 1;
    //set viewport to the new window
    glViewport(0,0 , width,height);

    //switch to projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //adjust projection
    gluPerspective(60, w2h, 0.125, 1024);

    //switch back to model matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Per frame keyboard input here, per keypress input in main()
void keyboard(const Uint8* state)
{
    if (state[SDL_SCANCODE_ESCAPE] or state[SDL_SCANCODE_Q])
        quit = true;

    if (state[SDL_SCANCODE_LEFT])
        dth = 0.5;
    else if (state[SDL_SCANCODE_RIGHT])
        dth = -0.5;
    else
        dth = 0;
    if (state[SDL_SCANCODE_UP])
        dph = 0.5;
    else if (state[SDL_SCANCODE_DOWN])
        dph = -0.5;
    else
        dph = 0;
    if (state[SDL_SCANCODE_Z])
        dzoom = -0.20;
    else if (state[SDL_SCANCODE_X])
        dzoom = 0.20;
    else
        dzoom = 0;
}

// all user interaction goes here
void handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                    Pause = 1 - Pause;
                else if (event.key.keysym.scancode == SDL_SCANCODE_0)
                {
                    th = 0;
                    ph = 40;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_COMMA)
                {
                  ff *= 2;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_PERIOD)
                {
                  if (ff > 1)
                    ff /= 2;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_LEFTBRACKET)
                {
                  reverse = true;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHTBRACKET)
                {
                  reverse = false;
                }
                else
                {
                    const Uint8* state = SDL_GetKeyboardState(NULL);
                    keyboard(state);
                }
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    //cerr << event.window.data1 << " " << event.window.data2 << endl;
                    reshape(event.window.data1, event.window.data2);
                }
                break;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: vis <sim_filename>.sim\n";
        return 0;
    }
    //Initialize
    if (init() != true)
    {
        cerr << "Initialization Error, Shutting Down\n";
        return 1;
    }

    ifstream file (argv[1], ios::in|ios::binary);
    if (!file.is_open())
    {
        cerr << "Could not open file: " << argv[1] << endl;
    }
    file.seekg(0);
    file.read((char*)&n_creatures, sizeof(int));
    file.seekg(sizeof(int));
    file.read((char*)&n_boardsize, sizeof(int));
    file.seekg(2*sizeof(int));
    file.read((char*)&n_frames, sizeof(int));
    cout << "boardsize: " << n_boardsize << endl << "frames: " << n_frames << endl;

    W = new World(n_boardsize);
    //aminoList = new float[2*n_boardsize];
    //file.seekg(2*sizeof(int));
    //file.read((char*)aminoList, 2*n_aminos*sizeof(float));

    //creatures = new Creature[n_creatures];
    float tempgenes[13];
    for (int i=0; i < n_creatures; ++i) {
      W->add_creature(&tempgenes[0], 0);
    }
    file.seekg(3*sizeof(int)); // + 2*n_boardsize*sizeof(float));
    file.read((char*)&(W->creatures[0]), n_creatures*sizeof(Creature));

    // I don't like this hack, but I like storing and reading vectors I won't use even less
    vector<Connection> conns;
    for (int i=0; i < n_creatures; ++i) {
      //W->creatures[i].brain.conns = {};
      memcpy((char*)&(W->creatures[i].brain.conns),  &conns, sizeof(vector<Connection>));
    }
    //cout << W->creatures[0].brain.conns[0].strength << endl;

    unsigned int headersize = 3*sizeof(int) + n_creatures*sizeof(Creature);
    unsigned int framesize = n_boardsize*n_boardsize*sizeof(Cell) + n_creatures*sizeof(int) + n_creatures*sizeof(float);

    sim = new Cell**[n_frames];
    directions = new int*[n_frames];
    energies = new float*[n_frames];
    for (int i=0; i < n_frames; ++i) {
      sim[i] = new Cell* [n_boardsize];
      directions[i] = new int[n_creatures];
      energies[i] = new float[n_creatures];
      file.seekg(headersize + i*framesize);
      file.read((char*)directions[i], n_creatures*sizeof(int));
      file.seekg(headersize + i*framesize + n_creatures*sizeof(int));
      file.read((char*)energies[i], n_creatures*sizeof(float));
      for (int j=0; j < n_boardsize; ++j) {
        sim[i][j] = new Cell[n_boardsize];
        file.seekg(headersize + i*framesize + j*n_boardsize*sizeof(Cell) + n_creatures*(sizeof(int)+sizeof(float)));
        file.read((char*)sim[i][j], n_boardsize*sizeof(Cell));
        //if (i <= 3) {
        //for (int k=0; k < n_boardsize; ++k) {
        //  cout << sim[i][j][k].plant << " ";
        //}
        //cout << endl;
        //}
      }
      //if (i <= 3) cout << endl;
    }
    Cell **tmp_grid_0 = W->grid[0];
    Cell **tmp_grid_1 = W->grid[1];
    W->grid[0] = &sim[step][0];

    file.close();

    //compile shaders
    shader = CreateShaderProg((char*)"pixlight.vert",(char*)"pixlight.frag");
    //filter = CreateShaderProg(NULL, (char*)"src/gaussian.frag");
    //blend  = CreateShaderProg(NULL, (char*)"src/blender.frag");

    //create and configure textures for filters
    //glGenTextures(1,&img);
    //glBindTexture(GL_TEXTURE_2D,img);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    //glGenTextures(1,&frame);
    //glBindTexture(GL_TEXTURE_2D,frame);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    // SANITY CHECK
    //for (int i=0; i < n_frames; ++i) {
    //  for (int c=0; c < n_creatures; ++c) {
    //    if (directions[i][c] < 0 || directions[i][c] >= 8)
    //      cout << "directions[" << i << "][" << c << "]: " << directions[i][c] << endl;
    //    if (energies[i][c] < 0 || energies[i][c] > 100)
    //      cout << "energies[" << i << "][" << c << "]: " << energies[i][c] << endl;
    //  }
    //  for (int j=0; j < n_boardsize; ++j) {
    //    for (int k=0; k < n_boardsize; ++k) {
    //      Cell C = sim[i][j][k];
    //      if (C.plant < 0 || C.plant > 9)
    //        cout << "plant[" << i << "][" << j << "][" << k << "]: " << C.plant << endl;
    //      if (C.creature_id < -1 || C.creature_id > n_creatures)
    //        cout << "creature_id[" << i << "][" << j << "][" << k << "]: " << C.creature_id << endl;
    //    }
    //  }
    //}

    reshape(w,h);

    LightPos[0] = 50.0; LightPos[1] = 50.0; LightPos[2] = 50.0; LightPos[3] = 1.0;

    int startuptime = SDL_GetTicks();

    cout << "Controls:\nSpaceBar - Pause/Play\n[ & ] - reverse & forward\n< & > - slow down & speed up\narrow keys - rotate camera\nz & x - zoom\n0 - reset camera\nEsc & q - exit\n";

    ////////Main Loop////////
    while (!quit)
    {
        handleEvents();

        //// PHYSICS TIMING ////
        t_r = SDL_GetTicks();
        t_dr += t_r - t_oldr;
        while (t_dr >= ff)
        {
            physics();
            t_dr -= ff;
        }
        t_oldr = t_r;
        display();
        frames += 1;
    }

    cout << "Shutting Down\n";
    cout << "average framerate: " << 1000*(float)frames/(t_r - startuptime) << endl;

    W->grid[0] = tmp_grid_0;
    W->grid[1] = tmp_grid_1;
    for (int i=0; i < n_frames; ++i) {
      for (int j=0; j < n_boardsize; ++j) {
        delete[] sim[i][j];
      }
      delete[] sim[i];
      delete[] directions[i];
      delete[] energies[i];
    }
    delete[] sim;
    delete[] directions;
    delete[] energies;
    delete W;

    SDL_Quit();

    return 0;
}
