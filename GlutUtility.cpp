/**
 * Sterling Hirsh
 * GlutUtility.c
 * Functions that aid with glut.
 */
#include <math.h>
#include <algorithm>
#include "GlutUtility.h"
#include <sys/time.h>

int GW, GH, mouseX, mouseY;
unsigned long curFrame;
bool drawPerspective = true;
GLUquadricObj *quadric;
RGB myimage[64][64];
RGB* pixel;

using namespace std;

int flopY(int yIn) {
   return (GH - 1) - yIn;
}

double p2wx(int xp) { 
   double d = ((GW - 1.0) / 2.0);
   double c = ((1 - GW) * GH) / (-2.0 * GW);
   return (xp - d) / c;
}

double p2wy(int yp) {
   double e = ((GH - 1) / 2.0);
   double f = e;
   return (flopY(yp) - f) / e;
}

double p2ix(int xp) {
   return p2wx(xp) * GH / (double) GW;
}

double p2iy(int yp) {
   return p2wy(yp);
}

int w2px(double xw) {
   double d = ((GW - 1.0) / 2.0);
   double c = ((1 - GW) * GH) / (-2.0 * GW);
   return round((xw * c) + d);
}

int w2py(double yw) {
   double e = ((GH - 1) / 2.0);
   double f = e;
   return flopY(round((yw * e) + f));
}

/* Maintain Aspect Ratio */
void reshape (GLsizei w, GLsizei h) {
   GW = w;
   GH = h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (drawPerspective) {
      gluPerspective(VERT_FOV, (double)w/h, 1, 200);
   } else {
      glOrtho((double)w/-h, (double) w/h, -1, 1, -0.5, 0.5); 
   }
   glMatrixMode(GL_MODELVIEW);
   glViewport(0, 0, w, h);
}

void usePerspective() {
   drawPerspective = true;
   reshape(GW, GH);
}

void useOrtho() {
   drawPerspective = false;
   reshape(GW, GH);
}

double distance3D(double x1, double y1, double z1, double x2,
 double y2, double z2) {
   return sqrt(
    (x2 - x1) * (x2 - x1) + 
    (y2 - y1) * (y2 - y1) + 
    (z2 - z1) * (z2 - z1));
}

double distance2D(double x1, double y1, double x2, double y2) {
   return sqrt(
    (x2 - x1) * (x2 - x1) +
    (y2 - y1) * (y2 - y1));
}

double clamp(double num, double minVal, double maxVal) {
   return max(minVal, min(num, maxVal));
}

double randdouble() {
   return (double)rand() / (double) RAND_MAX;
}

double doubleTime() {
   static struct timeval tp;
   gettimeofday(&tp, NULL);
   
   //printf("Sec: %ld, Usec: %d\n", tp.tv_sec, tp.tv_usec);
   return (double)(tp.tv_sec) + ((tp.tv_usec) / 1000000.0);
   /*
      // Doesn't work on OSX
   static struct timespec tp;
   if (!clock_gettime(CLOCK_REALTIME, &tp)) {
      perror("Clock: ");
      exit(EXIT_FAILURE);
   }
   return (double)(tp.tv_sec) + (tp.tv_nsec);
   */
   // Old way got funky.
   //return (double) clock() / (double) CLOCKS_PER_SEC;
}

void drawCylinder(double radius, double length) {
   glPushMatrix();
   gluDisk(quadric, 0, radius, 32, 1);
   gluCylinder(quadric, radius, radius, length, 10, 2);
   glTranslatef(0, 0, length);
   gluDisk(quadric, 0, radius, 32, 1);
   glPopMatrix();
}

//sets up a specific material
void materials(materialStruct materials) {
  glMaterialfv(GL_FRONT, GL_AMBIENT, materials.ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materials.diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materials.specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, materials.shininess);
}

//initialization for texture mapping
void init_tex() {
  //glBindTexture(GL_TEXTURE_2D, 0);
  //glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}


GLdouble objectM[4][4] = {
  {1.0, 0.0, 0.0, 0.0},
  {0.0, 1.0, 0.0, 0.0},
  {0.0, 0.0, 1.0, 0.0},
  {0.0, 0.0, 0.0, 1.0}
};

