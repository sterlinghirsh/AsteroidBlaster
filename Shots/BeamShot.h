/**
 * BeamShot.h
 * Sterling Hirsh
 * Shot type 2
 */

#ifndef __BEAMSHOT_H__
#define __BEAMSHOT_H__

#include "Shot.h"

class BeamShot : public Shot {
   public:
      static double frequency; // Shots per sec
      bool hitYet;
      unsigned long lastHitFrame;

      BeamShot(Point3D& posIn, Vector3D dirIn, AsteroidShip* const ownerIn);
      virtual void draw();
      virtual void drawInMinimap();
      virtual bool detectCollision(Object3D* other, bool checkOther);
      virtual void update(double timeDiff);
      virtual void handleCollision(Object3D* other);
      virtual void debug();
};


#endif
