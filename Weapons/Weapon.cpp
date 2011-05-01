/**
 * Weapon Class for AsteroidBlaster
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * This is a superclass to real weapons.
 */

#include "Weapons/Weapon.h"
#include "Items/Object3D.h"
#include "Items/AsteroidShip.h"
#include "Utility/Custodian.h"

/**
 * Initialize the ship and timeLastFired.
 */
Weapon::Weapon(AsteroidShip* owner)
: ship(owner), timeLastFired(0), icon("ShotIcon") {
   level = 1;
   levelMax = 5;
   purchased = false;
   ammoPrice = 1;
   ammoAmount = 1;
   weaponPrice = 2;
   fireBackwards = false;
}

Weapon::~Weapon() {
   // Do nothing.
}

/**
 * Returns true if the weapon is ready to be fired.
 */
bool Weapon::isCooledDown() {
   if (!ship->gameState->godMode) {
      return doubleTime() > timeLastFired + (coolDown/((double)level));
   }
   return doubleTime() > timeLastFired + 0.05;
}

/**
 * Return the name of the weapon.
 */
std::string Weapon::getName() {
   return name;
}

/**
 * Return a double representing how cool the weapon is, from 0 to 1.
 * 0 means just fired, 1 means max cool!
 */
double Weapon::getCoolDownAmount() {
   if (coolDown == 0)
      return 1;
   return clamp((doubleTime() - timeLastFired) / (coolDown/((double)level)), 0, 1);
}

std::string Weapon::weaponString() {
   std::stringstream ss;

   if(!purchased) {
      ss << "Buy " << name << " for $" << buyPrice();
   } else if(level < levelMax){
      ss << "Upgrade " << name << " to level " << (level+1) << " for $" << buyPrice();
   } else {
      ss << name << " upgrade level max!(" << level << ")";
   }

   return ss.str();
}

std::string Weapon::ammoString() {
   std::stringstream ss;

   if(!purchased) {
      ss << "Buy " << name << " first!";
   } else if(curAmmo == -1){
      ss << name << " has no ammo!";
   } else {
      ss << "Buy " << ammoAmount << " " << name << " ammo for $" << ammoPrice << "(" << curAmmo << ")";
   }

   return ss.str();
}

void Weapon::setIcon(std::string iconName) {
   icon = iconName;
}

void Weapon::drawIcon(bool selected) {
   if (!purchased) { return; }
   //glEnable(GL_TEXTURE_2D);
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
         GL_CLAMP );
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
         GL_CLAMP );
   glBindTexture(GL_TEXTURE_2D, Texture::getTexture(icon));
   /* Really Nice Perspective Calculations */
   glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   //glColor3f(1, 1, 1);
   //glColor3f(clr, clr, clr);
   float startX, startY, width, height;
   //width = ;
   if (!selected) {
      glColor4f(0.1f, 0.1f, 0.1f, 0.4f);
      glScalef(0.5f, 0.5f, 0.5f);
   } else {
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
   }
   glBegin(GL_QUADS);
   glTexCoord2f(0.0f, 1.0f);
   glVertex3f(-1.0, -1.0, 1.0);
   glTexCoord2f(1.0f, 1.0f);
   glVertex3f(-1.0, 1.0, 1.0);
   glTexCoord2f(1.0f, 0.0f);
   glVertex3f(1.0, 1.0, 1.0);
   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(1.0, -1.0, 1.0);
   glEnd();
   glDisable(GL_TEXTURE_2D);
}
