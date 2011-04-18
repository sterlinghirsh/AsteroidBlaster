#include "Items/Ball.h"

Ball::Ball(const GameState* _gameState) :
   Object3D(_gameState) {
   }

Ball::Ball(double x, double y, double z, const GameState* _gameState) :
   Object3D(_gameState) {
      position->update(x, y, z);
   }

void Ball::draw() {
   GLUquadricObj *quadratic;
   quadratic=gluNewQuadric();
   gluQuadricNormals(quadratic, GLU_SMOOTH);
   glColor3f(1.0, 0.0, 0.0);
   glEnable(GL_LIGHTING);
   gluSphere(quadratic,0.1f,32,32);
   glEnable(GL_COLOR_MATERIAL);
}
