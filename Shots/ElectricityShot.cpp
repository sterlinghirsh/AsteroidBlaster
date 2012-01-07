/**
 * ElectricityShot.cpp
 * Incredibly attractive shots.
 * Vwahhhh
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date Valentine's Day
 * <3
 */

#include "Items/Shard.h"
#include "Shots/ElectricityShot.h"
#include <math.h>

#include "Network/gamestate.pb.h"

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

static float flashiness = 0;
const double damageBase = 3;
const double defaultLength = 80;
const double startLength = 1;
const double growSpeed = 80;

ElectricityShot::ElectricityShot(Point3D& posIn, Vector3D dirIn, int _weaponIndex,
 AsteroidShip* const ownerIn, const GameState* _gameState) : 
 Shot(posIn, dirIn, _weaponIndex, ownerIn, _gameState) {
   type = TYPE_ELECTRICITYSHOT;
   persist = true;
   length = startLength;

   forward = new Vector3D(*velocity); // Goofy.
   up = new Vector3D(0, 1, 0);
   // Set endPoint2 100 units away.
   setPosAndDir(*position, *velocity);

   shouldConstrain = false;
   /* Make sure tractor beam shots aren't culled from the view frustum (necessary to make them appear)
   */
   shouldBeCulled = false;
   updateBoundingBox();
   setStrength(1);
   collisionType = new CollisionRay(length, *velocity, *position);
   hitYet = false;
}

/**
 */
void ElectricityShot::update(double timeDiff) {
   // We have to reset the length after a hit.
   if (gameState->gsm != ClientMode) {
      if (length < defaultLength) {
         length = std::min(length + (timeDiff * growSpeed), defaultLength);
      }
   }
}

void ElectricityShot::drawGlow() {
   drawShot(true);
}

void ElectricityShot::draw() {
   drawShot(false);
}

/**
 */
void ElectricityShot::drawShot(bool isGlow) {
   if (!isGlow)
      glUseProgram(elecShader);
   glPushMatrix();

   //floats used in loop iteration
   GLint loc1;
   float j;
   float k;
   float x;
   float y;
   float z;

   //multipliers for randomness in lightning
   float rot;
   float srot;

   //width of inidvidual lightning lines
   float thickness = 5.0;
   if (isGlow) {
      thickness = 10.0;
   }

   //density of the lightning in the beam
   float density = 2;

   //Width of the lightning shot
   int elecWidth = 25;

   //how fast you want the lighting flashing from blue to white. Higher number == faster flash
   float flash = .7f;

   Point3D start(*position);
   velocity->movePoint(start, length);
   start.glTranslate();

   glRotate();
   flashiness = flashiness + flash;
   //if (flashiness >= 360 ) {
   if (flashiness >= 2 * M_PI) {
      flashiness = 0;
   }
   if (!isGlow) {
      loc1 = glGetUniformLocation(elecShader,"flash");
      glUniform1f(loc1,flashiness);
      setMaterial(GreenShiny);
   } else {
      glEnable(GL_COLOR_MATERIAL);
      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      glColor3f(0.5, 0.5, 1.0);
   }

   glLineWidth(thickness);

   glBegin(GL_LINES);

   //Creates the twisted lines whose vertices are sent to the shader to modify
   for(k = -density/2; k < density/2; k = k+1){
      x = 0;
      y = 0;
      z = (float) length;
      for(j = 0; j < length ; j = j+(float)(rand() % 10)){
         glVertex3f(x, y, z);

         rot = (float) (rand() % elecWidth - elecWidth/2);
         srot = (float) (rand() % elecWidth - elecWidth/2);
         glVertex3d(srot * (1 / length), rot  * (1 / length), length - j - 2);
         x = (float) (srot * (1 / length));
         y = (float) (rot * (1 / length));
         z = (float) (length - j - 2);
      }
   }
   glEnd();


   glLineWidth(1.0);
   glPopMatrix();
   if (!isGlow) {
      glUseProgram(0);
   } else {
      glDisable(GL_COLOR_MATERIAL);
   }
}

void ElectricityShot::setPosAndDir(Vector3D& newPos, Vector3D& newDir) {
   position->clone(&newPos);
   velocity->clone(&newDir); // It should be a unit vector.
   forward->clone(velocity);

   Point3D endPoint1(*position);
   Point3D endPoint2(*position);

   // Set endPoint2 100 units away.
   velocity->movePoint(endPoint2, length);
   // Correct for position when calculating endpoint1 and 2.
   Vector3D positionVector(*position);
   positionVector = positionVector.scalarMultiply(-1);
   positionVector.movePoint(endPoint1);
   positionVector.movePoint(endPoint2);
   
   // Now set min/max xyz
   minX = std::min(endPoint1.x, endPoint2.x);
   maxX = std::max(endPoint1.x, endPoint2.x);
   minY = std::min(endPoint1.y, endPoint2.y);
   maxY = std::max(endPoint1.y, endPoint2.y);
   minZ = std::min(endPoint1.z, endPoint2.z);
   maxZ = std::max(endPoint1.z, endPoint2.z);
   
   updateBoundingBox();
}

void ElectricityShot::setStrength(double strength) {
   damage = damageBase * strength;
}

bool ElectricityShot::saveDiff(const ast::Entity& old, ast::Entity* ent) {
   bool changed = Shot::saveDiff(old, ent);
   
   if (length != old.length()) {
      ent->set_length(length);
      changed = true;
   }

   return changed;
}

void ElectricityShot::save(ast::Entity* ent) {
   Shot::save(ent);
   ent->set_length(length);
}

void ElectricityShot::load(const ast::Entity& ent) {
   Shot::load(ent);
   
   if (ent.has_length())
      length = ent.length();
   
   if (gameState->gsm == ClientMode && owner == gameState->ship) {
      velocity->updateMagnitude(owner->shotDirection);
   }

   setPosAndDir(*position, *velocity);
}

