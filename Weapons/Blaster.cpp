/**
 * Blaster! The original Asteroids Weapon.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/14/2011
 * <3
 */

#include "Weapons/Blaster.h"
#include "Graphics/GlutUtility.h"
#include "Shots/ProjectileShot.h"

Blaster::Blaster(AsteroidShip* owner) 
 : Weapon(owner) {
   shotSpeed = 40; // Units per second
   coolDown = 0.08; // Seconds
   randomVariationAmount = 2; // Units
   name = "Blaster";
}

Blaster::~Blaster() {
   // Do nothing.
}

/**
 * Called every frame.
 * We'll probably keep track of something or other here.
 */
void Blaster::update(double timeDiff) {
   // Do nothing.
}

/**
 * This is what actually shoots. Finally!
 */
void Blaster::fire() {
   static Vector3D randomVariation;
   if (!isCooledDown())
      return;
   // Update timeLastFired with new current time.
   timeLastFired = doubleTime();
   // Copy the ship's position for the start point.
   Point3D start = *ship->position;
   // Copy the shot direction, set length to shotSpeed (since shotDirection is unit-length).
   Vector3D shotDirection(ship->shotDirection.scalarMultiply(shotSpeed));
   // Add a random variation to each of the shots.
   randomVariation.randomMagnitude();
   randomVariation.scalarMultiplyUpdate(randomVariationAmount);
   shotDirection.addUpdate(randomVariation);
   ship->shotDirection.movePoint(start);
   gameState->custodian.add(new ProjectileShot(start,
    shotDirection, ship));
}

/**
 * Basic debug function. Just in case!
 */
void Blaster::debug() {
   printf("Blaster!\n");
}

bool Blaster::aimAt(Object3D* target) {
   return false;
}
