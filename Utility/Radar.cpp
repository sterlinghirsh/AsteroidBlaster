/**
 * Radar
 * Taylor Arnicar
 * 2-1-2011
 * Every ship will have a radar which it can query to get information about its surroundings.
 */

#include "Radar.h"
#include "Items/AsteroidShip.h"
#include "Particles/Particle.h"
#include "Graphics/Sprite.h"
#include "Graphics/MeshFace.h"
#include "Items/Drawable.h"

// Set up a list iterator to go over a list.
std::list<Drawable*> :: iterator listIter;
// Set up a vector iterator to go over a vector.
std::vector<Object3D*> :: iterator vectorIter;

Point3D* Radar::cameraPosition;
Vector3D* Radar::viewVector;

Radar :: Radar(AsteroidShip* const ship) {
   owner = ship;
   curFrustum = new ViewFrustum();
   custodian = owner->custodian;
   cameraPosition = NULL;
   viewVector = NULL;
}

/* Deconstructor shouldn't need to do anything
 */
Radar :: ~Radar() {
   delete curFrustum;
}

/**
 * Provides a filtered reading of the environment based on what's near 
 * the owner AsteroidShip.
 * The distance which objects must be within to be returned by getMinimapReading()
 * This reading does not include Particles of any type!
 *
 * totalItems is the count of all drawable items that belong in the minimap.
 */
std::list<Drawable*>* Radar :: getMinimapReading(float radius, int& totalItems) {
   /* Get the custodian out of gameState, and get its vector of Objects.
    * Use this vector to construct a new copied vector (to be safe), and return that one.
    */
   std::vector<Object3D*>* all = custodian->getListOfObjects();
   
   std::list<Drawable*>* nearList = new std::list<Drawable*>();
   
   // As opposed to the checker, the one doing the checking. 
   Drawable* checkee;
   
   totalItems = 0;
   // Iterate over all the Object3D's the custodian gave us
   for (vectorIter = all->begin(); vectorIter != all->end(); ++vectorIter)
   {
      checkee = *vectorIter;
      
      // If the object is not a particle & is within the required nearby distance to be drawn
      if(checkee != NULL && checkee->shouldDrawInMinimap) { 
         ++totalItems;
         if (owner->position->distanceFrom(*(checkee->position)) <= radius) {
            nearList->push_back(checkee);
         }
      }
   }
   
   return nearList;
}

/**
 * Provides a filtered reading of the environment based on what's within the camera's viewFrustum right now.
 * This ONLY returns targetable objects, NOT particles.
 * Ex: Asteroid3D's, Shards.
 */
std::list<Object3D*>* Radar :: getTargetableViewFrustumReading() {
   // Get the custodian out of gameState, and copy its vector of Objects into a new vector.
   std::vector<Object3D*>* allObjects = custodian->getListOfObjects();

   // Set the camera temporarily.
   glPushMatrix();
      owner->setCameraToShotOrigin();
      // Turn the vector of allObjects into a list. True says to cull it down to only the targetable ones (no particles)
      std::list<Object3D*>* culledList = curFrustum->cullToViewFrustum(allObjects);
   glPopMatrix();
   
   return culledList;
}

/**
 * Provides a filtered reading of the environment based on what's within the camera's viewFrustum right now.
 */
std::list<Drawable*>* Radar :: getViewFrustumReading() {
   // Get the custodian out of gameState, and copy its vector of Objects into a new vector.
   std::list<Drawable*>* allObjects = new std::list<Drawable*>();
   
   std::vector<Object3D*>* custodianObjects = (custodian->getListOfObjects());
   for (vectorIter = custodianObjects->begin(); vectorIter != custodianObjects->end(); ++vectorIter) {
      allObjects->push_back(*vectorIter);
   }

   // Sprite list iterator.
   std::list<Sprite*>::iterator spriteIter;
   // Set up a particle list iterator.
   std::list<Particle*>::iterator particleIter;
   // Set up a meshface list iterator.
   std::list<MeshFace*>::iterator meshFaceIter;

   // TODO: Reverse this: get the list of particles first, and append objects to that, rather than getting the list of objects, and appending particles to it.
   Drawable* curParticle;
   // Add all of the particles to allObjects.
   for (particleIter = Particle::particles.begin(); particleIter != Particle::particles.end(); ++particleIter) {
      curParticle = *particleIter;
      allObjects->push_back(curParticle);
   }

   for (spriteIter = Sprite::sprites.begin(); spriteIter != Sprite::sprites.end(); ++spriteIter) {
      allObjects->push_back(*spriteIter);
   }
   
   for (meshFaceIter = MeshFace::independentFaces.begin(); meshFaceIter != MeshFace::independentFaces.end(); ++meshFaceIter) {
      allObjects->push_back(*meshFaceIter);
   }

   // Sterling:
   // Consider each particle one at a time and cull it one at a time.
   // Consider each object one at a time and cull it one at a time.
   // Only add an object / particle to the list of things to draw if
   // it has not been culled.

   // Turn the vector of allObjects into a list, & cull it down to only Drawable objects.
   std::list<Drawable*>* culledList = curFrustum->cullToViewFrustum(allObjects);
   
   /* We need to set the camera position which will be used to compare the distances
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
