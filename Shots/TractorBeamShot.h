/**
 * TractorBeamShot.h
 * Incredibly attractive shots.
 * Vwahhhh
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day
 * <3
 */

#ifndef __TRACTORBEAMSHOT_H__
#define __TRACTORBEAMSHOT_H__

#include "Shots/Shot.h"

class TractorBeamShot : public Shot {
   public:
      TractorBeamShot(Point3D& posIn, Vector3D dirIn, AsteroidShip* const ownerIn);
      virtual void draw();
      virtual void update(double timeDiff);
      int framesAlive;
      double angle;
      double length;
      double farRadius;
      bool detectCollision(Object3D* other, bool checkOther);
      void handleCollision(Object3D* other);
};

#endif
