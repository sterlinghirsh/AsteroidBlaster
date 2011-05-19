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
#include "Items/AsteroidShip.h"

WeaponDisplay::WeaponDisplay(float _height, float _width, float _x, float _y, const GameState *_gameState) :
 gameState(_gameState), height(_height), width(_width), x(_x), y(_y) {
   icon = "ShotIcon";
}

void WeaponDisplay::draw() {
   unsigned curWeapon = gameState->ship->currentWeapon;
   glPushMatrix();
   glTranslatef(x, y, 0);
   glPushMatrix();

   // Make a list of weapons that are purchased.
   std::vector<Weapon*>* shipWeapons = &gameState->ship->weapons; // For convenience
   std::vector<Weapon*>::iterator weaponIter;

   int numWeapons = 0;

   for (weaponIter = shipWeapons->begin(); weaponIter != shipWeapons->end(); weaponIter++) {
      if ((*weaponIter)->purchased) {
         ++numWeapons;
      }
   }

   glDisable(GL_CULL_FACE);
   glScaled(0.13, 0.13, 1);
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
   
   glPopMatrix();
   glPopMatrix();
}

void WeaponDisplay::setIcon(std::string _texture) {
   icon = _texture;
}
