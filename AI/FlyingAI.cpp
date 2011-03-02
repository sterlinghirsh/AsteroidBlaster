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
#include "Utility/Quaternion.h"
#include "Utility/Matrix4.h"
#include "math.h"

using namespace std;
using std::vector;

// Tell c++ that gameState was declared elsewhere (in main.cpp)
extern GameState* gameState;

FlyingAI::FlyingAI(AsteroidShip* owner) {
   ship = owner;
   radar = new Radar(ship);
   enabled = false;
}

/**
 * Grabs a list of asteroids from the game radar.
 */
std::list<Asteroid3D*>* FlyingAI :: getAsteroidList() {
   std::list<Asteroid3D*>* asteroids = new std::list<Asteroid3D*>();
   //std::list<Object3D*>* targets = gameState->viewFrustumObjects;
   std::list<Object3D*>* targets = radar->getFullReading();
   std::list<Object3D*>::iterator targets_iterator;
   
   targets_iterator = targets->begin();
   
   for ( ; targets_iterator != targets->end(); targets_iterator++) {
      if (*targets_iterator == NULL || (dynamic_cast<Asteroid3D*>(*targets_iterator) == NULL)) {
         continue;
      }      
      asteroids->push_front( dynamic_cast<Asteroid3D*>(*targets_iterator) );
   }
   
   return asteroids;
}



/** Creates a trajectory for the ship to fly.
 *
 * Create a vector from each asteroid to the ship, scaled by the distance, 
 * then sum them to create a single vector to represent trajectory.
 * 
 * @return Vector for ship to fly towards.
 */
Vector3D* FlyingAI :: getFlyDirection() {
   
   // Max dist an asteroid will affect the trajectory
   const double Max_Dist = 40;  
   
   std::list<Asteroid3D*>* asteroids = getAsteroidList();
   std::list<Asteroid3D*>::iterator i;
   
   Vector3D *sum = new Vector3D();   
   
   // Add a small weight towards the center
   sum->addUpdate(*ship->position);
   sum->normalize();
   
   for( i = asteroids->begin(); i != asteroids->end(); ++i) {
   
      Vector3D astrToShip (*(*i)->position, *ship->position); 
      
      double dist = astrToShip.getLength();
      
      if(dist <= Max_Dist)
      {
         astrToShip.normalize();
         
         if (dist > 1) 
            astrToShip.scalarMultiplyUpdate(fmax(Max_Dist, Max_Dist / dist));
         else
            astrToShip.scalarMultiplyUpdate(Max_Dist);
            
         sum->addUpdate(astrToShip); 
      }
   }
   
   sum->normalize();
   
   // clean up
   if(asteroids != NULL)
      delete asteroids;
   
   return sum;
}

/** Creates a trajectory for the ship to point towards
 *
 * Create a vector from each asteroid to the ship, scaled by the distance, 
 * then sum them to create a single vector to represent trajectory.
 * 
 * @return Vector for ship to point towards.
 */
Vector3D* FlyingAI :: getPointDirection() {

   const double Max_Dist = 50;  // Max dist an asteroid will affect the trajectory
   
   std::list<Asteroid3D*>* asteroids = getAsteroidList();
   std::list<Asteroid3D*>::iterator i;
   Vector3D *sum = new Vector3D();
   
   // Add a small weight towards the center
   sum->addUpdate( Vector3D(*ship->position, Point3D(0,0,0)) );
   sum->normalize();   
   sum->scalarMultiplyUpdate(0.005);
   
   for( i = asteroids->begin(); i != asteroids->end(); ++i) {
   
      Vector3D astrToShip (*ship->position, *(*i)->position); 
      
      double dist = astrToShip.getLength();
      
      if(dist <= Max_Dist)
      {
         astrToShip.normalize();
         
         astrToShip.scalarMultiplyUpdate(-1 * dist / Max_Dist + 1);
      
         sum->addUpdate(astrToShip);
      }
   }
   
   sum->normalize();
   
   // clean up
   if(asteroids != NULL)
      delete asteroids;
   
   return sum;
}

/**
 * Calculate the projected vector 'w' onto the plane spanned by 'u1' and 'u2'
 */
Vector3D FlyingAI :: calcProjection( Vector3D *w, Vector3D *u1, Vector3D *u2) {
   double scalar1 = w->dot(*u1) / u1->dot(*u1);
   double scalar2 = w->dot(*u2) / u2->dot(*u2);
   return u1->scalarMultiply(scalar1).add( u2->scalarMultiply(scalar2) );
}

/**
 * Preform flying AI operations
 */
int FlyingAI :: think(double dt) {
   
   if(!enabled) {
      return 0;
   }
   
   double diff_front, diff_up, diff_right;
   Vector3D proj;
   Vector3D* desiredForward = getPointDirection();
   
   // do some math to figure out the yaw and pitch difference from the current
   // orientation to the desired, then apply a simple P-Control.
   
   // Yaw Correction
   proj = calcProjection( desiredForward, ship->forward, ship->right);
                   
   diff_front = acos(proj.dot(*ship->forward));
   diff_right = acos(proj.dot(*ship->right));
   
   if(diff_front > 0.1) {
      if(diff_right < 0.5 * PI) diff_front *= -1;
      ship->setYawSpeed(diff_front / PI);
   } else {
      ship->setYawSpeed(0);
   }
   
   // Pitch Correction
   proj = calcProjection( desiredForward, ship->forward, ship->up);
                   
   diff_front = acos(proj.dot(*ship->forward));
   diff_up = acos(proj.dot(*ship->up));
   
   if(diff_front > 0.1) {
      if(diff_up < 0.5 * PI) diff_front *= -1;
      ship->setPitchSpeed(diff_front / PI);
   } else {
      ship->setPitchSpeed(0);
   }
   
   // Little bit of linear algebra solving for flight trajectory
   Vector3D* desiredTraj = getFlyDirection();
   
   Matrix4 B ( desiredTraj->xMag, 0, 0, 0,
               desiredTraj->yMag, 0, 0, 0,
               desiredTraj->zMag, 0, 0, 0,
                               0, 0, 0, 0 );
  
   Matrix4 A ( ship->forward->xMag, ship->forward->yMag, ship->forward->zMag, 0,
               ship->right->xMag  , ship->right->yMag  , ship->right->zMag  , 0,
               ship->up->xMag     , ship->up->yMag     , ship->up->zMag     , 0,
                                 0,                   0,                   0, 0 );
                                 
   Matrix4 Soln = A.toInverse() * B;
   
   Point3D trajControl = Soln.getRow(0);
   
   ship->accelerateForward( trajControl.x * 1.5 );
   ship->accelerateRight( trajControl.y * 1.5 );
   ship->accelerateUp( trajControl.z * 1.5 );
   
   // clean up
   if(desiredForward != NULL)
      delete desiredForward;

   if(desiredTraj != NULL)
      delete desiredTraj;
      
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
