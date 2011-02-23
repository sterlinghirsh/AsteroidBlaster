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

/* Provides a complete, unfiltered reading of the whole environment.
 */
std::list<Object3D*>* Radar :: getFullReading() {
   /* Get the custodian out of gameState, and get its vector of Objects.
    * Use this vector to construct a new copied vector (to be safe), and return that one.
    */
   std::vector<Object3D*>* all (gameState->custodian.getListOfObjects());
   
   // The new list, which will be returned
   std::list<Object3D*>* allNew = new std::list<Object3D*>();
   
   // As opposed to the checker, the one doing the checking. 
   Object3D* checkee;
   
   // Iterate over all the Object3D's the custodian gave us
   for (vectorIter = all->begin(); vectorIter != all->end(); ++vectorIter)
   {
      // Copy all of the ptrs into a list
      checkee = *vectorIter;
      allNew->push_back(checkee);
   }
   
   return allNew;
}

/* Provides a filtered reading of the environment based on what's near the owner AsteroidShip.
 */
std::list<Object3D*>* Radar :: getNearbyReading() {
   /* Get the custodian out of gameState, and get its vector of Objects.
    * Use this vector to construct a new copied vector (to be safe), and return that one.
    */
   std::vector<Object3D*>* all (gameState->custodian.getListOfObjects());
   
   std::list<Object3D*>* nearList = new std::list<Object3D*>();
   
   // As opposed to the checker, the one doing the checking. 
   Object3D* checkee;
   
   // Iterate over all the Object3D's the custodian gave us
   for (vectorIter = all->begin(); vectorIter != all->end(); ++vectorIter)
   {
      checkee = *vectorIter;
      
      // If the object is within the required nearby distance to be drawn
      if(checkee != NULL && owner->position->distanceFrom(*(checkee->position)) <= REQUIRED_NEARBY_DIST)
      {
         nearList->push_back(checkee);
      }
   }
   
   return nearList;
}

/* Provides a filtered reading of the environment based on what's within the camera's viewFrustum right now.
 */
std::list<Object3D*>* Radar :: getViewFrustumReading() {
   // Get the custodian out of gameState, and get its vector of Objects.
   std::vector<Object3D*>* all (gameState->custodian.getListOfObjects());
   
   // Turn the vector of all Objects into a list, & cull it
   std::list<Object3D*>* culledList = curFrustum->cullToViewFrustum(all);
   
   //curFrustum->print();
   
   /*for (listIter = culledList->begin(); listIter != culledList->end(); ++listIter)
   {
     //(*listIter)->debug();
   }*/  
   return culledList;
}
