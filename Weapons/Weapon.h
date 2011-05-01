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
struct Point3D;

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
      virtual double getCoolDownAmount();

      /**
       * AIs will use this.
       * Given an object to aim at, this does what is possible to aim the weapon at the target.
       * Subclasses will override this. For example, Blaster will lead the target and Railgun won't.
       * Returns true when ready to fire.
       */
      virtual Point3D project(Object3D*)=0;
      virtual bool shouldFire(Point3D*, Point3D*)=0;

      /**
       * Called every frame.
       */
      virtual void update(double timeDiff)=0;

      // owner of this weapon
      AsteroidShip* ship;
      virtual std::string getName();
      virtual int buyPrice() { return level*weaponPrice; };
      virtual std::string weaponString();
      virtual std::string ammoString();
      void setIcon(std::string iconName);
      void drawIcon(bool isSelected);

      /*
       * The current amount of ammo available to use. -1 is infinite.
       */
      int curAmmo;
      /*
       * True if the ship owns it, false if it doesn't.
       */
      bool purchased;
      int weaponPrice;
      int level;
      int levelMax;
      int ammoPrice;
      int ammoAmount;

      bool fireBackwards;

   protected:
      WeaponType type; // Do we need this?
      double timeLastFired;
      double coolDown;
      double damage;
      std::string name;
      std::string icon;
};

#endif
