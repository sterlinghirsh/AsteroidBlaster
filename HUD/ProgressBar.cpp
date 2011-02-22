/**
 * ProgressBar.cpp
 * Displays a progress bar on the hud, for things like cooldown or health.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/22/2011
 */

#include "HUD/ProgressBar.h"
#include "Utility/GlobalUtility.h"

ProgressBar::ProgressBar(float _height, float _width, float _x, float _y) :
 height(_height), width(_width), x(_x), y(_y), amount(0), outerBoxThickness(0.01) {
}

void ProgressBar::setAmount(float _amount) {
   amount = clamp(_amount, 0, 1);
}

void ProgressBar::draw() {
   float drawHeight = amount * height;
   const float backgroundZOffset = -0.01; // Some small negative number.
   glPushMatrix();
      glTranslatef(x, y, 0);
      setMaterial(BlackSolid);
      glBegin(GL_QUADS);
      // First draw black box.
      glColor3f(0, 0, 0);
      glVertex3f(-outerBoxThickness, height + outerBoxThickness, backgroundZOffset);
      glVertex3f(width + outerBoxThickness, height + outerBoxThickness, backgroundZOffset);
      glVertex3f(width + outerBoxThickness, -outerBoxThickness, backgroundZOffset);
      glVertex3f(-outerBoxThickness, -outerBoxThickness, backgroundZOffset);

      // Color goes from red to green.
      glColor3f(1 - amount, amount, 0);
      
      glVertex3f(0, drawHeight, 0); // Top left
      glVertex3f(width, drawHeight, 0); // top Right
      glVertex3f(width, 0, 0); // bottom Right
      glVertex3f(0, 0, 0); // Bottom left
      glEnd();
   glPopMatrix();
}
