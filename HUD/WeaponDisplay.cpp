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
   //glScalef(0.67f, 0.67f, 0.67f);
   glPushMatrix();
   //setMaterial(BlackSolid);
   //glColor3f(1, 1, 1);

   // Make a list of weapons that are purchased.
   std::vector<Weapon*> purchasedWeapons;
   std::vector<Weapon*>* shipWeapons = &gameState->ship->weapons; // For convenience
   std::vector<Weapon*>::iterator weaponIter;

   for (weaponIter = shipWeapons->begin(); weaponIter != shipWeapons->end(); weaponIter++) {
      if ((*weaponIter)->purchased) {
         purchasedWeapons.push_back(*weaponIter);
      }
   }

   int numWeapons = purchasedWeapons.size();
   glDisable(GL_CULL_FACE);
   //glScalef(width / 2, height / 2, 0);
   //glScalef(width / (float)numWeapons, height / (float)numWeapons, 0);
   //glScalef(1.0f / (float)numWeapons, 1.0f / (float)numWeapons, 0);
   glScaled(0.13, 0.13, 1);
   //glScalef(1.0f * gameState->aspect / 3.0f, 1.0f / 3.0f, 1.0f);
   //glScalef(3.0f / (float)gameSettings->GW, 3.0f / (float)gameSettings->GH, 0);
   float smallW = gameState->aspect / (float)(numWeapons + 1);
   float largeW = 2.0f * gameState->aspect / (float)(numWeapons + 1);
   int count = 0;

   double smallIconWidth = 1.2; // 1 + 0.2 space.
   double iconPosition =  -smallIconWidth * numWeapons / 2;

   for (unsigned i = 0; i < shipWeapons->size(); i++) {
      if (!(*shipWeapons)[i]->purchased) {
         continue;
      }
      glPushMatrix();
      if (i == curWeapon)
         iconPosition += smallIconWidth / 2;
      glTranslated(iconPosition, 0, 0);

      (*shipWeapons)[i]->drawIcon(i == curWeapon);
      glPopMatrix();
      if (i == curWeapon)
         iconPosition += smallIconWidth / 2;
      iconPosition += smallIconWidth;
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
