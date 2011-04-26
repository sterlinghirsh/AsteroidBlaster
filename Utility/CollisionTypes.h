/**
 * Sterling Hirsh
 * This is a bunch of collision types that various objects will override.
 * 4/25/11
 */

#ifndef __COLLISIONTYPES_H__
#define __COLLISIONTYPES_H__

#include "Utility/Point3D.h"

class CollisionType {
};

class CollisionBox : public CollisionType {
   // Axis-aligned bounding box.
   public:
      virtual Point3D& getMinPosition() const = 0;
      virtual Point3D& getMaxPosition() const = 0;
};

class CollisionSphere : public CollisionType {
   public:
      virtual double getRadius() = 0;
      virtual Point3D& getCenter() const = 0;
};

class CollisionPoint : public CollisionSphere {
   public:
      virtual double getRadius() { return 0; }
};

class CollisionRay : public CollisionType {
   public:
      virtual Point3D& getOrigin() const = 0;
      virtual Vector3D& getNormalizedDirection() const = 0;
};
//class CollisionMesh : public CollisionType {};

class CollisionCone : public CollisionType {
   public:
      virtual Point3D& getOrigin() const = 0;
      virtual double getAngle();
};

// Default case
template<class A, class B>
bool detectCollision(A* a, B* b) {
   printf("in default detectCollision\n");
   return false;
}

template<>
bool detectCollision<CollisionBox, CollisionBox>(CollisionBox* obj1, CollisionBox* obj2);

#endif
