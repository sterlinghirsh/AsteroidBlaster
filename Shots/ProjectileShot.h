/**
 * ProjectileShot.h
 */

#ifndef __PROJECTILESHOT_H__
#define __PROJECTILESHOT_H__

#include "Shots/Shot.h"


class ProjectileShot : public Shot {
   public:
      ProjectileShot(Point3D &posIn, Vector3D dirIn, AsteroidShip* const ownerIn );
      static double frequency; // Shots per sec
      virtual void draw();
      virtual void update(double timeDiff);
      virtual void handleCollision(Object3D* other);

};

#endif
