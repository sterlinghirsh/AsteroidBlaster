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
#include "math.h"


#warning FlyingAI.cpp remove debug prints
#include <iostream>

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

   const double Max_Dist = 40;  // Max dist an asteroid will affect the trajectory
   
   
   std::list<Asteroid3D*>* asteroids = getAsteroidList();
   std::list<Asteroid3D*>::iterator i;
   Vector3D *sum = new Vector3D();
   
   cout << asteroids->size() << " Asteroids found.  Vector Sum:";
   
   for( i = asteroids->begin(); i != asteroids->end(); ++i) {
      //                   || LOL POINTERS
      //                   \/ 
      Vector3D astrToShip (*(*i)->position, *ship->position); 
      
      double dist = astrToShip.getLength();
      
      if(dist <= 40.0)
      {
         astrToShip.normalize();
         
         // for now the dist vs weight is linear
         // in the future this will be exponential.
         astrToShip.scalarMultiplyUpdate(-1 * dist / Max_Dist + 1);
      
         sum->addUpdate(astrToShip);   
      }
   }
   
   sum->normalize();
   sum->scalarMultiplyUpdate(-1.0);
   cout << sum->xMag << "," << sum->yMag << "," << sum->zMag << "," << endl;
   
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
   Vector3D* desiredForward = getFlyDirection();
   
   // do some math to figure out the yaw and pitch difference from the current
   // orientation to the desired, then apply a simple P-Control.
   
   // Yaw Correction
   proj = calcProjection( desiredForward, ship->forward, ship->right);
                   
   diff_front = acos(proj.dot(*ship->forward));
   diff_right = acos(proj.dot(*ship->right));
   
   if(diff_front > 0.3) {
      if(diff_right < 0.5 * PI) diff_front *= -1;
      ship->setYawSpeed(diff_front / PI);
      //cout << diff_right << ", " << diff_front << endl;
   } else {
      ship->setYawSpeed(0);
   }
   
   // Pitch Correction
   proj = calcProjection( desiredForward, ship->forward, ship->up);
                   
   diff_front = acos(proj.dot(*ship->forward));
   diff_up = acos(proj.dot(*ship->right));
   
   if(diff_front > 0.3) {
      if(diff_up < 0.5 * PI) diff_front *= -1;
      ship->setPitchSpeed(diff_front / PI);
      //cout << diff_right << ", " << diff_front << endl;
   } else {
      ship->setPitchSpeed(0);
   }
   
   // clean up
   if(desiredForward != NULL)
      delete desiredForward;
 
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
