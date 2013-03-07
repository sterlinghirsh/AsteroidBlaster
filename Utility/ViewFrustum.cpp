/**
 * viewFrustum: It's got useful functions to cull items that aren't in your view frustum
 * Taylor Arnicar / Sterling Hirsh
 * 2-7-11
 * CPE 476
 */

#include "Utility/GlobalUtility.h"
#include "Utility/ViewFrustum.h"
#include "Particles/Particle.h"

ViewFrustum :: ViewFrustum() {
   // This should not need to be initialized, since it happens when calling culltoViewFrustum.
   //glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&projMatrix);
   topPlane = new Plane();
   bottomPlane = new Plane();
   leftPlane = new Plane();
   rightPlane = new Plane();
   nearPlane = new Plane();
   farPlane = new Plane();
}

// Deconstructor doesn't need to do anything.
ViewFrustum :: ~ViewFrustum() {
   delete topPlane;
   delete bottomPlane;
   delete leftPlane;
   delete rightPlane;
   delete nearPlane;
   delete farPlane;
}

/** 
 * A modified version of chedDrawableOutside, used by the AI. This stops the
 * AI from targeting Drawable objects which are slightly outside the field
 * of view, which still need to be drawn.
 */
bool ViewFrustum :: checkTargetableOutside(Drawable* obj) {
   // Decide each of the Results based on obj's min & max x, y, & z.
   return !( 
      leftPlane -> onTargetablePositiveSide(obj) && rightPlane -> onTargetablePositiveSide(obj) &&
      topPlane -> onTargetablePositiveSide(obj) && bottomPlane -> onTargetablePositiveSide(obj) &&
      nearPlane -> onTargetablePositiveSide(obj) && farPlane -> onTargetablePositiveSide(obj)); 
}

/* Returns true if the object3D is completely outside of the view frustum planes.
 * Returns false if it's even part-way inside.
 */
bool ViewFrustum :: checkDrawableOutside(Drawable* obj) {
   // Decide each of the Results based on obj's min & max x, y, & z.
   return !( 
      leftPlane -> onDrawablePositiveSide(obj) && rightPlane -> onDrawablePositiveSide(obj) &&
      topPlane -> onDrawablePositiveSide(obj) && bottomPlane -> onDrawablePositiveSide(obj) &&
      nearPlane -> onDrawablePositiveSide(obj) && farPlane -> onDrawablePositiveSide(obj)); 
}

// Take in a list of all Drawables, and cull it down to a list of only the Drawables in front.
std::list<Object3D*>* ViewFrustum :: cullToViewFrustum(std::vector<Object3D*>* all) {
   // The culled vector to be returned.
   std::list<Object3D*>* returnMe = new std::list<Object3D*>;
   // Set up an iterator to go over the list.
   std::vector<Object3D*> :: iterator iter;

   // Save a copy of the model view matrix.
   glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&modelViewMatrix);
   
   // Switch to the Projection Matrix.
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
      // Multiply the model view matrix onto the projection matrix
      glMultMatrixf((GLfloat*)&modelViewMatrix);
      // Save the new composite matrix, which holds the magic key to our view frustum's planes.
      glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&projMatrix);
   // Do all this inside push and pop so we don't mess up the real Projection Matrix.
   glPopMatrix();
   // Switch back to the Modelview Matrix.
   glMatrixMode(GL_MODELVIEW);

   // Load values for the left clipping plane.
   leftPlane->a = projMatrix._41 + projMatrix._11;
   leftPlane->b = projMatrix._42 + projMatrix._12;
   leftPlane->c = projMatrix._43 + projMatrix._13;
   leftPlane->d = projMatrix._44 + projMatrix._14;
   
   // Load values for the right clipping plane->
   rightPlane->a = projMatrix._41 - projMatrix._11;
   rightPlane->b = projMatrix._42 - projMatrix._12;
   rightPlane->c = projMatrix._43 - projMatrix._13;
   rightPlane->d = projMatrix._44 - projMatrix._14;
   
   // Load values for the top clipping plane->
   topPlane->a = projMatrix._41 - projMatrix._21;
   topPlane->b = projMatrix._42 - projMatrix._22;
   topPlane->c = projMatrix._43 - projMatrix._23;
   topPlane->d = projMatrix._44 - projMatrix._24;
   
   // Load values for the bottom clipping plane->
   bottomPlane->a = projMatrix._41 + projMatrix._21;
   bottomPlane->b = projMatrix._42 + projMatrix._22;
   bottomPlane->c = projMatrix._43 + projMatrix._23;
   bottomPlane->d = projMatrix._44 + projMatrix._24;
   
   // Load values for the near clipping plane->
   nearPlane->a = projMatrix._41 + projMatrix._31;
   nearPlane->b = projMatrix._42 + projMatrix._32;
   nearPlane->c = projMatrix._43 + projMatrix._33;
   nearPlane->d = projMatrix._44 + projMatrix._34;
   
   // Load values for the far clipping plane->
   farPlane->a = projMatrix._41 - projMatrix._31;
   farPlane->b = projMatrix._42 - projMatrix._32;
   farPlane->c = projMatrix._43 - projMatrix._33;
   farPlane->d = projMatrix._44 - projMatrix._34;
   
   leftPlane->calcMag();
   rightPlane->calcMag();
   topPlane->calcMag();
   bottomPlane->calcMag();
   nearPlane->calcMag();
   farPlane->calcMag();
 
   // As opposed to the checker, the one doing the checking. 
   Object3D* checkee;
   // call checkOutside() on each of the Object3Ds in all.
   // Build a new set of Object3Ds out of each of the ones that got back false.
   for (iter = all->begin(); iter != all->end(); ++iter) {
      checkee = *iter;

      // Skip this one if it's NULL
      if(checkee == NULL)
         continue;

      /**
       * Only actually add it if it's inside the targetable area
       * (slightly different than the drawable area).
       */
      if(!checkTargetableOutside(checkee) || !checkee->shouldBeCulled) {
         returnMe->push_back(checkee);
      }
   }
   return returnMe;
}

// Take in a list of all Drawables, and cull it down to a list of only the Drawables in front.
std::list<Drawable*>* ViewFrustum :: cullToViewFrustum(std::list<Drawable*>* all) {
   // Set up an iterator to go over the list.
   std::list<Drawable*> :: iterator iter;

   // Save a copy of the model view matrix.
   glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&modelViewMatrix);
   
   // Switch to the Projection Matrix.
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
      // Multiply the model view matrix onto the projection matrix
      glMultMatrixf((GLfloat*)&modelViewMatrix);
      // Save the new composite matrix, which holds the magic key to our view frustum's planes.
      glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&projMatrix);
   // Do all this inside push and pop so we don't mess up the real Projection Matrix.
   glPopMatrix();
   // Switch back to the Modelview Matrix.
   glMatrixMode(GL_MODELVIEW);

   // Load values for the left clipping plane.
   leftPlane->a = projMatrix._41 + projMatrix._11;
   leftPlane->b = projMatrix._42 + projMatrix._12;
   leftPlane->c = projMatrix._43 + projMatrix._13;
   leftPlane->d = projMatrix._44 + projMatrix._14;
   
   // Load values for the right clipping plane->
   rightPlane->a = projMatrix._41 - projMatrix._11;
   rightPlane->b = projMatrix._42 - projMatrix._12;
   rightPlane->c = projMatrix._43 - projMatrix._13;
   rightPlane->d = projMatrix._44 - projMatrix._14;
   
   // Load values for the top clipping plane->
   topPlane->a = projMatrix._41 - projMatrix._21;
   topPlane->b = projMatrix._42 - projMatrix._22;
   topPlane->c = projMatrix._43 - projMatrix._23;
   topPlane->d = projMatrix._44 - projMatrix._24;
   
   // Load values for the bottom clipping plane->
   bottomPlane->a = projMatrix._41 + projMatrix._21;
   bottomPlane->b = projMatrix._42 + projMatrix._22;
   bottomPlane->c = projMatrix._43 + projMatrix._23;
   bottomPlane->d = projMatrix._44 + projMatrix._24;
   
   // Load values for the near clipping plane->
   nearPlane->a = projMatrix._41 + projMatrix._31;
   nearPlane->b = projMatrix._42 + projMatrix._32;
   nearPlane->c = projMatrix._43 + projMatrix._33;
   nearPlane->d = projMatrix._44 + projMatrix._34;
   
   // Load values for the far clipping plane->
   farPlane->a = projMatrix._41 - projMatrix._31;
   farPlane->b = projMatrix._42 - projMatrix._32;
   farPlane->c = projMatrix._43 - projMatrix._33;
   farPlane->d = projMatrix._44 - projMatrix._34;
   
   leftPlane->calcMag();
   rightPlane->calcMag();
   topPlane->calcMag();
   bottomPlane->calcMag();
   nearPlane->calcMag();
   farPlane->calcMag();
 
   // As opposed to the checker, the one doing the checking. 
   Drawable* checkee;
   // call checkOutside() on each of the Drawables in all.
   // Build a new set of Drawables out of each of the ones that got back false.
   iter = all->begin();
   while(iter != all->end()) {
      checkee = *iter;

      // Skip this one if it's NULL
      if(checkee == NULL) {
         iter = all->erase(iter);
         continue;
      }

      /**
       * If it's inside the view frustum, or if it just shouldn't be 
       * culled (ex: Beam Shots)
       */
      if(checkDrawableOutside(checkee) && checkee->shouldBeCulled) {
         // Since we're not skipping particles, then just add it.
         iter = all->erase(iter);
      } else {
         iter++;
      }
   }
   return all;
}

/**
 * Prints out details about all of the planes of the view frustum.
 */
void ViewFrustum::print() {
   printf("Top plane:\n");
   topPlane->print();
   printf("----------\n");
   printf("Bottom plane:\n");
   bottomPlane->print();
   printf("----------\n");
   printf("Left plane:\n");
   leftPlane->print();
   printf("----------\n");
   printf("Right plane:\n");
   rightPlane->print();
   printf("----------\n");
   printf("Near plane:\n");
   nearPlane->print();
   printf("----------\n");
   printf("Far plane:\n");
   farPlane->print();
   printf("----------\n");
   
   printf("NORMALIZED:\n");
   printf("Top plane:\n");
   topPlane->printNormalized();
   printf("----------\n");
   printf("Bottom plane:\n");
   bottomPlane->printNormalized();
   printf("----------\n");
   printf("Left plane:\n");
   leftPlane->printNormalized();
   printf("----------\n");
   printf("Right plane:\n");
   rightPlane->printNormalized();
   printf("----------\n");
   printf("Near plane:\n");
   nearPlane->printNormalized();
   printf("----------\n");
   printf("Far plane:\n");
   farPlane->printNormalized();
   printf("----------\n");
}

