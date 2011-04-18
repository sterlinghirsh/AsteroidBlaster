#include "Items/Spring.h"

Spring::Spring(const GameState* _gameState) :
   Object3D(_gameState) {
      isAttached = false;
   }

void Spring::attach(Object3D* anchorIn, Camera* itemIn) {
   anchor = anchorIn;
   item = itemIn;
   isAttached = true;
}

void Spring::update(double ms) {
   if (isAttached) {
      Vector3D displace = *anchor->position;
      Vector3D dBack = anchor->forward->scalarMultiply(3.0);
      Vector3D dUp = anchor->up->scalarMultiply(2.0);
      displace.subtractUpdate(dBack);
      displace.addUpdate(dUp);
      Vector3D springVector = displace.subtract(*item->position);
      double length = springVector.getLength();
      if (length == 0.0) { return; }
      double normalLength = SPRING_LEN;
      double forceScalar = (length - normalLength) / normalLength;
      springVector.scalarMultiplyUpdate(1.0 / length);
      Vector3D forceVector = springVector.scalarMultiply(forceScalar);
      forceVector.scalarMultiplyUpdate(ms * FORCE_SCALE);
      item->position->addUpdate(forceVector);
   }
}

void Spring::draw() {
   glPushMatrix();
   glDisable(GL_LIGHTING);
   glDisable(GL_COLOR_MATERIAL);
   /*
   glBegin(GL_LINES);
   glColor3f(1.0, 1.0, 1.0);
   glVertex3f((float)anchor->position->x, (float)anchor->position->y, (float)anchor->position->z);
   glVertex3f((float)item->position->x, (float)item->position->y, (float)item->position->z);
   glEnd();
   */
   glTranslatef((float)item->position->x, (float)item->position->y, (float)item->position->z);
   //item->draw();
   glPopMatrix();
}
