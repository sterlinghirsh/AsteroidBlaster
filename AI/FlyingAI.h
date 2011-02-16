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
#include "Items/AsteroidShip.h"
#include "Utility/Point3D.h"
#include "AI/AI.h"
#include "Utility/Object3D.h"



// Incomplete class declaration so we can use pointers.
class Object3D;
class AsteroidShip;

class FlyingAI : public AI {
   AsteroidShip* ship;
   bool enabled;
   
public:  
   FlyingAI(AsteroidShip* owner);
   virtual int think(double dt);
   virtual void enable();
   virtual void disable();
   virtual bool isEnabled();

};

#endif
