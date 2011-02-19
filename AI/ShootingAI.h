/**
 * @file
 * A description of the Shooting AI class.
 *
 * <pre>
 * This file contains the class description of the Shooting AI class. This
 * (like every other AI class) inherits from the AI interface. The
 * implementation of the think function will involve the AI choosing a
 * target, continuing a gun rotation toward a chosen target, or finally
 * firing the chosen weapon once the gun has been aimed at the target. 
 * </pre>
 *
 * @author Mike Smith, Taylor Arnicar, Sean Ghiocel, Justin Kuehn
 */
#ifndef _SHOOTINGAI_H_
#define _SHOOTINGAI_H_

/* Here we should have the includes for the interface of weapons, radar data,
 * or anything else that has not yet been created. */
#include "Items/AsteroidShip.h"
#include "Utility/Point3D.h"
#include "AI/AI.h"
#include "Utility/Object3D.h"

// Incomplete class declaration so we can use pointers.
class Object3D;
class AsteroidShip;
class Weapon;

class ShootingAI : public AI {
   /* There actually is no need to hold the entire ship.
    * It'd be entirely possible to only have pointers to the radar, weapons,
    * etc and no need to give the AI the entire ship.  It's just necessary
    * right now*/
   AsteroidShip* ship;
   Weapon* chosenWeapon;
   Point3D lastShotPos;
   Object3D* lastTarget;
   Point3D aimingAt;
   bool enabled;
   
   /* TODO Add your respective functions here. */
   int aimAt(double dt, Object3D* target);
   void chooseWeapon(Object3D* target);
   Object3D* chooseTarget();

   public:
   ShootingAI(AsteroidShip* owner);
   virtual int think(double dt);
   // Enable this AI
   virtual void enable();
   // Disable this AI. It will stop shooting.
   virtual void disable();
   virtual bool isEnabled();

   static const double gunRotSpeed;
};

#endif
