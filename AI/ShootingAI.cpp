/**
 * @file
 * The Shooting AI class implementation.
 *
 * <pre>
 * The Shooting AI is in charge of controlling the ships weapons independently
 * of the flyer.
 * The Shooting AI shall:
 *    * Acquire targets quickly
 *    * Use information about targets to choose the best target.
 *    * Destroy targets
 *    * Help the ship stay alive
 *    * Keep the game interesting by simulating human inaccuracies.
 *
 * @author Mike Smith, Taylor Arnicar, Sean Ghiocel, Justin Kuehn
 */

#include "AI/ShootingAI.h"
#include "Items/Asteroid3D.h"
#include "Utility/Quaternion.h"
#include "Weapons/Weapon.h"
#include <math.h>

// Tell c++ that gameState was declared elsewhere (in main.cpp)
extern GameState* gameState;

// Radians/sec
const double ShootingAI::gunRotSpeed = M_PI / 2.0;

ShootingAI::ShootingAI(AsteroidShip* owner) {
   ship = owner;
   aimingAt = Point3D(0,1,0);
   lastShotPos = Point3D(0,0.9,0);
   // Start the AI as disabled
   enabled = false;
   //TODO possibly more stuff here.
   lastTarget == NULL;
}

double min(double a, double b) {
   return a < b ? a : b;
}

/** Aims at an object.
 *
 * This function should be called multiple frames consecutively in order
 * to actually aim at the object. The gun barrel has a rotation time,
 * and this function takes care of moving the virtual barrel to where it
 * needs to be. This will take care of predicting where the target will
 * be based on its current velocity.
 *
 * @param dt time difference since last frame, for gun turning.
 * @return no idea, really. Just leaving this open in case I think of something
 */
int ShootingAI::aimAt(double dt, Object3D* target) {
   Point3D wouldHit;
   double ang = 0;
  
   Point3D aim = lastShotPos;
   Point3D targetPos = chosenWeapon->project(target);
   Point3D targetDir = (targetPos - *ship->position).normalize(); 
   
   // This section of code does angle interpolation.
   // Find the angle between our vector and where we want to be.
   ang = acos(aim * targetDir);

   // Get our axis of rotation.
   wouldHit = (aim ^ targetDir).normalize();

   if (ang > (gunRotSpeed * dt)) {
      Quaternion q;
      q.FromAxis(wouldHit, gunRotSpeed * dt);

      aim = q * aim;
      // Normalize the vector.
      aim = aim.normalize();
   }
   else {
      aim = targetDir;
   }
   
   ship->updateShotDirection(aim);
   lastShotPos = aim;
   ship->fire(chosenWeapon->shouldFire(&targetPos, &aim));

   return 0;
}

// kinda useless right now.
void ShootingAI::chooseWeapon(Object3D* target) {
   if (target != NULL) {
      if ((dynamic_cast<Shard*>(target)) != NULL)
         ship->selectWeapon(2);
      else if (target->radius < 2 && ship->getWeapon(1)->isCooledDown()) {
         ship->selectWeapon(1);
      }
      else
         ship->selectWeapon(0);
   }
   chosenWeapon = ship->getCurrentWeapon();
}

Object3D* ShootingAI::chooseTarget() {
   std::list<Object3D*>* targets = gameState->viewFrustumObjects;
   std::list<Object3D*>::iterator targets_iterator;
   Point3D* ship_position = ship->position;
   double curDist, shortestDist = -1;
   
   targets_iterator = targets->begin();
   Object3D* closest = NULL;
   
   for ( ; targets_iterator != targets->end(); targets_iterator++) {
      // get closest asteroid
      //if (*targets_iterator == NULL || *targets_iterator == ship || NULL == (asteroid = dynamic_cast<Asteroid3D*>(*targets_iterator)) || asteroid->isShard  )
      // Trying this with shard subclass
      // IF THE AI BREAKS, REMOVE THIS
      if (*targets_iterator == NULL ||
       (dynamic_cast<Asteroid3D*>(*targets_iterator) == NULL &&
        dynamic_cast<Shard*>(*targets_iterator) == NULL)) {
      // END OF NEW CODE
         continue;
      }
      if (dynamic_cast<Shard*>(*targets_iterator) != NULL &&
       (*targets_iterator)->position->distanceFrom(*ship_position) > 30)
         continue;

      curDist = 1 / (((*targets_iterator)->radius) *
       pow((*targets_iterator)->position->distanceFrom( *ship_position ),2));
      
      if (shortestDist < 0 || curDist > shortestDist) {
         shortestDist = curDist;
         closest = *targets_iterator;
      } 
   }

   return closest;
}

int ShootingAI::think(double dt) {
   if(!enabled) {
      return 0;
   }
   
   // TODO this is where stuff goes! Should probably figure out the order of
   // the functions.

   /* Also, do we want to try and rechoose a target every think (ie frame)?
    * If not, it would make the shooter a little quicker, but may end up
    * shooting at bad targets (if the gun is pointed in the opposite direction,
    * it will take time to rotate it around to shoot the target, then
    * time to kill the asteroid, and throughout all this time a new asteroid
    * could have become a better target, or even a critical target)
    *
    * Just an idea, we will talk it over.
    */

   /* This order is tentative. We will probably change it. */
   
   // choose weapon railgun

   Object3D* target = chooseTarget();
   if (target == NULL && target != lastTarget) {
      printf("Could not find a target!\n");
      lastTarget = target;
   }
   else if (target != lastTarget) {
      printf("Switching targets..\n");
      if (!lastTarget) {
         printf("No previous target.\n");
      }
      printf("New target at: ");
      target->position->print();
      lastTarget = target;
   }
   chooseWeapon(target);

   chooseWeapon(target);

   if (target != NULL) {
      target->setTargeted(true);
      aimAt(dt, target);
   }
   else {
      ship->fire(false);
   }

   // Think has a return value just in case it needs to.
   return 0;
}

void ShootingAI :: enable() {
   enabled = true;
}

void ShootingAI :: disable() {
   enabled = false;
   ship->fire(false);
}

bool ShootingAI::isEnabled() {
   return enabled;
}
