// Jordan Dick 2015
// functions for drawing various objects
//

//#ifndef STDIncludes
//#define STDIncludes
//#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>
//#ifdef __APPLE__
//#include <OpenGL/glu.h>
//#else
//#include <GL/glu.h>
//#endif
//#endif
//
//#ifndef Jobjects
//#define Jobjects
//
//
////Cos and Sin in degrees - stolen from ex8
//#define Sin(x) (sin((x) * 3.1415927/180))
//#define Cos(x) (cos((x) * 3.1415927/180))

#include "objects.h"

//Vertex Polar-Cartesian tranformation function
void VertexC(float th, float ph)
{
    //glTexCoord2f(th/360, (ph-90)/180);
    //glColor3f ((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
    //glColor3f (Sin(th)*Cos(ph), Sin(ph), Cos(th)*Cos(ph));
    glNormal3f(Sin(th)*Cos(ph), Sin(ph), Cos(th)*Cos(ph));
    glVertex3f(Sin(th)*Cos(ph), Sin(ph), Cos(th)*Cos(ph));
    //printf("%s\n",gluErrorString(glGetError()));
}

//without the coloration
void VertexS(float th, float ph)
{
    glNormal3f(Sin(th)*Cos(ph), Sin(ph), Cos(th)*Cos(ph));
    glVertex3f(Sin(th)*Cos(ph), Sin(ph), Cos(th)*Cos(ph));
}

//without the spherical normal
void Vertex(float th, float ph)
{
    glVertex3f(Sin(th)*Cos(ph), Sin(ph), Cos(th)*Cos(ph));
}

//Normal Polar-Cartesian transformation and summation function
void pNormal(float th1, float ph1, float th2, float ph2, float th3, float ph3)
{
    glNormal3f(Sin(th1)*Cos(ph1) + Sin(th2)*Cos(th2) + Sin(th3)*Cos(ph3),
                  Sin(ph1) + Sin(ph2) + Sin(ph3),
                  Cos(th1)*Cos(ph1) + Cos(th2)*Cos(ph2) + Cos(th3)*Cos(ph3));
}
//Sphere function nabbed from ex8 and slightly modified
void ball(float x, float y, float z,
             float s)
{
    int d = 10;
    glPushMatrix();

    glTranslatef(x, y, z);
    glScalef(s, s, s);

    //top fan
    glBegin(GL_TRIANGLE_FAN);
    VertexS(0,90);
    for(int th = 0; th <= 360; th += d)
    {
        VertexS(th, 90-d);
    }
    glEnd();

    //latitude rings
    for (int ph = 90-d; ph >= -90+2*d; ph -= d)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += d)
        {
            VertexS(th, ph);
            VertexS(th, ph-d);
        }
        glEnd();
    }
    
    //bottom fan
    glBegin(GL_TRIANGLE_FAN);
    VertexS(0,-90);
    for(int th = 360; th >= 0; th -= d)
    {
        VertexS(th, -90+d);
    }
    glEnd();

    glPopMatrix();
}

//Sphere
void sphere(float x, float y, float z,
                float r,
                float s)
{
    int d = 10;
    glPushMatrix();

    glTranslatef(x, y, z);
    glRotatef(r, 0,1,0);
    glScalef(s, s, s);

    //top fan
    glBegin(GL_TRIANGLE_FAN);
    VertexC(0,90);
    for(int th = 0; th <= 360; th += d)
    {
        VertexC(th, 90-d);
    }
    glEnd();

    //latitude rings
    for (int ph = 90-d; ph >= -90+2*d; ph -= d)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += d)
        {
            VertexC(th, ph);
            VertexC(th, ph-d);
        }
        glEnd();
    }
    
    //bottom fan
    glBegin(GL_TRIANGLE_FAN);
    VertexC(0,-90);
    for(int th = 360; th >= 0; th -= d)
    {
        VertexC(th, -90+d);
    }
    glEnd();

    glPopMatrix();
}

void torus(float x, float y, float z,
              float r,
              float s)
{
    int d = 10;
    int dd = 30;
    int m = 720/dd + 2;
    float cylinder[m][3];
    float normals[m][3];
    for (int i=0; i < m; i += 2)
    {
        cylinder[i][0] = s + r*Sin(i/2*dd);
        cylinder[i][1] = r*Cos(i/2*dd);
        cylinder[i][2] = 0;
        cylinder[i+1][0] = cylinder[i][0]*Cos(d);
        cylinder[i+1][1] = cylinder[i][1];
        cylinder[i+1][2] = cylinder[i][0]*Sin(d);

        normals[i][0] = Sin(i/2*dd);
        normals[i][1] = Cos(i/2*dd);
        normals[i][2] = 0;
        normals[i+1][0] = normals[i][0] * Cos(d);
        normals[i+1][1] = normals[i][1];
        normals[i+1][2] = normals[i][0] * Sin(d);
    }
    glPushMatrix();

    glTranslatef(x, y, z);

    for (int i=0; i < 360; i += d)
    {
        glBegin(GL_QUAD_STRIP);
        for (int j=0; j < m; ++j)
        {
            glNormal3f( normals[j][0],  normals[j][1],  normals[j][2]);
            glVertex3f(cylinder[j][0], cylinder[j][1], cylinder[j][2]);
        }
        glEnd();
        glRotatef(d, 0,-1,0);
    }
    
    glPopMatrix();
}

void cube(float x, float y, float z,
             float r,
             float s)
{
    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(r, 0,1,0);
    glScalef(s, s, s);
    //glScalef(s/sqrt(2), s/sqrt(2), s/sqrt(2)); //radius at corners

    //glColor3f(0.8,0.2,0.2);

    glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    glTexCoord2f(0.0, 0.2);
    glVertex3f(1,1,1);
    glTexCoord2f(0.25, 0.2);
    glVertex3f(1,1,-1);
    glTexCoord2f(0.5, 0.2);
    glVertex3f(-1,1,-1);
    glTexCoord2f(0.75, 0.2);
    glVertex3f(-1,1,1);

    glNormal3f(0,0,1);
    glTexCoord2f(0.0, 0.2);
    glVertex3f(1,1,1);
    glTexCoord2f(0.75, 0.2);
    glVertex3f(-1,1,1);
    glTexCoord2f(0.75, 0.8);
    glVertex3f(-1,-1,1);
    glTexCoord2f(0.0, 0.8);
    glVertex3f(1,-1,1);

    glNormal3f(1,0,0);
    glTexCoord2f(0.0, 0.2);
    glVertex3f(1,1,1);
    glTexCoord2f(0.0, 0.8);
    glVertex3f(1,-1,1);
    glTexCoord2f(0.25, 0.8);
    glVertex3f(1,-1,-1);
    glTexCoord2f(0.25, 0.2);
    glVertex3f(1,1,-1);

    glNormal3f(0,0,-1);
    glTexCoord2f(0.5, 0.8);
    glVertex3f(-1,-1,-1);
    glTexCoord2f(0.5, 0.2);
    glVertex3f(-1,1,-1);
    glTexCoord2f(0.25, 0.2);
    glVertex3f(1,1,-1);
    glTexCoord2f(0.25, 0.8);
    glVertex3f(1,-1,-1);

    glNormal3f(-1,0,0);
    glTexCoord2f(0.5, 0.8);
    glVertex3f(-1,-1,-1);
    glTexCoord2f(0.75, 0.8);
    glVertex3f(-1,-1,1);
    glTexCoord2f(0.75, 0.2);
    glVertex3f(-1,1,1);
    glTexCoord2f(0.5, 0.2);
    glVertex3f(-1,1,-1);

    glNormal3f(0,-1,0);
    glTexCoord2f(0.5, 0.8);
    glVertex3f(-1,-1,-1);
    glTexCoord2f(0.25, 0.8);
    glVertex3f(1,-1,-1);
    glTexCoord2f(0.0, 0.8);
    glVertex3f(1,-1,1);
    glTexCoord2f(0.75, 0.8);
    glVertex3f(-1,-1,1);
    glEnd();


    // WIREFRAME
    //glColor3f(1,1,1);

    //glBegin(GL_LINES);
    //glNormal3f(1,1,0);
    //glVertex3f(1,1,1);
    //glVertex3f(1,1,-1);

    //glNormal3f(0,1,-1);
    //glVertex3f(1,1,-1);
    //glVertex3f(-1,1,-1);

    //glNormal3f(-1,1,0);
    //glVertex3f(-1,1,-1);
    //glVertex3f(-1,1,1);

    //glNormal3f(0,1,1);
    //glVertex3f(-1,1,1);
    //glVertex3f(1,1,1);

    //glNormal3f(1,0,1);
    //glVertex3f(1,1,1);
    //glVertex3f(1,-1,1);

    //glNormal3f(1,-1,0);
    //glVertex3f(1,-1,1);
    //glVertex3f(1,-1,-1);

    //glNormal3f(0,-1,-1);
    //glVertex3f(1,-1,-1);
    //glVertex3f(-1,-1,-1);

    //glNormal3f(-1,-1,0);
    //glVertex3f(-1,-1,-1);
    //glVertex3f(-1,-1,1);

    //glNormal3f(0,-1,1);
    //glVertex3f(-1,-1,1);
    //glVertex3f(1,-1,1);

    //glNormal3f(-1,0,1);
    //glVertex3f(-1,1,1);
    //glVertex3f(-1,-1,1);

    //glNormal3f(-1,0,-1);
    //glVertex3f(-1,1,-1);
    //glVertex3f(-1,-1,-1);

    //glNormal3f(1,0,-1);
    //glVertex3f(1,1,-1);
    //glVertex3f(1,-1,-1);
    //glEnd();

    glPopMatrix();
}

void octahedron(float x, float y, float z,
                      float r,
                      float s)
{
    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(r, 0,1,0);
    //glRotatef(54.7356, 1,0,1);
    glScalef(s, s, s);

    //glColor3f(0.6,0.4,0.2);

    glBegin(GL_TRIANGLES);
    glNormal3f(1,1,1);
    glVertex3f(1,0,0);
    glVertex3f(0,1,0);
    glVertex3f(0,0,1);

    glNormal3f(-1,1,1);
    glVertex3f(0,0,1);
    glVertex3f(0,1,0);
    glVertex3f(-1,0,0);

    glNormal3f(-1,-1,1);
    glVertex3f(0,0,1);
    glVertex3f(-1,0,0);
    glVertex3f(0,-1,0);

    glNormal3f(-1,-1,-1);
    glVertex3f(0,-1,0);
    glVertex3f(-1,0,0);
    glVertex3f(0,0,-1);

    glNormal3f(1,-1,-1);
    glVertex3f(0,-1,0);
    glVertex3f(0,0,-1);
    glVertex3f(1,0,0);

    glNormal3f(1,1,-1);
    glVertex3f(1,0,0);
    glVertex3f(0,0,-1);
    glVertex3f(0,1,0);

    glNormal3f(1,-1,1);
    glVertex3f(1,0,0);
    glVertex3f(0,0,1);
    glVertex3f(0,-1,0);

    glNormal3f(-1,1,-1);
    glVertex3f(-1,0,0);
    glVertex3f(0,1,0);
    glVertex3f(0,0,-1);
    glEnd();


    //WIREFRAME
    //glColor3f(1,1,1);

    //glBegin(GL_LINE_STRIP);
    //glVertex3f(1,0,0);
    //glVertex3f(0,0,1);
    //glVertex3f(0,-1,0);
    //glVertex3f(1,0,0);
    //glVertex3f(0,1,0);
    //glVertex3f(-1,0,0);
    //glVertex3f(0,0,-1);
    //glVertex3f(0,1,0);
    //glVertex3f(0,0,1);
    //glVertex3f(-1,0,0);
    //glVertex3f(0,-1,0);
    //glVertex3f(0,0,-1);
    //glVertex3f(1,0,0);
    //glEnd();

    glPopMatrix();
}

void dodecahedron(float x, float y, float z,
                      float r,
                      float s)
{
    //points plotted using cartesian coordinates
    float phi = 0.5 + sqrt(5.0)/2; //the golden ratio, not the latitude angle
    float points[20][3];
    //cube corners
    points[0][0] = 1;         points[0][1] = 1;         points[0][2] = 1;         
    points[1][0] = -1;        points[1][1] = 1;         points[1][2] = 1;         
    points[2][0] = -1;        points[2][1] = 1;         points[2][2] = -1;         
    points[3][0] = 1;         points[3][1] = 1;         points[3][2] = -1;         
    points[4][0] = 1;         points[4][1] = -1;        points[4][2] = 1;         
    points[5][0] = -1;        points[5][1] = -1;        points[5][2] = 1;         
    points[6][0] = -1;        points[6][1] = -1;        points[6][2] = -1;         
    points[7][0] = 1;         points[7][1] = -1;        points[7][2] = -1;         
    //top and bottom
    points[8][0] = 1/phi;    points[8][1] = phi;      points[8][2] = 0;         
    points[9][0] = -1/phi;  points[9][1] = phi;      points[9][2] = 0;         
    points[10][0] = 1/phi;  points[10][1] = -phi;    points[10][2] = 0;        
    points[11][0] = -1/phi; points[11][1] = -phi;    points[11][2] = 0;        
    //front and back
    points[12][0] = 0;        points[12][1] = 1/phi;  points[12][2] = phi;        
    points[13][0] = 0;        points[13][1] = -1/phi; points[13][2] = phi;        
    points[14][0] = 0;        points[14][1] = 1/phi;  points[14][2] = -phi;        
    points[15][0] = 0;        points[15][1] = -1/phi; points[15][2] = -phi;        
    //left and right
    points[16][0] = phi;     points[16][1] = 0;        points[16][2] = 1/phi;        
    points[17][0] = phi;     points[17][1] = 0;        points[17][2] = -1/phi;        
    points[18][0] = -phi;    points[18][1] = 0;        points[18][2] = 1/phi;        
    points[19][0] = -phi;    points[19][1] = 0;        points[19][2] = -1/phi;        

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(r, 0,1,0);
    glRotatef(-58.2825, 0,0,1);
    glScalef(s/sqrt(3), s/sqrt(3), s/sqrt(3)); //vertices are sqrt(3) units from center

    //glColor3f(0.6,0.2,0.4);

    // pentagons from the top down

    //top front
    glBegin(GL_POLYGON);
    glNormal3f(points[0][0]+points[8][0]+points[9][0]+points[1][0]+points[12][0],
                  points[0][1]+points[8][1]+points[9][1]+points[1][1]+points[12][1],
                  points[0][2]+points[8][2]+points[9][2]+points[1][2]+points[12][2]);
    glVertex3f(points[0][0], points[0][1], points[0][2]);
    glVertex3f(points[8][0], points[8][1], points[8][2]);
    glVertex3f(points[9][0], points[9][1], points[9][2]);
    glVertex3f(points[1][0], points[1][1], points[1][2]);
    glVertex3f(points[12][0], points[12][1], points[12][2]);
    glEnd();

    //top back
    glBegin(GL_POLYGON);
    glNormal3f(points[2][0]+points[9][0]+points[8][0]+points[3][0]+points[14][0],
                  points[2][1]+points[9][1]+points[8][1]+points[3][1]+points[14][1],
                  points[2][2]+points[9][2]+points[8][2]+points[3][2]+points[14][2]);
    glVertex3f(points[2][0], points[2][1], points[2][2]);
    glVertex3f(points[9][0], points[9][1], points[9][2]);
    glVertex3f(points[8][0], points[8][1], points[8][2]);
    glVertex3f(points[3][0], points[3][1], points[3][2]);
    glVertex3f(points[14][0], points[14][1], points[14][2]);
    glEnd();

    //left top
    glBegin(GL_POLYGON);
    glNormal3f(points[2][0]+points[19][0]+points[18][0]+points[1][0]+points[9][0],
                  points[2][1]+points[19][1]+points[18][1]+points[1][1]+points[9][1],
                  points[2][2]+points[19][2]+points[18][2]+points[1][2]+points[9][2]);
    glVertex3f(points[2][0], points[2][1], points[2][2]);
    glVertex3f(points[19][0], points[19][1], points[19][2]);
    glVertex3f(points[18][0], points[18][1], points[18][2]);
    glVertex3f(points[1][0], points[1][1], points[1][2]);
    glVertex3f(points[9][0], points[9][1], points[9][2]);
    glEnd();

    //right top
    glBegin(GL_POLYGON);
    glNormal3f(points[0][0]+points[16][0]+points[17][0]+points[3][0]+points[8][0],
                  points[0][1]+points[16][1]+points[17][1]+points[3][1]+points[8][1],
                  points[0][2]+points[16][2]+points[17][2]+points[3][2]+points[8][2]);
    glVertex3f(points[0][0], points[0][1], points[0][2]);
    glVertex3f(points[16][0], points[16][1], points[16][2]);
    glVertex3f(points[17][0], points[17][1], points[17][2]);
    glVertex3f(points[3][0], points[3][1], points[3][2]);
    glVertex3f(points[8][0], points[8][1], points[8][2]);
    glEnd();

    //front right
    glBegin(GL_POLYGON);
    glNormal3f(points[0][0]+points[12][0]+points[13][0]+points[4][0]+points[16][0],
                  points[0][1]+points[12][1]+points[13][1]+points[4][1]+points[16][1],
                  points[0][2]+points[12][2]+points[13][2]+points[4][2]+points[16][2]);
    glVertex3f(points[0][0], points[0][1], points[0][2]);
    glVertex3f(points[12][0], points[12][1], points[12][2]);
    glVertex3f(points[13][0], points[13][1], points[13][2]);
    glVertex3f(points[4][0], points[4][1], points[4][2]);
    glVertex3f(points[16][0], points[16][1], points[16][2]);
    glEnd();

    //front left
    glBegin(GL_POLYGON);
    glNormal3f(points[5][0]+points[13][0]+points[12][0]+points[1][0]+points[18][0],
                  points[5][1]+points[13][1]+points[12][1]+points[1][1]+points[18][1],
                  points[5][2]+points[13][2]+points[12][2]+points[1][2]+points[18][2]);
    glVertex3f(points[5][0], points[5][1], points[5][2]);
    glVertex3f(points[13][0], points[13][1], points[13][2]);
    glVertex3f(points[12][0], points[12][1], points[12][2]);
    glVertex3f(points[1][0], points[1][1], points[1][2]);
    glVertex3f(points[18][0], points[18][1], points[18][2]);
    glEnd();

    //back left
    glBegin(GL_POLYGON);
    glNormal3f(points[2][0]+points[14][0]+points[15][0]+points[6][0]+points[19][0],
                  points[2][1]+points[14][1]+points[15][1]+points[6][1]+points[19][1],
                  points[2][2]+points[14][2]+points[15][2]+points[6][2]+points[19][2]);
    glVertex3f(points[2][0], points[2][1], points[2][2]);
    glVertex3f(points[14][0], points[14][1], points[14][2]);
    glVertex3f(points[15][0], points[15][1], points[15][2]);
    glVertex3f(points[6][0], points[6][1], points[6][2]);
    glVertex3f(points[19][0], points[19][1], points[19][2]);
    glEnd();

    //back right
    glBegin(GL_POLYGON);
    glNormal3f(points[7][0]+points[15][0]+points[14][0]+points[3][0]+points[17][0],
                  points[7][1]+points[15][1]+points[14][1]+points[3][1]+points[17][1],
                  points[7][2]+points[15][2]+points[14][2]+points[3][2]+points[17][2]);
    glVertex3f(points[7][0], points[7][1], points[7][2]);
    glVertex3f(points[15][0], points[15][1], points[15][2]);
    glVertex3f(points[14][0], points[14][1], points[14][2]);
    glVertex3f(points[3][0], points[3][1], points[3][2]);
    glVertex3f(points[17][0], points[17][1], points[17][2]);
    glEnd();

    //left bottom
    glBegin(GL_POLYGON);
    glNormal3f(points[5][0]+points[18][0]+points[19][0]+points[6][0]+points[11][0],
                  points[5][1]+points[18][1]+points[19][1]+points[6][1]+points[11][1],
                  points[5][2]+points[18][2]+points[19][2]+points[6][2]+points[11][2]);
    glVertex3f(points[5][0], points[5][1], points[5][2]);
    glVertex3f(points[18][0], points[18][1], points[18][2]);
    glVertex3f(points[19][0], points[19][1], points[19][2]);
    glVertex3f(points[6][0], points[6][1], points[6][2]);
    glVertex3f(points[11][0], points[11][1], points[11][2]);
    glEnd();

    //right bottom
    glBegin(GL_POLYGON);
    glNormal3f(points[7][0]+points[17][0]+points[16][0]+points[4][0]+points[10][0],
                  points[7][1]+points[17][1]+points[16][1]+points[4][1]+points[10][1],
                  points[7][2]+points[17][2]+points[16][2]+points[4][2]+points[10][2]);
    glVertex3f(points[7][0], points[7][1], points[7][2]);
    glVertex3f(points[17][0], points[17][1], points[17][2]);
    glVertex3f(points[16][0], points[16][1], points[16][2]);
    glVertex3f(points[4][0], points[4][1], points[4][2]);
    glVertex3f(points[10][0], points[10][1], points[10][2]);
    glEnd();

    //bottom front
    glBegin(GL_POLYGON);
    glNormal3f(points[5][0]+points[11][0]+points[10][0]+points[4][0]+points[13][0],
                  points[5][1]+points[11][1]+points[10][1]+points[4][1]+points[13][1],
                  points[5][2]+points[11][2]+points[10][2]+points[4][2]+points[13][2]);
    glVertex3f(points[5][0], points[5][1], points[5][2]);
    glVertex3f(points[11][0], points[11][1], points[11][2]);
    glVertex3f(points[10][0], points[10][1], points[10][2]);
    glVertex3f(points[4][0], points[4][1], points[4][2]);
    glVertex3f(points[13][0], points[13][1], points[13][2]);
    glEnd();

    //bottom back
    glBegin(GL_POLYGON);
    glNormal3f(points[7][0]+points[10][0]+points[11][0]+points[6][0]+points[15][0],
                  points[7][1]+points[10][1]+points[11][1]+points[6][1]+points[15][1],
                  points[7][2]+points[10][2]+points[11][2]+points[6][2]+points[15][2]);
    glVertex3f(points[7][0], points[7][1], points[7][2]);
    glVertex3f(points[10][0], points[10][1], points[10][2]);
    glVertex3f(points[11][0], points[11][1], points[11][2]);
    glVertex3f(points[6][0], points[6][1], points[6][2]);
    glVertex3f(points[15][0], points[15][1], points[15][2]);
    glEnd();


    //WIREFRAME
    //glColor3f(1,1,1);
    //glBegin(GL_LINE_STRIP);
    //glVertex3f(points[8][0], points[8][1], points[8][2]);
    //glVertex3f(points[0][0], points[0][1], points[0][2]);
    //glVertex3f(points[16][0], points[16][1], points[16][2]);
    //glVertex3f(points[17][0], points[17][1], points[17][2]);
    //glVertex3f(points[3][0], points[3][1], points[3][2]);
    //glVertex3f(points[14][0], points[14][1], points[14][2]);
    //glVertex3f(points[15][0], points[15][1], points[15][2]);
    //glVertex3f(points[7][0], points[7][1], points[7][2]);
    //glVertex3f(points[10][0], points[10][1], points[10][2]);
    //glVertex3f(points[4][0], points[4][1], points[4][2]);
    //glVertex3f(points[13][0], points[13][1], points[13][2]);
    //glVertex3f(points[12][0], points[12][1], points[12][2]);
    //glVertex3f(points[1][0], points[1][1], points[1][2]);
    //glVertex3f(points[18][0], points[18][1], points[18][2]);
    //glVertex3f(points[5][0], points[5][1], points[5][2]);
    //glVertex3f(points[11][0], points[11][1], points[11][2]);
    //glVertex3f(points[6][0], points[6][1], points[6][2]);
    //glVertex3f(points[19][0], points[19][1], points[19][2]);
    //glVertex3f(points[2][0], points[2][1], points[2][2]);
    //glVertex3f(points[9][0], points[9][1], points[9][2]);
    //glVertex3f(points[8][0], points[8][1], points[8][2]);
    //glEnd();

    //glBegin(GL_LINES);

    //glVertex3f(points[8][0], points[8][1], points[8][2]);
    //glVertex3f(points[3][0], points[3][1], points[3][2]);

    //glVertex3f(points[2][0], points[2][1], points[2][2]);
    //glVertex3f(points[14][0], points[14][1], points[14][2]);

    //glVertex3f(points[6][0], points[6][1], points[6][2]);
    //glVertex3f(points[15][0], points[15][1], points[15][2]);

    //glVertex3f(points[10][0], points[10][1], points[10][2]);
    //glVertex3f(points[11][0], points[11][1], points[11][2]);

    //glVertex3f(points[5][0], points[5][1], points[5][2]);
    //glVertex3f(points[13][0], points[13][1], points[13][2]);

    //glVertex3f(points[7][0], points[7][1], points[7][2]);
    //glVertex3f(points[17][0], points[17][1], points[17][2]);

    //glVertex3f(points[4][0], points[4][1], points[4][2]);
    //glVertex3f(points[16][0], points[16][1], points[16][2]);

    //glVertex3f(points[0][0], points[0][1], points[0][2]);
    //glVertex3f(points[12][0], points[12][1], points[12][2]);

    //glVertex3f(points[1][0], points[1][1], points[1][2]);
    //glVertex3f(points[9][0], points[9][1], points[9][2]);

    //glVertex3f(points[18][0], points[18][1], points[18][2]);
    //glVertex3f(points[19][0], points[19][1], points[19][2]);

    //glEnd();

    glPopMatrix();
}

void icosahedron(float x, float y, float z,
                      float r,
                      float s)
{
    //points plotted using polar coordinates
    float lat = atan(0.5)*180/3.1415927;
    float points[12][2];
    points[0][0] = 0;         points[0][1] = 90; 
    points[1][0] = 0;         points[1][1] = lat; 
    points[2][0] = 36;        points[2][1] = -lat; 
    points[3][0] = 72;        points[3][1] = lat; 
    points[4][0] = 108;      points[4][1] = -lat; 
    points[5][0] = 144;      points[5][1] = lat; 
    points[6][0] = 180;      points[6][1] = -lat; 
    points[7][0] = 216;      points[7][1] = lat; 
    points[8][0] = 252;      points[8][1] = -lat; 
    points[9][0] = 288;      points[9][1] = lat; 
    points[10][0]= 324;      points[10][1]= -lat; 
    points[11][0]= 0;         points[11][1]= -90; 

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(r, 0,1,0);
    glScalef(s, s, s);

    //glColor3f(0.2,0.4,0.6);

    glBegin(GL_TRIANGLES);
    pNormal(points[3][0], points[3][1],
              points[0][0], points[0][1],
              points[1][0], points[1][0]);
    Vertex(points[3][0], points[3][1]);
    Vertex(points[0][0], points[0][1]);
    Vertex(points[1][0], points[1][1]);

    pNormal(points[5][0], points[5][1],
              points[0][0], points[0][1],
              points[3][0], points[3][1]);
    Vertex(points[5][0], points[5][1]);
    Vertex(points[0][0], points[0][1]);
    Vertex(points[3][0], points[3][1]);

    pNormal(points[7][0], points[7][1],
              points[0][0], points[0][1],
              points[5][0], points[5][1]);
    Vertex(points[7][0], points[7][1]);
    Vertex(points[0][0], points[0][1]);
    Vertex(points[5][0], points[5][1]);

    pNormal(points[9][0], points[9][1],
              points[0][0], points[0][1],
              points[7][0], points[7][1]);
    Vertex(points[9][0], points[9][1]);
    Vertex(points[0][0], points[0][1]);
    Vertex(points[7][0], points[7][1]);

    pNormal(points[1][0], points[1][1],
              points[0][0], points[0][1],
              points[9][0], points[9][1]);
    Vertex(points[1][0], points[1][1]);
    Vertex(points[0][0], points[0][1]);
    Vertex(points[9][0], points[9][1]);

    pNormal(points[1][0], points[1][1],
              points[2][0], points[2][1],
              points[3][0], points[3][1]);
    Vertex(points[1][0], points[1][1]);
    Vertex(points[2][0], points[2][1]);
    Vertex(points[3][0], points[3][1]);

    pNormal(points[3][0], points[3][1],
              points[4][0], points[4][1],
              points[5][0], points[5][1]);
    Vertex(points[3][0], points[3][1]);
    Vertex(points[4][0], points[4][1]);
    Vertex(points[5][0], points[5][1]);

    pNormal(points[5][0], points[5][1],
              points[6][0], points[6][1],
              points[7][0], points[7][1]);
    Vertex(points[5][0], points[5][1]);
    Vertex(points[6][0], points[6][1]);
    Vertex(points[7][0], points[7][1]);

    pNormal(points[7][0], points[7][1],
              points[8][0], points[8][1],
              points[9][0], points[9][1]);
    Vertex(points[7][0], points[7][1]);
    Vertex(points[8][0], points[8][1]);
    Vertex(points[9][0], points[9][1]);

    pNormal(points[9][0], points[9][1],
              points[10][0], points[10][1],
              points[1][0], points[1][1]);
    Vertex(points[9][0], points[9][1]);
    Vertex(points[10][0], points[10][1]);
    Vertex(points[1][0], points[1][1]);

    pNormal(points[10][0], points[10][1],
              points[9][0], points[9][1],
              points[8][0], points[8][1]);
    Vertex(points[10][0], points[10][1]);
    Vertex(points[9][0], points[9][1]);
    Vertex(points[8][0], points[8][1]);

    pNormal(points[8][0], points[8][1],
              points[7][0], points[7][1],
              points[6][0], points[6][1]);
    Vertex(points[8][0], points[8][1]);
    Vertex(points[7][0], points[7][1]);
    Vertex(points[6][0], points[6][1]);

    pNormal(points[6][0], points[6][1],
              points[5][0], points[5][1],
              points[4][0], points[4][1]);
    Vertex(points[6][0], points[6][1]);
    Vertex(points[5][0], points[5][1]);
    Vertex(points[4][0], points[4][1]);

    pNormal(points[4][0], points[4][1],
              points[3][0], points[3][1],
              points[2][0], points[2][1]);
    Vertex(points[4][0], points[4][1]);
    Vertex(points[3][0], points[3][1]);
    Vertex(points[2][0], points[2][1]);

    pNormal(points[2][0], points[2][1],
              points[1][0], points[1][1],
              points[10][0], points[10][1]);
    Vertex(points[2][0], points[2][1]);
    Vertex(points[1][0], points[1][1]);
    Vertex(points[10][0], points[10][1]);

    pNormal(points[2][0], points[2][1],
              points[11][0], points[11][1],
              points[4][0], points[4][1]);
    Vertex(points[2][0], points[2][1]);
    Vertex(points[11][0], points[11][1]);
    Vertex(points[4][0], points[4][1]);

    pNormal(points[4][0], points[4][1],
              points[11][0], points[11][1],
              points[6][0], points[6][1]);
    Vertex(points[4][0], points[4][1]);
    Vertex(points[11][0], points[11][1]);
    Vertex(points[6][0], points[6][1]);

    pNormal(points[6][0], points[6][1],
              points[11][0], points[11][1],
              points[8][0], points[8][1]);
    Vertex(points[6][0], points[6][1]);
    Vertex(points[11][0], points[11][1]);
    Vertex(points[8][0], points[8][1]);

    pNormal(points[8][0], points[8][1],
              points[11][0], points[11][1],
              points[10][0], points[10][1]);
    Vertex(points[8][0], points[8][1]);
    Vertex(points[11][0], points[11][1]);
    Vertex(points[10][0], points[10][1]);

    pNormal(points[10][0], points[10][1],
              points[11][0], points[11][1],
              points[2][0], points[2][1]);
    Vertex(points[10][0], points[10][1]);
    Vertex(points[11][0], points[11][1]);
    Vertex(points[2][0], points[2][1]);
    glEnd();


    //WIREFRAME
    //glColor3f(1,1,1);

    ////top fan of edges
    //glBegin(GL_LINES);
    //for (i = 1; i <= 9; i += 2)
    //{
    //    Vertex(points[0][0], points[0][1]);
    //    Vertex(points[i][0], points[i][1]);
    //}
    //glEnd();
    ////top ring of edges
    //glBegin(GL_LINE_STRIP);
    //for (i = 1; i <= 9; i += 2)
    //    Vertex(points[i][0], points[i][1]);
    ////middle zigzag edges
    //for (i = 1; i <= 10; ++i)
    //    Vertex(points[i][0], points[i][1]);
    //Vertex(points[1][0], points[1][1]);
    //glEnd();
    ////bottom ring of edges
    //glBegin(GL_LINE_STRIP);
    //for (i = 2; i <= 10; i += 2)
    //    Vertex(points[i][0], points[i][1]);
    //Vertex(points[2][0], points[2][1]);
    //glEnd();
    ////bottom fan of edges
    //glBegin(GL_LINES);
    //for (i = 2; i <= 10; i += 2)
    //{
    //    Vertex(points[11][0], points[11][1]);
    //    Vertex(points[i][0], points[i][1]);
    //}
    //glEnd();

    glPopMatrix();
}
