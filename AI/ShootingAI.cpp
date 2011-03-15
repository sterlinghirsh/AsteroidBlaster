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
   lastTarget = NULL;
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
void ShootingAI::chooseWeapon(Object3D** target) {
   if (*target != NULL) {

      if ((dynamic_cast<Shard*>(*target)) != NULL) {
         ship->selectWeapon(2);
      }
      else if ((*target)->radius < 2 && ship->getWeapon(1)->isCooledDown()
       && ship->getWeapon(1)->curAmmo != 0) {
         ship->selectWeapon(1);
      }
      else
         ship->selectWeapon(0);
   }
   chosenWeapon = ship->getCurrentWeapon();
}

Object3D* ShootingAI::chooseTarget() {
   // Make the AI choose from a list of Targetable objects instead of Drawable objects, which are inside the view frustum.
   std::list<Drawable*>* targets = gameState->targetableViewFrustumObjects;
   std::list<Drawable*>::iterator targets_iterator;
   Point3D* ship_position = ship->position;
   Point3D vec;
   double curWeight, maxWeight = -1;
   const double distWeight = 1000;
   const double radiusWeight = 1;
   const double proximityWeight = 3;
   
   // If the list of targets from the viewFrustum does not exist, give the AI no target.
   if(targets == NULL) {
      return NULL;
   }

   targets_iterator = targets->begin();
   Object3D* closest = NULL;

   for ( ; targets_iterator != targets->end(); ++targets_iterator) {
      //printf("starting null check.\n");
      if (*targets_iterator == NULL) {
      printf("targets_iterator was null!\n\n\n\n\n\n");
         continue;
      }
     // printf ("Made it past targets_iterator null check!\n");
     if (dynamic_cast<Particle*>(*targets_iterator) != NULL) {
        //printf("continuing b/c of a particle!!! \n");
        continue;
     }

      if (*targets_iterator == NULL || (dynamic_cast<Asteroid3D*>(*targets_iterator) == NULL && dynamic_cast<Shard*>(*targets_iterator) == NULL)) {
         continue;
      }
      if (dynamic_cast<Shard*>(*targets_iterator) != NULL &&
       (*targets_iterator)->position->distanceFrom(*ship_position) > 30)
         continue;

      curWeight = radiusWeight / ((*targets_iterator)->radius);
      vec = (*(*targets_iterator)->position - *ship->position);
      curWeight += distWeight / (vec * vec);
      vec = vec.normalize();
      curWeight += vec * lastShotPos * proximityWeight;

      if (dynamic_cast<Shard*>(*targets_iterator) != NULL)
         curWeight *= ship->getWeapon(2)->curAmmo != 0;
      
      if (maxWeight < 0 || curWeight > maxWeight) {
         maxWeight = curWeight;
         closest = dynamic_cast<Object3D*> (*targets_iterator);
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
   chooseWeapon(&target);

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
