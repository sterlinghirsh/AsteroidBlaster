/**
 * viewFrustum: It's got useful functions to cull items that aren't in your view frustum
 * Taylor Arnicar / Sterling Hirsh
 * 2-7-11
 * CPE 476
 */

#include "Utility/GlobalUtility.h"
#include "Utility/ViewFrustum.h"

ViewFrustum :: ViewFrustum() {
   // This should not need to be initialized, since it happens when calling culltoViewFrustum.
   //glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&projMatrix);
   top = new Plane();
   bottom = new Plane();
   left = new Plane();
   right = new Plane();
   near = new Plane();
   far = new Plane();
}

// Deconstructor doesn't need to do anything.
ViewFrustum :: ~ViewFrustum() {
   delete top;
   delete bottom;
   delete left;
   delete right;
   delete near;
   delete far;
}

/* Returns true if the object3D is completely outside of the view frustum planes.
 * Returns false if it's even part-way inside.
 */
bool ViewFrustum :: checkOutside(Drawable* obj) {
   // Decide each of the Results based on obj's min & max x, y, & z.
   return !( 
      left -> onPositiveSide(obj) && right -> onPositiveSide(obj) &&
      top -> onPositiveSide(obj) && bottom -> onPositiveSide(obj) &&
      near -> onPositiveSide(obj) && far -> onPositiveSide(obj)); 
}

// Take in a list of all Object3D's, and cull it down to a list of only the Object 3D's in front.
std::list<Drawable*>* ViewFrustum :: cullToViewFrustum(std::vector<Drawable*>* all) {
   // The culled vector to be returned.
   std::list<Drawable*>* returnMe = new std::list<Drawable*>;
   // Set up an iterator to go over the list.
   std::vector<Drawable*> :: iterator iter;

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
   left->a = projMatrix._41 + projMatrix._11;
   left->b = projMatrix._42 + projMatrix._12;
   left->c = projMatrix._43 + projMatrix._13;
   left->d = projMatrix._44 + projMatrix._14;
   
   // Load values for the right clipping plane->
   right->a = projMatrix._41 - projMatrix._11;
   right->b = projMatrix._42 - projMatrix._12;
   right->c = projMatrix._43 - projMatrix._13;
   right->d = projMatrix._44 - projMatrix._14;
   
   // Load values for the top clipping plane->
   top->a = projMatrix._41 - projMatrix._21;
   top->b = projMatrix._42 - projMatrix._22;
   top->c = projMatrix._43 - projMatrix._23;
   top->d = projMatrix._44 - projMatrix._24;
   
   // Load values for the bottom clipping plane->
   bottom->a = projMatrix._41 + projMatrix._21;
   bottom->b = projMatrix._42 + projMatrix._22;
   bottom->c = projMatrix._43 + projMatrix._23;
   bottom->d = projMatrix._44 + projMatrix._24;
   
   // Load values for the near clipping plane->
   near->a = projMatrix._41 + projMatrix._31;
   near->b = projMatrix._42 + projMatrix._32;
   near->c = projMatrix._43 + projMatrix._33;
   near->d = projMatrix._44 + projMatrix._34;
   
   // Load values for the far clipping plane->
   far->a = projMatrix._41 - projMatrix._31;
   far->b = projMatrix._42 - projMatrix._32;
   far->c = projMatrix._43 - projMatrix._33;
   far->d = projMatrix._44 - projMatrix._34;
   
   left->calcMag();
   right->calcMag();
   top->calcMag();
   bottom->calcMag();
   near->calcMag();
   far->calcMag();
 
   // As opposed to the checker, the one doing the checking. 
   Drawable* checkee;
   // call checkOutside() on each of the Object3D's in all.
   // Build a new set of Object3D's out of each of the ones that got back false.
   for (iter = all->begin(); iter != all->end(); ++iter)
   {
      checkee = *iter;

      // Skip this one if it's NULL
      if(checkee == NULL)
         continue;

      /* If it's inside the view frustum, or if it just shouldn't be 
         culled (ex: Beam Shots)
       */
      if(!checkOutside(checkee) || !checkee->shouldBeCulled)
         // Then add it to the list of things to be returned in the culled list.
         returnMe->push_back(checkee);
   }
   return returnMe;
}

/* Prints out details about all of the planes of the view frustum.
 */
void ViewFrustum :: print() {
   printf("Top plane:\n");
   top->print();
   printf("----------\n");
   printf("Bottom plane:\n");
   bottom->print();
   printf("----------\n");
   printf("Left plane:\n");
   left->print();
   printf("----------\n");
   printf("Right plane:\n");
   right->print();
   printf("----------\n");
   printf("Near plane:\n");
   near->print();
   printf("----------\n");
   printf("Far plane:\n");
   far->print();
   printf("----------\n");
   
   printf("NORMALIZED:\n");
   printf("Top plane:\n");
   top->printNormalized();
   printf("----------\n");
   printf("Bottom plane:\n");
   bottom->printNormalized();
   printf("----------\n");
   printf("Left plane:\n");
   left->printNormalized();
   printf("----------\n");
   printf("Right plane:\n");
   right->printNormalized();
   printf("----------\n");
   printf("Near plane:\n");
   near->printNormalized();
   printf("----------\n");
   printf("Far plane:\n");
   far->printNormalized();
   printf("----------\n");
}

