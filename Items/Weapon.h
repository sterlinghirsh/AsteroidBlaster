/**
 * @file
 * The weapon interface class.
 * <pre>
 * All weapons must inherit this class. Step one of this is to get all existing
 * weapons defined as Weapon objects. Ships will have a std::list of weapons.
 * This list should be made available to the AI.
 * </pre>
 * @author Mike Smith
 */

#ifndef WEAPON_H
#define WEAPON_H

struct Point3D;

class Weapon {
public:
   virtual double getDamage()=0;
   virtual double getSpeed()=0;
   virtual double getCooldown()=0;
   virtual double getRange()=0;
   virtual Point3D project(Object3D* o)=0;
};

#endif
