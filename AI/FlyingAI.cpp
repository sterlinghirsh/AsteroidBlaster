/**
 * @file
 * The Flying AI class implementation.
 *
 * <pre>
 * The Flying AI is in charge of ship movement and navigation
 * The Flying AI shall:
 *    * Avoid collision with objects when possible
 *    * Use information about asteroids and minerals to determine waypoints
 *    * Keep the game interesting by simulating human inaccuracies.
 *
 * @author Justin Kuehn, Mike Smith, Taylor Arnicar, Sean Ghiocel
 */

#include "AI/FlyingAI.h"
#include "Items/Asteroid3D.h"

FlyingAI::FlyingAI(AsteroidShip* owner) {
   ship = owner;
   enabled = false;
}

int FlyingAI :: think(double dt) {

   if(!enabled) {
      return 0;
   }
   
   // Do stuff here
   
   return 0;
}

void FlyingAI :: enable() {
   enabled = true;
}

void FlyingAI :: disable() {
   enabled = false;
}

bool FlyingAI :: isEnabled() {
   return enabled;
}
