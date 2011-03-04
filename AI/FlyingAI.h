/**
 * @file
 * A description of the Flying AI class.
 *
 * <pre>
 * This file contains the class description of the Flying AI class. This
 * (like every other AI class) inherits from the AI interface. 
 * </pre>
 *
 * @author Justin Kuehn, Mike Smith, Taylor Arnicar, Sean Ghiocel
 */

#ifndef _FLYINGAI_H_
#define _FLYINGAI_H_

/* Here we should have the includes for the interface of weapons, radar data,
 * or anything else that has not yet been created. */
#include "AI/AI.h"
#include "Items/AsteroidShip.h"
#include "Items/Asteroid3D.h"
#include "Items/Object3D.h"
#include "Utility/Point3D.h"
#include "Utility/Radar.h"
#include "Utility/Vector3D.h"


// Incomplete class declaration so we can use pointers.
class Object3D;
class AsteroidShip;
class Radar;
class Vector;

class FlyingAI : public AI {
   static const double PI = 3.141592;
   
   AsteroidShip* ship;
   Radar* radar;
   bool enabled;
   
public:  
   FlyingAI(AsteroidShip* owner);
   virtual int think(double dt);
   virtual void enable();
   virtual void disable();
   virtual bool isEnabled();
   
   // Control Functions
   void faceDirection( Vector3D* desiredForward );
   void flyDirection( Vector3D* desiredTraj );
   
   // Helper Functions
   Vector3D* getFlyDirection();
   Vector3D* getPointDirection();
   std::list<Asteroid3D*>* getAsteroidList();
   Vector3D calcProjection( Vector3D *w, Vector3D *u1, Vector3D *u2);

};

#endif
