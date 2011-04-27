/**
 * Sterling Hirsh
 * This is a bunch of collision types that various objects will override.
 * 4/25/11
 */

#ifndef __COLLISIONTYPES_H__
#define __COLLISIONTYPES_H__

#include "Utility/Point3D.h"
#include <math.h>

class CollisionSphere;
class CollisionBox;
class CollisionCone;
class CollisionPoint;
class CollisionRay;

class CollisionType {
   public:
      // Axis-aligned bounding box.
      Point3D minPosition;
      Point3D maxPosition;
      
      virtual bool collides(CollisionType* obj) {
         printf("Un-Overridden CollisionType::collides.\n");
         return false;
      }

      virtual bool collidesWithSphere(CollisionSphere* sphere) {
         printf("Un-Overridden CollisionType::collidesWithSphere.\n");
         return false;
      }

      virtual bool collidesWithBox(CollisionBox* box) {
         printf("Un-Overridden CollisionType::collidesWithBox.\n");
         return false;
      }

      virtual bool collidesWithPoint(CollisionPoint* point) {
         printf("Un-Overridden CollisionType::collidesWithBox.\n");
         return false;
      }

      virtual bool collidesWithRay(CollisionRay* Ray) {
         printf("Un-Overridden CollisionType::collidesWithRay.\n");
         return false;
      }

      virtual bool collidesWithCone(CollisionCone* cone) {
         printf("Un-Overridden CollisionType::collidesWithBox.\n");
         return false;
      }
};

class CollisionBox : public CollisionType {
   public:
      virtual bool collides(CollisionType* obj) {
         return obj->collidesWithBox(this);
      }

      virtual bool collidesWithSphere(CollisionSphere* sphere);
      virtual bool collidesWithPoint(CollisionPoint* point);

      virtual bool collidesWithBox(CollisionBox* box) {
         return !(minPosition.y > box->maxPosition.y ||
          maxPosition.y < box->minPosition.y ||
          minPosition.z > box->maxPosition.z ||
          maxPosition.z < box->minPosition.z);
      }

};

class CollisionSphere : public CollisionType {
   public:
      double radius;
      double radiusSquared;
      Point3D& center;

      CollisionSphere(double _radius, Point3D& _center) :
       radius(_radius), radiusSquared(_radius * _radius), center(_center) {

      }
      
      virtual bool collides(CollisionType* obj) {
         return obj->collidesWithSphere(this);
      }

      virtual bool collidesWithSphere(CollisionSphere* other) {
         Vector3D tmp(center, other->center);
         double radiusSum = radius + other->radius;
         return tmp.getComparisonLength() <= radiusSum * radiusSum;
      }

      virtual bool collidesWithBox(CollisionBox* box) {
         Point3D centerToClosestPoint(center);
         // Set centerToClosestPoint to the closest point on the box.
         centerToClosestPoint.x = clamp(center.x, box->minPosition.x, box->maxPosition.x);
         centerToClosestPoint.y = clamp(center.y, box->minPosition.y, box->maxPosition.y);
         centerToClosestPoint.z = clamp(center.z, box->minPosition.z, box->maxPosition.z);

         // Get the vector from the closest point to the center.
         centerToClosestPoint.subtractUpdate(center);

         // Make sure its distance is less than radius.
         return centerToClosestPoint.getComparisonLength() < radiusSquared;
      }

      virtual bool collidesWithPoint(CollisionPoint* point);
      virtual bool collidesWithCone(CollisionCone* cone);
};

class CollisionPoint : public CollisionType {
   public:
      Point3D position;
      virtual bool collides(CollisionType* obj) {
         return obj->collidesWithPoint(this);
      }

      virtual bool collidesWithSphere(CollisionSphere* sphere) {
         Vector3D tmp(sphere->center, position);
         return tmp.getComparisonLength() < sphere->radiusSquared;
      }

      virtual bool collidesWithBox(CollisionBox* box) {
         // x is checked by custodian.
         return position.y <= box->maxPosition.y &&
          position.y >= box->minPosition.y &&
          position.z <= box->maxPosition.z &&
          position.z >= box->minPosition.z;
      }
};

class CollisionRay : public CollisionType {
   public:
      virtual Point3D& getOrigin() const = 0;
      virtual Vector3D& getNormalizedDirection() const = 0;
      virtual bool collides(CollisionType* obj) {
         return obj->collidesWithRay(this);
      }
};
//class CollisionMesh : public CollisionType {};

class CollisionCone : public CollisionType {
   public:
      double angle;
      double sinReciprocal;
      double cosReciprocal;
      double sinAngle;
      double cosAngle;
      double sinSquared;
      double length;
      Vector3D direction;
      Point3D origin;

      CollisionCone(double _angle, double _length, Vector3D& _direction) :
       angle(_angle), length(_length), direction(_direction) {
         sinAngle = sin(angle);
         cosAngle = cos(angle);
         sinReciprocal = 1 / sinAngle;
         cosReciprocal = 1 / cosAngle;
         sinSquared = sinAngle * sinAngle;
      }

      virtual bool collides(CollisionType* obj) {
         return obj->collidesWithCone(this);
      }

      /**
       * http://www.geometrictools.com/LibMathematics/Intersection/Intersection.html
       */
      virtual bool collidesWithSphere(CollisionSphere* sphere) {
         Vector3D U = origin - 
          direction.scalarMultiply(sphere->radius * sinReciprocal);

         Vector3D D = sphere->center - U;

         double dsqr = D.dot(D);
         double e = direction.dot(D);

         if (e > 0 && e * e >= dsqr * cosReciprocal) {
            D = sphere->center - origin;
            dsqr = D.dot(D);
            e = - (D.dot(direction));
            
            if (e > 0 && e * e >= dsqr * sinSquared) {
               return dsqr <= sphere->radiusSquared;
            } else {
               return true;
            }
         }
         return false;
      }
};


#endif
