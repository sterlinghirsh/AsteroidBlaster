/**
 * Sterling Hirsh
 * This is a bunch of collision types that various objects will override.
 * 4/25/11
 */

class CollisionType {
   // Axis-aligned bounding box.
   public:
      virtual Point3D& getMinPosition() const = 0;
      virtual Point3D& getMaxPosition() const = 0;
};

class CollisionBox : public CollisionType {};

class CollisionSphere : public CollisionType {
   public:
      virtual double getRadius() = 0;
      virtual Point3D& getCenter() const = 0;
};

class CollisionPoint : public CollisionSphere {
   public:
      virtual double getRadius() { return 0; }
}

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
}

// Default case
template<class A, class B>
bool detectCollision(A& a, B& b) {
   return false;
}

// Specialization for sphere sphere.
template<>
bool detectCollision<CollisionSphere, CollisionSphere>(CollisionSphere* obj1, CollisionSphere* obj2) {
   Vector3D tmp(obj1->getOrigin, Obj2->getOrigin);
   double radiusSum = obj1->getRadius() + obj2->getRadius();
   return tmp.getComparisonLength() <= radiusSum * radiusSum;
}

// Specialization for box box.
template<>
bool detectCollision<CollisionBox, CollisionBox>(CollisionBox* obj1, CollisionBox* obj2) {
   return !(obj1->getMinPosition().y > obj2->getMaxPosition().y ||
    obj1->getMaxPosition().y < obj2->getMinPosition().y ||
    obj1->getMinPosition().z > obj2->getMaxPosition().z ||
    obj1->getMaxPosition().z < obj2->getMinPosition().z);
}

// Specialization for sphere box.
template<>
bool detectCollision<CollisionSphere, CollisionBox>(CollisionSphere* sphere, CollisionBox*, box) {
   Point3D& center = sphere->getCenter();
   Point3D centerToClosestPoint(sphere->getCenter());
   // Set centerToClosestPoint to the closest point on the box.
   centerToClosestPoint.x = clamp(center.x, box.getMinPosition().x, box.getMaxPosition().x);
   centerToClosestPoint.y = clamp(center.y, box.getMinPosition().y, box.getMayPosition().y);
   centerToClosestPoint.z = clamp(center.z, box.getMinPosition().z, box.getMazPosition().z);

   // Get the vector from the closest point to the center.
   closestPoint.subtractUpdate(center);

   // Make sure its distance is less than radius.
   return closestPoint.getComparisonLength() < (sphere.getRadius() * sphere.getRadius());
}

template<>
bool detectCollision<CollisionBox, CollisionSphere>(CollisionBox* obj1, CollisionSphere*, obj2) {
   return detectCollision<CollisionSphere, CollisionBox>(obj2, obj1);
}
