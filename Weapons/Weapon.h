/**
 * Weapon Class for AsteroidBlaster
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * This produces shots.
 * Well, subclasses of this will.
 * Pew Pew!
 */

#ifndef __WEAPON_H__
#define __WEAPON_H__

#include <string>

/* Incomplete declaration for pointers. */
class Object3D;
class AsteroidShip;
class Custodian;
class GameState;
extern GameState* gameState;

enum WeaponType {
   PROJECTILE,
   BEAM,
   OTHER
};

class Weapon {
   public:
      Weapon(AsteroidShip* owner);
      virtual ~Weapon();
      virtual void fire()=0;
      virtual void debug()=0;
      virtual bool isCooledDown();

      /**
       * AIs will use this.
       * Given an object to aim at, this does what is possible to aim the weapon at the target.
       * Subclasses will override this. For example, Blaster will lead the target and Railgun won't.
       * Returns true when ready to fire.
       */
      virtual bool aimAt(Object3D*)=0;
      
      /**
       * Called every frame.
       */
      virtual void update(double timeDiff)=0;
      AsteroidShip* ship;
      virtual std::string getName();
      
      protected:
         WeaponType type; // Do we need this?
         double timeLastFired;
         double coolDown;
         std::string name;
};

#endif
