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
      unsigned long firstFrame;
      unsigned long lastHitFrame;
      Drawable* hitItem;

      BeamShot(Point3D& posIn, Vector3D dirIn, AsteroidShip* const ownerIn);
      void drawBeam(bool drawDots);
      virtual void draw();
      virtual void drawGlow();
      virtual void drawInMinimap();
      virtual bool detectCollision(Drawable* other, bool checkOther);
      virtual void update(double timeDiff);
      virtual void handleCollision(Drawable* other);
      virtual void debug();
   private:
      double drawLength;
      double length;
};


#endif
