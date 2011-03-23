/**
 * Drawable. Any item that is to be drawn to the screen.
 * Taylor Arnicar
 * 3-6-11
 * CPE 476
 */

#include "Items/Drawable.h"
#include "Items/BoundingWall.h"

/**
 * We should get rid of the displayListIn thing, I think.
 * -Sterling
 */
Drawable :: Drawable(double x, double y, double z, GLuint displayListIn) : position(new Point3D(x, y, z)) {
   minPosition = new Point3D(0, 0, 0);
   maxPosition = new Point3D(0, 0, 0);

   velocity = NULL;
   shouldRemove = false; // True when custodian should remove this.
   minXRank = maxXRank = 0;
   // Initialize some other variables here.
   cullRadius = -1.0;
   radius = 0.0;
   shouldBeCulled = true;
   minX = minY = minZ = maxX = maxY = maxZ = 0;
   shouldDrawInMinimap = false;
}

/**
 * Deconstructor - not much to do at this high of a level
 */
Drawable :: ~Drawable() {
   if (position != NULL)
      delete position;
   //if (velocity != NULL)
      //delete velocity;
}

/**
 * Tell the <un-square-rooted> distance between the other point and this 
 * Drawable object. This is not an accurate distance. This function should only
 * be used for fast comparisons of relative distances between objects,
 * not absolute distances.
 * This function is equivalent to (x2-x1)^2 + (y2-y1)^2 + (z2-z1)^2
 */
double Drawable::unrootedDist(Point3D *other) {
   double xDiff = position->x - other->x;
   double yDiff = position->y - other->y;
   double zDiff = position->z - other->z;
   return (xDiff * xDiff) + (yDiff * yDiff) + (zDiff * zDiff);
}

/**
 * This is like the function above, but accepts another Drawable*, instead
 * of a Point3D*.
 */
double Drawable::unrootedDist(Drawable *other) {
   return unrootedDist(other->position);
}

/**
 * Returns the radius of the object which should be used for View Frustum Culling.
 */
double Drawable::getCullRadius() {
   if (cullRadius == -1.0) {
      return radius;
   } else {
      return cullRadius;
   }
}

/**
 * To be overwritten by all subclasses. This is an empty stub only for now.
 * Anything done here will happen to all Object3D's and Particles.
 */
void Drawable::update(double timeDifference) {
}

void Drawable::setCustodian(Custodian *cust) {
   custodian = cust;
}


/**
 * checkOther is set to true by default in Drawable.h.
 * If this doesn't detect a collision, then checkOther specifies if we should use other's 
 * collision detection function.
 * We use this as a last check so we can do other types of hit detection in the custodian.
 * If this is called first and another unoverrided detectCollision is called with
 * other->detectCollision(), this trusts the custodian's judgement.
 */
bool Drawable :: detectCollision(Drawable* other, bool checkOther) {
   if (this == other) {
      printf("detected collision with self!\n");
   }
   if (checkOther) {
      return other->detectCollision(this, false);
   }
   return !(other->maxPosition->y < minPosition->y || 
       other->minPosition->y > maxPosition->y ||
       other->maxPosition->z < minPosition->z ||
       other->minPosition->z > maxPosition->z);
}

void Drawable :: handleCollision(Drawable* other) {
   // Handle stuff in subclasses.
}

/**
 * Subclasses can extend this, but this draws a sphere on the minimap.
 */
void Drawable::drawInMinimap() {
   glPushMatrix();
      position->glTranslate();
      setMaterial(RedFlat);
      gluSphere(quadric, 5, 8, 8);
   glPopMatrix();
}

/*
 * drawGlow() is here to be overwritten by all subclasses.
 */
void Drawable::drawGlow() {}

/**
 * hitWall is here to be overriden. Called when the wall reflects it.
 */
void Drawable::hitWall(BoundingWall* wall) {
   // Do nothing.
}
