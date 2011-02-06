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

const double ShootingAI::gunRotSpeed = 1.0;

ShootingAI::ShootingAI(AsteroidShip* owner)
{
   ship = owner;
   aimingAt = Point3D::Zero;
   lastShotPos = Point3D::Zero;
   //TODO possibly more stuff here.
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
int ShootingAI::aimAt(double dt, Object3D* target)
{
   Point3D wouldHit;
   double speed = chosenWeapon->getSpeed();
   double time = 0;
   double dist;
   double len;
   Vector3D dp;
   
   Point3D curTarget = *target->position;
   wouldHit = lastShotPos - aimingAt;
   if (wouldHit.magnitude() > target->radius) {
      wouldHit = wouldHit.normalize();
      wouldHit = wouldHit * (gunRotSpeed * dt);
      aimingAt = (wouldHit - *ship->position).normalize();
   }

   do {
      time = ship->position->distanceFrom(curTarget);
      dp = target->velocity->scalarMultiply(time);
      curTarget = curTarget + Point3D(dp.xMag, dp.yMag, dp.zMag);

      wouldHit = curTarget - *ship->position;
      wouldHit = wouldHit.normalize() * speed * time + *ship->position;
      dist = wouldHit.distanceFrom(curTarget);

   } while (dist > target->radius);
   lastShotPos = curTarget.normalize();

   return 0;
}

// kinda useless right now.
void ShootingAI::chooseWeapon( int weapon )
{
    ship->selectWeapon( weapon );
    //chosenWeapon = ship->getWeapon( weapon );
}

Object3D* ShootingAI::chooseTarget()
{

    // not real yet.
    std::vector<Object3D*> *targets = ship->getRadar()->getFullReading();

    std::vector<Object3D*>::iterator targets_iterator;

    Point3D* ship_position = ship->position;

    targets_iterator = targets->begin();
    Object3D* closest = *targets_iterator;
    double shortest_distance = closest->position->distanceFrom( *ship_position );
    for( ; targets_iterator != targets->end();
	 targets_iterator++ )
    {
	// get closest asteroid
	double distance = (*targets_iterator)->position->distanceFrom( *ship_position );
	if( distance < shortest_distance )
	{
	    shortest_distance = distance;
            closest = *targets_iterator;
        } 
    }

    return closest;
}

int ShootingAI::think(double dt)
{
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
   
   // choose weapon
   chooseWeapon( 1 );

   Object3D* target = chooseTarget();
   // choose target

   aimAt(dt, target);

   // Think has a return value just in case it needs to.
   return 0;
}
