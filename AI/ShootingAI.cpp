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

/** Aims at an object.
 *
 * This function should be called multiple frames consecutively in order
 * to actually aim at the object. The gun barrel has a rotation time,
 * and this function takes care of moving the virtual barrel to where it
 * needs to be. This will take care of predicting where the target will
 * be based on its current velocity.
 *
 * @param obj the object to aim at
 * @return no idea, really. Just leaving this open in case I think of something
 */
int ShootingAI::aimAt(const Object3D& obj)
{

}

/** Checks if the virtual barrel is currently aimed at an object.
 *
 * Simple check to see if the barrel is currently aimed at a point.
 * Pretty simple, just calculate the vector between our position and
 * the distant point, normalize, and compare to the current direction
 * vector of the barrel.
 *
 * @param point the point to see if the barrel is aimed at.
 * @return true if it is aimed at the point, false if not
 */
bool ShootingAI::aimedAt(const Point3D& point)
{

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
   // choose target

   // If we aren't aiming at it, better start
   if (!aimedAt(target))
      aimAt(target);

   // Think has a return value just in case it needs to.
   return 0;
}
