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
      ElectricityShot(Point3D& posIn, Vector3D dirIn, int _weaponIndex, AsteroidShip* const ownerIn,
       const GameState* _gameState);
      virtual void draw();
      virtual void drawGlow();
      virtual void drawShot(bool isGlow);
      virtual void update(double timeDiff);
      // TODO: Looks like this still has some cone logic. Remove.
      double length;
      bool hitYet;
      virtual void save(ast::Entity* ent);
      virtual void load(const ast::Entity& ent);
      void setPosAndDir(Vector3D& newPos, Vector3D& newDir);
      void setStrength(double strength);
};

#endif
