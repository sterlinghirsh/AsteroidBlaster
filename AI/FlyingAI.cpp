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
#include <math.h>

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

#include <stdio.h>
#include <iostream>

using namespace std;
using std::vector;



// Tell c++ that gameState was declared elsewhere (in main.cpp)
extern GameState* gameState;

FlyingAI::FlyingAI(AsteroidShip* owner) {
   ship = owner;
   radar = new Radar(ship);
   enabled = false;
   
   
   goalPoint = new Point3D(0,0,0);
   waypoints = new std::vector<Point3D>(14);
   
   waypoints->push_back(Point3D(34,0,0));
   waypoints->push_back(Point3D(-34,0,0));
   waypoints->push_back(Point3D(0,34,0));
   waypoints->push_back(Point3D(0,-34,0));
   waypoints->push_back(Point3D(0,0,34));
   waypoints->push_back(Point3D(0,0,-34));
   
   waypoints->push_back(Point3D( 34, 34, 34));
   waypoints->push_back(Point3D( 34, 34,-34));
   waypoints->push_back(Point3D( 34,-34, 34));
   waypoints->push_back(Point3D( 34,-34,-34));
   waypoints->push_back(Point3D(-34, 34, 34));
   waypoints->push_back(Point3D(-34, 34,-34));
   waypoints->push_back(Point3D(-34,-34, 34));
   waypoints->push_back(Point3D(-34,-34,-34));
   

}

/**
 * Grabs a list of asteroids from the game radar.
 */
std::list<Asteroid3D*>* FlyingAI :: getAsteroidList() {
   std::list<Asteroid3D*>* asteroids = new std::list<Asteroid3D*>();
   //std::list<Object3D*>* targets = gameState->viewFrustumObjects;
   std::list<Drawable*>* targets = radar->getFullReading();
   std::list<Drawable*>::iterator targets_iterator;
   
   if (targets == NULL)
      return NULL;
   targets_iterator = targets->begin();
   
   for ( ; targets_iterator != targets->end(); targets_iterator++) {
      if (*targets_iterator == NULL || (dynamic_cast<Asteroid3D*>(*targets_iterator) == NULL)) {
         continue;
      }      
      asteroids->push_front( dynamic_cast<Asteroid3D*>(*targets_iterator) );
   }
   
   return asteroids;
}


Point3D* FlyingAI :: getClosestShard() {
   
   float shortestDist = 9999, tempDist = 9999;
   Point3D* closestShard = new Point3D(0,0,0);
   Point3D temp;
   std::list<Drawable*>* targets = radar->getFullReading();
   std::list<Drawable*>::iterator iter;
   
   iter = targets->begin();
   
   for ( ; iter != targets->end(); iter++) {
      if (*iter == NULL || (dynamic_cast<Shard*>(*iter) == NULL)) {
         continue;
      }  
      tempDist = (float) (
        ((*iter)->position->x - ship->position->x, 2) *
        ((*iter)->position->x - ship->position->x, 2) +
        ((*iter)->position->y - ship->position->y, 2) *
        ((*iter)->position->y - ship->position->y, 2) +
        ((*iter)->position->z - ship->position->z, 2) *
        ((*iter)->position->z - ship->position->z, 2));
         
      if(shortestDist > tempDist) {
         shortestDist = tempDist;
         closestShard->x = (*iter)->position->x;
         closestShard->y = (*iter)->position->y;
         closestShard->z = (*iter)->position->z;
      }
   }
   
   return closestShard;
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
   double dist = 0;
   
   std::list<Asteroid3D*>* asteroids = getAsteroidList();
   std::list<Asteroid3D*>::iterator i;

   Vector3D *sum = new Vector3D();   
   Vector3D v;
   
   if(asteroids->size() == 0) {
      v = Vector3D(*ship->position, *(getClosestShard()) );
      sum->addUpdate(v);
      sum->normalize();
      return sum; 
   }
   
   
   // Add goalPoint
   v = Vector3D(*ship->position, *goalPoint);
   v.normalize();
   v.scalarMultiplyUpdate(2.5);
   sum->addUpdate(v);
   
   // X walls   
   v = Vector3D(Point3D(0,0,0), Point3D(1,0,0));
   dist = gameState->getWallxMax() - ship->position->x;
   v.scalarMultiplyUpdate(-0.25 * fmin(Max_Dist, Max_Dist / dist));
   sum->addUpdate(v);
   
   v = Vector3D(Point3D(0,0,0), Point3D(-1,0,0));
   dist = ship->position->x - gameState->getWallxMin();
   v.scalarMultiplyUpdate(-0.25 * fmin(Max_Dist, Max_Dist / dist));
   sum->addUpdate(v);

   // Y walls
   v = Vector3D(Point3D(0,0,0), Point3D(0,1,0));
   dist = gameState->getWallyMax() - ship->position->y;
   v.scalarMultiplyUpdate(-0.25 * fmin(Max_Dist, Max_Dist / dist));
   sum->addUpdate(v);
   
   v = Vector3D(Point3D(0,0,0), Point3D(0,-1,0));
   dist = ship->position->y - gameState->getWallyMin();
   v.scalarMultiplyUpdate(-0.25 * fmin(Max_Dist, Max_Dist / dist));
   sum->addUpdate(v);   
   
   // Z Walls   
   v = Vector3D(Point3D(0,0,0), Point3D(0,0,1));
   dist = gameState->getWallzMax() - ship->position->z;
   v.scalarMultiplyUpdate(-0.25 * fmin(Max_Dist, Max_Dist / dist));
   sum->addUpdate(v);
   
   v = Vector3D(Point3D(0,0,0), Point3D(0,0,-1));
   dist = ship->position->z - gameState->getWallzMin();
   v.scalarMultiplyUpdate(-0.25 * fmin(Max_Dist, Max_Dist / dist));
   sum->addUpdate(v);
   
   // Add asteroid vectors
   for( i = asteroids->begin(); i != asteroids->end(); ++i) {
   
      Vector3D astrToShip (*(*i)->position, *ship->position); 
      dist = astrToShip.getLength();
      
      if(dist <= Max_Dist)
      {
         astrToShip.normalize();
         
         if (dist > 1) 
            astrToShip.scalarMultiplyUpdate(fmin(Max_Dist, Max_Dist / dist));
         else
            astrToShip.scalarMultiplyUpdate(Max_Dist);
            
         sum->addUpdate(astrToShip); 
      }
   }
   
   //sum->scalarMultiplyUpdate( 2.5 / Max_Dist );
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
   
   
   if(asteroids->size() == 0) {
      Vector3D v = Vector3D(*ship->position, *(getClosestShard()) );
      sum->addUpdate(v);
      sum->normalize();
      return sum; 
   }
   
   
   // Add a small weight towards the center
   sum->addUpdate( Vector3D(*ship->position, Point3D(0,0,0)) );
   sum->normalize();   
   sum->scalarMultiplyUpdate(0.00001);
   
   for( i = asteroids->begin(); i != asteroids->end(); ++i) {
   
      Vector3D astrToShip (*ship->position, *(*i)->position); 
      
      double dist = astrToShip.getLength();
      
      if(dist <= Max_Dist)
      {
         astrToShip.normalize();
         
         astrToShip.scalarMultiplyUpdate(-1 * dist / (Max_Dist) + 1);
      
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

void FlyingAI :: printMatrix( Matrix4 *m) 
{
   cout << "\t[ " << m->_11 << ", " <<  m->_12 << ", " <<  m->_13 << ", " <<  m->_14 << " ]" << endl;
   cout << "\t[ " << m->_21 << ", " <<  m->_22 << ", " <<  m->_23 << ", " <<  m->_24 << " ]" << endl;
   cout << "\t[ " << m->_31 << ", " <<  m->_32 << ", " <<  m->_33 << ", " <<  m->_34 << " ]" << endl;
   cout << "\t[ " << m->_41 << ", " <<  m->_42 << ", " <<  m->_43 << ", " <<  m->_44 << " ]" << endl;
}

void FlyingAI :: flyDirection ( Vector3D* desiredTraj ) {
   // Little bit of linear algebra solving for flight trajectory
   Matrix4 B ( (float) desiredTraj->xMag, 0, 0, 0,
               (float) desiredTraj->yMag, 0, 0, 0,
               (float) desiredTraj->zMag, 0, 0, 0,
                               0, 0, 0, 0 );
  
   Matrix4 A ( (float) ship->forward->xMag, (float) ship->forward->yMag, (float) ship->forward->zMag, 0,
               (float) ship->right->xMag  , (float) ship->right->yMag  , (float) ship->right->zMag  , 0,
               (float) ship->up->xMag     , (float) ship->up->yMag     , (float) ship->up->zMag     , 0,
                                 0,                   0,                   0, 0 );
   
   Point3D trajControl; 
   
   trajControl.x = A._11 * B._11 + A._12 * B._21 + A._13 * B._31; 
   trajControl.y = A._21 * B._11 + A._22 * B._21 + A._23 * B._31;
   trajControl.z = A._31 * B._11 + A._32 * B._21 + A._33 * B._31;
   
   ship->accelerateForward( (int) (trajControl.x * 2) );
   ship->accelerateRight( (int) (trajControl.y * 2) );
   ship->accelerateUp( (int) (trajControl.z * 2 ));
}

void FlyingAI :: faceDirection( Vector3D* desiredForward ) {
   const double PI = 3.14159265;
   double diff_front, diff_up, diff_right;
   
   // do some math to figure out the yaw and pitch difference from the current
   // orientation to the desired, then apply a simple P-Control.
   
   // Yaw Correction
   Vector3D proj = calcProjection( desiredForward, ship->forward, ship->right);
                   
   diff_front = acos(proj.dot(*ship->forward));
   diff_right = acos(proj.dot(*ship->right));
   
   if(diff_front > 0.0) {
      if(diff_right < 0.5 * PI) diff_front *= -1;
      ship->setYawSpeed(1.75 * diff_front / PI);
   } else {
      ship->setYawSpeed(0);
   }
   
   // Pitch Correction
   proj = calcProjection( desiredForward, ship->forward, ship->up);
                   
   diff_front = acos(proj.dot(*ship->forward));
   diff_up = acos(proj.dot(*ship->up));
   
   if(diff_front > 0.0) {
      if(diff_up < 0.5 * PI) diff_front *= -1;
      ship->setPitchSpeed(1.75 * diff_front / PI);
   } else {
      ship->setPitchSpeed(0);
   }
}

/**
 * Choose a correct flying mode, based on the current situation.
 */
FlyMode FlyingAI :: chooseMode(void) {
   return NEU; // What?
}

/**
 * Preform flying AI operations
 */
int FlyingAI :: think(double dt) {
   if (gameState == NULL)
      return 0;
   
   Vector3D* desiredForward;
   Vector3D* desiredTraj;
   if(!enabled) {
      return 0;
   }
   
   curMode = chooseMode();
   
   switch (curMode) {
   case 0:
      break;
   default:
      // Check waypoint
      Point3D diff = *(ship->position) - *goalPoint;
      double goalDist = sqrt ( diff * diff );
      if( goalDist < 3 ) {
         // pick a random point
         int index = (int)(rand() % waypoints->size());
         fprintf(stderr, "Index: %d \n", index); 
         *goalPoint = (*waypoints)[index];
      }
   
      // Get relevent vectors
      desiredForward = getPointDirection();
      desiredTraj = getFlyDirection();
   
   }

   // Seems like this should always be done. This is independent of the mode.
   // Issue flight commands
   faceDirection(desiredForward);   
   flyDirection(desiredTraj);
   
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
