/**
 * Radar
 * Taylor Arnicar
 * 2-1-2011
 * Every ship will have a radar which it can query to get information about its surroundings.
 */

#include "Radar.h"

// Tell c++ that gameState was declared elsewhere (in main.cpp)
extern GameState* gameState;

Radar :: Radar(AsteroidShip* const ship) {
   owner = ship;
   curFrustum = new ViewFrustum();
}

/* Deconstructor shouldn't need to do anything
 */
Radar :: ~Radar() {
   delete owner;
   delete curFrustum;
}

/**
 * Provides a complete, unfiltered reading of the whole environment.
 * This reading does not include Particles of any type!
 */
std::list<Drawable*>* Radar :: getFullReading() {
   /* Get the custodian out of gameState, and get its vector of Objects.
    * Use this vector to construct a new copied vector (to be safe), and return that one.
    */
   std::vector<Drawable*>* all (gameState->custodian.getListOfObjects());
   
   // The new list, which will be returned
   std::list<Drawable*>* allNew = new std::list<Drawable*>();
   
   // As opposed to the checker, the one doing the checking. 
   Drawable* checkee;
   
   // Iterate over all the Object3D's the custodian gave us
   for (vectorIter = all->begin(); vectorIter != all->end(); ++vectorIter)
   {
      // Copy all of the ptrs into a list
      checkee = *vectorIter;

      // Add the Drawable object to the allNew list if it's not a particle
      if (dynamic_cast<Particle*>(checkee) == NULL) {
         allNew->push_back(checkee);
      }
   }
   
   return allNew;
}

/**
 * Provides a filtered reading of the environment based on what's near 
 * the owner AsteroidShip.
 * The distance which objects must be within to be returned by getNearbyReading()
 * This reading does not include Particles of any type!
 */
std::list<Drawable*>* Radar :: getNearbyReading(float radius) {
   /* Get the custodian out of gameState, and get its vector of Objects.
    * Use this vector to construct a new copied vector (to be safe), and return that one.
    */
   std::vector<Drawable*>* all (gameState->custodian.getListOfObjects());
   
   std::list<Drawable*>* nearList = new std::list<Drawable*>();
   
   // As opposed to the checker, the one doing the checking. 
   Drawable* checkee;
   
   // Iterate over all the Object3D's the custodian gave us
   for (vectorIter = all->begin(); vectorIter != all->end(); ++vectorIter)
   {
      checkee = *vectorIter;
      
      // If the object is not a particle & is within the required nearby distance to be drawn
      if(checkee != NULL && (dynamic_cast<Particle*>(checkee) == NULL) &&
            owner->position->distanceFrom(*(checkee->position)) <= radius) {
         nearList->push_back(checkee);
      }
   }
   
   return nearList;
}

/**
 * Provides a filtered reading of the environment based on what's within the camera's viewFrustum right now.
 * This ONLY returns targetable objects, NOT particles.
 * Ex: Asteroid3D's, Shards.
 */
std::list<Drawable*>* Radar :: getTargetableViewFrustumReading() {
   // Get the custodian out of gameState, and copy its vector of Objects into a new vector.
   std::vector<Drawable*> allObjects (*gameState->custodian.getListOfObjects());

   // Turn the vector of allObjects into a list. True says to cull it down to only the targetable ones (no particles)
   std::list<Drawable*>* culledList = curFrustum->cullToViewFrustum(&allObjects, true);
   
   return culledList;
}

/**
 * Provides a filtered reading of the environment based on what's within the camera's viewFrustum right now.
 */
std::list<Drawable*>* Radar :: getViewFrustumReading() {
   // Get the custodian out of gameState, and copy its vector of Objects into a new vector.
   std::vector<Drawable*> allObjects (*gameState->custodian.getListOfObjects());
// TODO: Reverse this: get the list of particles first, and append objects to that, rather than getting the list of objects, and appending particles to it.
   Drawable* curParticle;
   // Add all of the particles to allObjects.
   for (particleIter = Particle::particles.begin(); particleIter != Particle::particles.end(); ++particleIter) {
      curParticle = *particleIter;
      allObjects.push_back(curParticle);
   }

   // Sterling:
   // Consider each particle one at a time and cull it one at a time.
   // Consider each object one at a time and cull it one at a time.
   // Only add an object / particle to the list of things to draw if
   // it has not been culled.

   // Turn the vector of allObjects into a list, & cull it down to only Drawable objects.
   std::list<Drawable*>* culledList = curFrustum->cullToViewFrustum(&allObjects, false);
   
   /*
    * Make sure that we are receiving particles back from view frustum culling.
    *
   for(listIter = culledList->begin(); listIter != culledList->end(); ++listIter) {
      Particle* part; 
      if( (part = dynamic_cast<Particle*>(*listIter)) != NULL)
         printf("particle was pushed back successfully!\n");
   }
   */
    
   // Print out the frustum for debugging.
   //curFrustum->print();
   
   // Print debug info for the culled list.
   /*for (listIter = culledList->begin(); listIter != culledList->end(); ++listIter)
   {
     //(*listIter)->debug();
   }*/  
   return culledList;
}
