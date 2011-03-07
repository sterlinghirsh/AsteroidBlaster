/**
 * Drawable. Any item that is to be drawn to the screen.
 * Taylor Arnicar
 * 3-6-11
 * CPE 476
 */

#include "Items/Drawable.h"

Drawable :: Drawable(double x, double y, double z, GLuint displayListIn) : position(new Point3D(x, y, z)) {
   // Initialize some other variables here.
}

// Deconstructor - not much to do at this high of a level
Drawable :: ~Drawable() {
   if (position != NULL)
      delete position;
}

// Init function
void Drawable :: init() {
   // Do cool display listing stuff here for the walls
}
