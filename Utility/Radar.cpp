/**
 * Radar
 * Taylor Arnicar
 * 2-1-2011
 * Every ship will have a radar which it can query to get information about its surroundings.
 */

#include "Radar.h"
#include "Items/AsteroidShip.h"

// Tell c++ that gameState was declared elsewhere (in main.cpp)
extern GameState* gameState;
Point3D* Radar::cameraPosition;
Vector3D* Radar::viewVector;

Radar :: Radar(AsteroidShip* const ship) {
   owner = ship;
   curFrustum = new ViewFrustum();
   cameraPosition = NULL;
   viewVector = NULL;
}

/* Deconstructor shouldn't need to do anything
 */
Radar :: ~Radar() {
   delete owner;
   delete curFrustum;
   if (cameraPosition != NULL)
      delete cameraPosition;
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

   if (gameState == NULL || all == NULL)
      return NULL;
   
   // The new list, which will be returned
   std::list<Drawable*>* allNew = new std::list<Drawable*>();
   
   // As opposed to the checker, the one doing the checking. 
   Drawable* checkee;
   
   // Iterate over all the Object3D's the custodian gave us
   for (vectorIter = all->begin(); vectorIter != all->end(); ++vectorIter) {
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

   // Set the camera temporarily.

   glPushMatrix();
      owner->setCameraDirectly();
      // Turn the vector of allObjects into a list. True says to cull it down to only the targetable ones (no particles)
      std::list<Drawable*>* culledList = curFrustum->cullToViewFrustum(&allObjects, true);
   glPopMatrix();
   
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

   for (spriteIter = Sprite::sprites.begin(); spriteIter != Sprite::sprites.end(); ++spriteIter) {
      allObjects.push_back(*spriteIter);
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
    
   /* We need to set the camera position which will be used to compare the distanes
    * of the two drawable objects.
    */
   // Make a copy of the ship's position
   Point3D movedCameraPosition(*(owner->position));
   // Augment it by the camera offset
   owner->getCameraOffset()->movePoint(movedCameraPosition);
   // Save it again for use by the comparator
   cameraPosition = &movedCameraPosition;

   // Get the view vector from the owner ship.
   viewVector = owner->getViewVector();

   // Sort the culledList in z order for tranparencies to work correctly.
   culledList->sort(compareDrawables); 
    
   // Print out the frustum for debugging.
   //curFrustum->print();
   
   // Print debug info for the culled list.
   /*for (listIter = culledList->begin(); listIter != culledList->end(); ++listIter)
   {
     //(*listIter)->debug();
   }*/  
   return culledList;
}

/**
 * Comparator used for sorting by the z axis.
 */
bool Radar::compareDrawables(Drawable* one, Drawable* two) {
   Vector3D cameraToOne(*cameraPosition, *one->position);
   Vector3D cameraToTwo(*cameraPosition, *two->position);
   return viewVector->dot(cameraToOne) > viewVector->dot(cameraToTwo);
}
