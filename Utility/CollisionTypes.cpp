#include "Utility/CollisionTypes.h"

bool CollisionBox::collidesWithSphere(CollisionSphere* sphere) {
   return sphere->collidesWithBox(this);
}

bool CollisionBox::collidesWithPoint(CollisionPoint* point) {
   return point->collidesWithBox(this);
}

bool CollisionSphere::collidesWithPoint(CollisionPoint* point) {
   return point->collidesWithSphere(this);
}

bool CollisionSphere::collidesWithCone(CollisionCone* cone) {
   return cone->collidesWithSphere(this);
}

bool CollisionSphere::collidesWithRay(CollisionRay* ray) {
   return ray->collidesWithSphere(this);
}
