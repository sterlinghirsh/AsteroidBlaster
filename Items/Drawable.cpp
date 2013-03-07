/**
 * Drawable. Any item that is to be drawn to the screen.
 * Taylor Arnicar
 * 3-6-11
 * CPE 476
 */

#include "Items/Drawable.h"
#include "Items/BoundingWall.h"
#include "Utility/GameState.h"
#include <iostream>

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
   shouldConstrain = true;
}

/**
 * Base class main constructor.
 */
Drawable :: Drawable(const GameState* _gameState) :
 gameState((GameState*) _gameState) {
 
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
   shouldConstrain = true;
}

/**
 * Deconstructor - get rid of everything shared that we can. 
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
 * Subclasses can extend this, but this draws a sphere on the minimap.
 */
void Drawable::drawInMinimap() {
   glPushMatrix();
      position->glTranslate();
      setMaterial(RedFlat);
      gluSphere(quadric, 5, 8, 8);
   glPopMatrix();
}

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
      
   if (positionDifference.dot(wall->normal) >= 0) {
      if (velocity->dot(wall->normal) < 0) {
         velocity->reflect(wall->normal);
      }
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

// serialize 
std::string Drawable::serialize() {
   // all of them except for stuff now owned by this class
   return "Drawable\n";
}


