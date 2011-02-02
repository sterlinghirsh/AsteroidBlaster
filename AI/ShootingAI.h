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
#include "Utility/Object3D.h"
#include "Items/Weapon.h"

class ShootingAI : public AI {
   /* There actually is no need to hold the entire ship.
    * It'd be entirely possible to only have pointers to the radar, weapons,
    * etc and no need to give the AI the entire ship.  It's just necessary
    * right now*/
   AsteroidShip* ship;
   Weapon* chosenWeapon;
   Point3D lastShotPos;
   Point3D aimingAt;
   
   /* TODO Add your respective functions here. */
   int aimAt(const Object3D& obj);
   bool aimedAt(const Object3D& obj);
   void chooseWeapon( int weapon );

   public:
   ShootingAI(AsteroidShip* owner);
   virtual int think(double dt);

};
