/**
 * ElectricityShot.h
 * Incredibly attractive shots.
 * Vwahhhh
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day
 * <3
 */

#ifndef __ELECTRICITYSHOT_H__
#define __ELECTRICITYSHOT_H__

#include "Shots/Shot.h"
//#include "Items/Object3D.h"

class ElectricityShot : public Shot {
   public:
      ElectricityShot(Point3D& posIn, Vector3D dirIn, AsteroidShip* const ownerIn);
      virtual void draw();
      virtual void update(double timeDiff);
      int framesAlive;
      double angle;
      double length;
      double farRadius;
      bool detectCollision(Drawable* other, bool checkOther);
      void handleCollision(Drawable* other);
};

#endif
