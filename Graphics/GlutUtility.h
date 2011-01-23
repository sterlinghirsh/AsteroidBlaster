/**
 * Sterling Hirsh
 * GlutUtility.h
 * A collection of functions that make working with glut possible.
 * Note that global variables GW, GH, mouseX, and mouseY are defined here.
 */
#ifndef __GLUTUTILITY_H__
#define __GLUTUTILITY_H__

#include <stdlib.h>
#include <time.h>
#include <list>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#define GLUT_DISABLE_ATEXIT_HACK 
#include <GL/glut.h>
#endif

#define VERT_FOV 50.0

extern int GW, GH;
extern int flopY(int);
extern unsigned long curFrame;
extern bool drawPerspective;
extern GLUquadricObj *quadric;
double p2wx(int);
double p2wy(int);
double p2ix(int);
double p2iy(int);
void reshape(GLsizei w, GLsizei h);
double distance3D(double x1, double y1, double z1, double x2 = 0,
 double y2 = 0, double z2 = 0);
double distance2D(double x1, double y1, double x2 = 0, double y2 = 0);

// The following aren't defined in GlutUtility.cpp, but should be in the app.
void display(void);
void mouse(int button, int state, int x, int y);
void mouseMove(int x, int y);
void keyboard(unsigned char key, int x, int y);

double clamp(double, double, double);
double randdouble();

typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess[1];
} materialStruct;

void materials(materialStruct materials);
double doubleTime();
void useOrtho();
void usePerspective();
void drawCylinder(double radius, double length);
void init_tex();

extern materialStruct GreenShiny;
extern materialStruct BlueShiny;
extern materialStruct RedFlat;
extern materialStruct WhiteTransparent;
extern materialStruct Rock;

typedef struct RGB {
  GLubyte r;
  GLubyte g; 
  GLubyte b;
} RGB;

#endif
