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

/**
 * Deconstructor - not much to do at this high of a level
 */
Drawable :: ~Drawable() {
   if (position != NULL)
      delete position;
}

/**
 * Init function
 */
void Drawable :: init() {
   // Do cool display listing stuff here for the walls
}

/**
 * Tell the <un-square-rooted> distance between the other point and this 
 * Drawable object. This is not an accurate distance. This function should only
 * be used for fast comparisons of relative distances between objects,
 * not absolute distances.
 * This function is equivalent to (x2-x1)^2 + (y2-y1)^2 + (z2-z1)^2
 */
double Drawable :: unrootedDist(Point3D *other) {
   double xDiff = position->x - other->x;
   double yDiff = position->y - other->y;
   double zDiff = position->z - other->z;
   return (xDiff * xDiff) + (yDiff * yDiff) + (zDiff * zDiff);
}

/**
 * This is like the function above, but accepts another Drawable*, instead
 * of a Point3D*.
 */
double Drawable :: unrootedDist(Drawable *other) {
   return unrootedDist(other->position);
}
