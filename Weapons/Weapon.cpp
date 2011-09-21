/**
 * Weapon Class for AsteroidBlaster
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * This is a superclass to real weapons.
 */

#include "Weapons/Weapon.h"
#include "Items/Object3D.h"
#include "Items/AsteroidShip.h"
#include "Utility/Custodian.h"
#include <sstream>
#include "Graphics/Texture.h"

/**
 * Initialize the ship and timeLastFired.
 */
Weapon::Weapon(AsteroidShip* owner, int _index)
: ship(owner), index(_index), timeLastFired(0), icon("ZoeRedEyes") {
   level = 1;
   levelMax = 5;
   purchased = false;
   ammoPrice = 1;
   ammoAmount = 1;
   weaponPrice = 2;
   fireBackwards = false;
   /* Initialize the range to an arbitrary large number for all weapons.
    * Children will set their own range if necessary.
    */
   range = 9999;

   r = g = b = 0;

   currentHeat = 0;
   overheatLevel = 5;
   heatPerShot = 0.3; // Blaster default.
   overheated = false;

   activationTimer.reset();
}

Weapon::~Weapon() {
   // Do nothing.
}

void Weapon::update(double timeDiff) {
   if (currentHeat > 0) {
      currentHeat = std::max(currentHeat - timeDiff, 0.0);
   }

   if (overheated && currentHeat == 0) {
      overheated = false;
   } else if (currentHeat > overheatLevel) {
      overheated = true;
   }
}

/**
 * Increase currentHeat by heatPerShot / level by default.
 */
void Weapon::addHeat(double newHeat) {
   if (!ship->gameState->godMode) {
      currentHeat += newHeat;
   }
}

/**
 * Returns true if the weapon is temporarily disabled.
 */
bool Weapon::isOverheated() {
   return overheated;
}

/**
 * Returns true if the weapon is ready to be fired.
 */
bool Weapon::isCooledDown() {
   if (ship->gameState->godMode) {
      return doubleTime() > timeLastFired + 0.05;
   } else {
      return !isOverheated() &&
       (doubleTime() > timeLastFired + (coolDown/((double)level)));
   }
   
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
      //ss << "Buy " << name << " for $" << buyPrice();
      ss << "This should never happen!";
   } else if(level < levelMax){
      ss << "Upgrade " << name << " to level " << (level+1) << " for " << buyPrice();
   } else {
      ss << name << " maxed out!";
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
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
   
   glBindTexture(GL_TEXTURE_2D, Texture::getTexture(icon));
   /* Really Nice Perspective Calculations */
   glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
   glBlendFunc(GL_ONE, GL_ONE);

   // Draw box around weapons.
   if (selected) {
      glColor4d(r / 4, g / 4, b / 4, 0.2);
   } else {
      glColor4d(0.2, 0.2, 0.2, 0.2);
      glScalef(0.5f, 0.5f, 0.5f);
   }
   glBegin(GL_QUADS);
   glVertex3f(-1.0, -1.0, 0.0);
   glVertex3f(-1.0, 1.0, 0.0);
   glVertex3f(1.0, 1.0, 0.0);
   glVertex3f(1.0, -1.0, 0.0);
   glEnd();

   if (selected) {
      glColor4d(r, g, b, 1.0);
      glLineWidth(2);
      glDisable(GL_LINE_SMOOTH);
      glBegin(GL_LINE_LOOP);
      glVertex3f(-1.0, -1.0, 0.5);
      glVertex3f(-1.0, 1.0, 0.5);
      glVertex3f(1.0, 1.0, 0.5);
      glVertex3f(1.0, -1.0, 0.5);
      glEnd();
      glEnable(GL_LINE_SMOOTH);
   }

   if (selected) {
      glColor4d(r, g, b, 1.0f);
   } else {
      glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
   }
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_TEXTURE_2D);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0f, 1.0f);
   glVertex3f(-1.0, -1.0, 1.0);
   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(-1.0, 1.0, 1.0);
   glTexCoord2f(1.0f, 0.0f);
   glVertex3f(1.0, 1.0, 1.0);
   glTexCoord2f(1.0f, 1.0f);
   glVertex3f(1.0, -1.0, 1.0);
   glEnd();
   glDisable(GL_TEXTURE_2D);

}

bool Weapon::isReady() {
   return activationTimer.getTimeLeft() <= 0 && isCooledDown();
}

void Weapon::stopSounds() {
   // Do nothing.
}
