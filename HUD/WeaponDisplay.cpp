/**
 * WeaponDisplay.cpp
 * Displays a progress bar on the hud, for things like cooldown or health.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/22/2011
 */

#include "HUD/WeaponDisplay.h"
#include "Graphics/Texture.h"
#include "Utility/GameState.h"
#include "Utility/GlobalUtility.h"

WeaponDisplay::WeaponDisplay(float _height, float _width, float _x, float _y, const GameState *_gameState) :
   height(_height), width(_width), x(_x), y(_y), gameState(_gameState) {
      icon = "ShotIcon";
   }

void WeaponDisplay::draw() {
   int curWeapon = gameState->ship->currentWeapon;
   //const float backgroundZOffset = -0.01f; // Some small negative number.
   const float backgroundZOffset = 0.0f; // Some small negative number.
   glPushMatrix();
   glTranslatef(x, y, 0);
   glScalef(0.67f, 0.67f, 0.67f);
   glPushMatrix();
   //setMaterial(BlackSolid);
   //glColor3f(1, 1, 1);
   int maxWeapon = gameState->ship->weapons.size();
   glDisable(GL_CULL_FACE);
   //glScalef(width / 2, height / 2, 0);
   //glScalef(width / (float)maxWeapon, height / (float)maxWeapon, 0);
   glScalef(1.0f / (float)maxWeapon, 1.0f / (float)maxWeapon, 0);
   //glScalef(1.0f * gameState->aspect / 3.0f, 1.0f / 3.0f, 1.0f);
   //glScalef(3.0f / (float)gameSettings->GW, 3.0f / (float)gameSettings->GH, 0);
   float smallW = gameState->aspect / (float)(maxWeapon + 1);
   float largeW = 2.0f * gameState->aspect / (float)(maxWeapon + 1);
   int count = 0;
   for (unsigned i = 0; i < gameState->ship->weapons.size(); i++) {
      glPushMatrix();
      glTranslatef(2.1f * ((float)i - (float)(maxWeapon - 1) / 2), 0, 0);
      gameState->ship->weapons[i]->drawIcon(i == curWeapon);
      glPopMatrix();
      count++;
   }
   //gameState->ship->weapons[curWeapon]->drawIcon();
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glPopMatrix();
   glPopMatrix();
}

void WeaponDisplay::setIcon(std::string _texture) {
   icon = _texture;
}
