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
      Object3D* hitItem;

      BeamShot(Point3D& posIn, Vector3D dirIn, int _weaponIndex, AsteroidShip* const ownerIn, const GameState* _gameState);
      void drawBeam(bool drawDots);
      virtual void draw();
      virtual void drawGlow();
      virtual void drawInMinimap();
      virtual void update(double timeDiff);
      virtual void debug();
      virtual void hitWall(BoundingWall* wall);
      virtual Point3D getWallIntersectionPoint(BoundingWall* wall);
      double drawLength;
      double length;
};


#endif
