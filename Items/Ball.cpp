#include "Items/Ball.h"

Ball::Ball() :
   Object3D(0.0, 0.0, 0.0, 0.0) {
   }

Ball::Ball(double x, double y, double z) :
   Object3D(x, y, z, 0.0) {
   }

void Ball::attach(Object3D* anchorIn) {
   anchor = anchorIn;
}

void Ball::draw() {
   GLUquadricObj *quadratic;
   quadratic=gluNewQuadric();
   gluQuadricNormals(quadratic, GLU_SMOOTH);
   glPushMatrix();
   //position->glTranslate();
   //glRotate();
   glDisable(GL_LIGHTING);
   glDisable(GL_COLOR_MATERIAL);
   glBegin(GL_LINES);
   glColor3f(1.0, 1.0, 1.0);
   //glVertex3f(0.0, 0.0, 0.0);
   glVertex3f((float)anchor->position->x, (float)anchor->position->y, (float)anchor->position->z);
   glVertex3f((float)position->x, (float)position->y, (float)position->z);
   glEnd();
   glTranslatef((float)position->x, (float)position->y, (float)position->z);
   glColor3f(1.0, 0.0, 0.0);
   glEnable(GL_LIGHTING);
   gluSphere(quadratic,0.1f,32,32);
   glEnable(GL_COLOR_MATERIAL);
   glPopMatrix();
}