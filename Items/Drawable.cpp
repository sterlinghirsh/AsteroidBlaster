/**
 * Drawable. Any item that is to be drawn to the screen.
 * Taylor Arnicar
 * 3-6-11
 * CPE 476
 */

#include "Items/Drawable.h"
#include "Items/BoundingWall.h"
#include "Utility/GameState.h"

/**
 * Default constructor. Shouldn't be used.
 */
Drawable::Drawable() {
   position = NULL;
   minPosition = NULL;
   maxPosition = NULL;
   gameState = NULL;
   std::cerr << "Default constructor called." << std::endl;
   collisionType = NULL;
}

/**
 * Base class main constructor.
 */
Drawable :: Drawable(const GameState* _gameState) :
 gameState(_gameState) {
 
   position = new Point3D(0, 0, 0);
   minPosition = new Point3D(0, 0, 0);
   maxPosition = new Point3D(0, 0, 0);

   if (gameState != NULL) {
      custodian = (Custodian*) &gameState->custodian;
   } else {
      std::cerr << "GameState is null!" << std::endl;
   }

   velocity = NULL;
   shouldRemove = false; // True when custodian should remove this.
   minXRank = maxXRank = 0;
   // Initialize some other variables here.
   cullRadius = -1.0;
   radius = 0.0;
   shouldBeCulled = true;
   minX = minY = minZ = maxX = maxY = maxZ = 0;
   shouldDrawInMinimap = false;
   collisionType = NULL;
}

/**
 * Deconstructor - not much to do at this high of a level
 */
Drawable :: ~Drawable() {
   if (position != NULL)
      delete position;
   if (velocity != NULL)
      delete velocity;
   if (minPosition != NULL)
      delete minPosition;
   if (maxPosition != NULL)
      delete maxPosition;
   if (collisionType != NULL)
      delete collisionType;
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
      Vector3D relevantExtent(0, 0, 0);
      
      switch (wall->wallID) {
         case WALL_TOP:
            relevantExtent.y = maxPosition->y;
            break;
         case WALL_BOTTOM:
            relevantExtent.y = minPosition->y;
            break;
         case WALL_RIGHT:
            relevantExtent.x = maxPosition->x;
            break;
         case WALL_LEFT:
            relevantExtent.x = minPosition->x;
            break;
         case WALL_FRONT:
            relevantExtent.z = maxPosition->z;
            break;
         case WALL_BACK:
            relevantExtent.z = minPosition->z;
            break;
      }

      Vector3D positionDifference(relevantExtent, wall->normal.scalarMultiply(-wall->wallSize));
   // Do nothing.
   if (velocity->dot(wall->normal) < 0 && positionDifference.dot(wall->normal) >= 0) {
      velocity->reflect(wall->normal);

      position->addUpdate(positionDifference);
   } else {
      wall->actuallyHit = false;
   }
}

/**
 * This debug function can be overridden with more specific debug info.
 * This one does not print velocity, since Object3D may have null velocity.
 */
void Drawable::debug() {
   printf("Drawable::debug(): (min/max)\n");
   minPosition->print();
   maxPosition->print();
}

void Drawable::nullPointers() {
   position = NULL;
   velocity = NULL;
   minPosition = NULL;
   maxPosition = NULL;
}

Point3D Drawable::getWallIntersectionPoint(BoundingWall* wall) {
   return *position;
}

// Overrides stuff from collisionbox.
Point3D& Drawable::getMinPosition() const {
   return *minPosition;
}

Point3D& Drawable::getMaxPosition() const {
   return *maxPosition;
}
