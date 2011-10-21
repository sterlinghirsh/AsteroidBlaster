/**
 * HomingMissileShot.h
 * This bomb seeks shit out.
 */

#ifndef __HOMINGMISSILESHOT_H__
#define __HOMINGMISSILESHOT_H__

#include "Shots/Shot.h"
#include "Shots/ExplosiveShot.h"

class HomingMissileShot : public ExplosiveShot {
   public:
      HomingMissileShot(Point3D &posIn, Vector3D dirIn, int _weaponIndex, AsteroidShip* const ownerIn, const GameState* _gamestate);
      virtual ~HomingMissileShot();
      virtual void draw();
      virtual void drawGlow();
      virtual void drawExplosion();
      virtual void update(double timeDiff);
      virtual double getDamage(Object3D* other);
      virtual void hitWall(BoundingWall* wall);
      // Called when the shot's isExploded becomes true.
      virtual void explode();

      // In seconds.
      double spin;
      double scaleSize;
      double secondScale;
      double addSize;
      double rx;
      double ry;
      double rz;
      double timeSinceExploded;
      double timeToExplode;
      double seekRadius;
      double slowDownPerSecond;
      double collisionRadius;
      bool hasDamaged;
      bool hasTarget;
      int targetID;
      int particleNum;
      Vector3D particleDirection;
      float particleColor;
      SoundChannel* soundHandle;
      
      virtual void save(ast::Entity* ent);
      virtual void load(const ast::Entity& ent);
};

#endif

