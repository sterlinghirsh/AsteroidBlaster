/**
 * Sterling Hirsh
 * This is a bunch of collision types that various objects will override.
 * 4/25/11
 */

#ifndef __COLLISIONTYPES_H__
#define __COLLISIONTYPES_H__

#include "Utility/Point3D.h"
#include <math.h>
#include <boost/serialization/base_object.hpp>

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


   // Serialization
   public:
      template<class Archive> 
            void serialize(Archive & ar, const unsigned int version) {
         ar & minPosition;
         ar & maxPosition;
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


   // Serialization
   public:
      template<class Archive> 
            void serialize(Archive & ar, const unsigned int version) {
         ar & boost::serialization::base_object<CollisionType>(*this);
      }
};

class CollisionSphere : public CollisionType {
   public:
      double radius;
      double radiusSquared;
      Point3D& center;

      inline void updateRadius(double _radius) {
         radius = _radius;
         radiusSquared = _radius * _radius;
      }

      CollisionSphere(double _radius, Point3D& _center) :
       radius(_radius), radiusSquared(_radius * _radius), center(_center) {

      }
      
      inline virtual bool collides(CollisionType* obj) {
         return obj->collidesWithSphere(this);
      }

      inline virtual bool collidesWithSphere(CollisionSphere* other) {
         Vector3D tmp(center, other->center);
         double radiusSum = radius + other->radius;
         return tmp.getComparisonLength() <= radiusSum * radiusSum;
      }

      inline virtual bool collidesWithBox(CollisionBox* box) {
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

      virtual bool collidesWithRay(CollisionRay* ray);
      virtual bool collidesWithPoint(CollisionPoint* point);
      virtual bool collidesWithCone(CollisionCone* cone);

   // Serialization
   public:
      template<class Archive> 
            void serialize(Archive & ar, const unsigned int version) {
         ar & boost::serialization::base_object<CollisionType>(*this);
         ar & radius;
         ar & radiusSquared;
         ar & center;
      }
};

class CollisionPoint : public CollisionType {
   public:
      Point3D& position;

      CollisionPoint(Point3D& _position) : position(_position) {}

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


   // Serialization
   public:
      template<class Archive> 
            void serialize(Archive & ar, const unsigned int version) {
         ar & boost::serialization::base_object<CollisionType>(*this);
         ar & position;
      }
};

class CollisionRay : public CollisionType {
   public:
      Point3D& origin;
      Vector3D& direction;
      double length;

      CollisionRay(double _length, Vector3D& _direction, Point3D& _origin) :
       length(_length), direction(_direction), origin(_origin) {

      }

      virtual bool collides(CollisionType* obj) {
         return obj->collidesWithRay(this);
      }

      virtual inline bool collidesWithSphere(CollisionSphere* sphere) {
         // Use the quadratic formula with magic.
         Vector3D objectLocationToRayOrigin(sphere->center, origin);
         double quadratic_b = 2 * direction.dot(objectLocationToRayOrigin);
         double quadratic_c = objectLocationToRayOrigin.dot(objectLocationToRayOrigin) - (sphere->radiusSquared);

         double discriminant = (quadratic_b * quadratic_b) - (4 * quadratic_c);
         
         // We don't want to continue if we're going to deal with imaginary numbers.
         if (discriminant < 0) {
            return false;
         }

         double root1 = ((-quadratic_b) + sqrt(discriminant)) / 2;
         double root2 = ((-quadratic_b) - sqrt(discriminant)) / 2;

         // We don't want any points that are behind the origin or further than length.
         return !((root1 < 0 || root1 > length) && (root2 < 0 || root2 > length));
      }


   // Serialization
   public:
      template<class Archive> 
            void serialize(Archive & ar, const unsigned int version) {
         ar & boost::serialization::base_object<CollisionType>(*this);
         ar & origin;
         ar & direction;
         ar & length;
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

      CollisionCone(double _angle, double _length, Vector3D& _direction, Point3D& _origin) :
       angle(_angle), length(_length), direction(_direction), origin(_origin) {
         sinAngle = sin(angle);
         cosAngle = cos(angle);
         sinReciprocal = 1 / sinAngle;
         cosReciprocal = 1 / cosAngle;
         sinSquared = sinAngle * sinAngle;
      }

      inline virtual bool collides(CollisionType* obj) {
         return obj->collidesWithCone(this);
      }

      /**
       * http://www.geometrictools.com/LibMathematics/Intersection/Intersection.html
       */
      virtual bool collidesWithSphere(CollisionSphere* sphere) {
         Vector3D diff(origin, sphere->center);
         double lenSqr = diff.getComparisonLength();

         // Too damn far away.
         if (lenSqr > length * length)
            return false;

         // Is the cone vertex inside the sphere?
         if (lenSqr < sphere->radiusSquared)
            return true;

         double dotProduct = diff.dot(direction);
         double dotSqr = dotProduct * dotProduct;

         // Is sphere center inside the cone?
         if (dotSqr > diff.getComparisonLength() * cosAngle * cosAngle && 
          dotProduct > 0.0 && dotProduct < length)
            return true;

         // Sphere center is outside cone.  Problem now reduces to looking for
         // an intersection between circle and ray in the plane containing
         // cone vertex and spanned by cone axis and vector from vertex to
         // sphere center.

         // Ray is t*D+V (t >= 0) where |D| = 1 and dot(A,D) = cos(angle).
         // Also, D = e*A+f*(C-V).  Plugging ray equation into sphere equation
         // yields R^2 = |t*D+V-C|^2, so the quadratic for intersections is
         // t^2 - 2*dot(D,C-V)*t + |C-V|^2 - R^2 = 0.  An intersection occurs
         // if and only if the discriminant is nonnegative.  This test becomes
         //
         //     dot(D,C-V)^2 >= dot(C-V,C-V) - R^2
         //
         // Note that if the right-hand side is nonpositive, then the inequality
         // is true (the sphere contains V).  I have already ruled this out in
         // the first block of code in this function.

         double uLen = sqrt(fabs(lenSqr - dotSqr));
         double test = cosAngle * dotProduct + sinAngle * uLen;
         double discr = test * test - lenSqr + sphere->radiusSquared;

         return discr >= 0.0 && test >= 0.0;
   


         /*
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
         */
      }


   // Serialization
   public:
      template<class Archive> 
            void serialize(Archive & ar, const unsigned int version) {
         ar & boost::serialization::base_object<CollisionType>(*this);
         ar & angle;
         ar & sinReciprocal;
         ar & cosReciprocal;
         ar & sinAngle;
         ar & cosAngle;
         ar & sinSquared;
         ar & length;
         ar & direction;
         ar & origin;
      }
};


#endif
