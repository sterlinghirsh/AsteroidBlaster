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
      bool hitYet;
      unsigned long lastHitFrame;

      BeamShot(Point3D& posIn, Vector3D dirIn, AsteroidShip* const ownerIn);
      void drawBeam(bool drawDots);
      virtual void draw();
      virtual void drawGlow();
      virtual void drawInMinimap();
      virtual bool detectCollision(Object3D* other, bool checkOther);
      virtual void update(double timeDiff);
      virtual void handleCollision(Object3D* other);
      virtual void debug();
};


#endif
