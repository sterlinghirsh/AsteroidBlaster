/**
 * Shot.cpp
 * Sterling Hirsh
 * A shot in 3D Asteroids.
 */

#include "Shots/Shot.h"
#include "Items/BoundingWall.h"
#include "Network/gamestate.pb.h"
#include <iostream>

materialStruct ShotMaterial = {
  {1, 0, 0, 1},
  {1, 0, 0, 1},
  {1, 0, 0, 1},
  {8.0}
};

Shot::Shot(Point3D& posIn, Vector3D dirIn, int _weaponIndex,
 AsteroidShip* const ownerIn, const GameState* _gameState) : 
 Object3D(_gameState),
 owner(ownerIn),
 weaponIndex(_weaponIndex) {
   *position = posIn;
   velocity = new Vector3D(dirIn);
   //velocity->setLength(40.0);
   timeFired = gameState->getGameTime();
   isBeam = false;
   ownerid = ownerIn->id;
}

Shot::~Shot() {
   // Do nothing.
}

void Shot::onRemove() {
   Object3D::onRemove();
}

void Shot::update(double timeDiff) {
   Object3D::update(timeDiff);
   if (gameState->getGameTime() - timeFired > lifetime) {
      shouldRemove = true;
   }
}

void Shot::draw() {
   Object3D::draw();
}

/**
 * Don't draw shots in the map.
 * For now :)
 */
void Shot::drawInMinimap() {
   return;
}

void Shot::debug() {
   printf("Shot: (min, max, position, velocity)\n");
   minPosition->print();
   maxPosition->print();
   position->print();
   velocity->print();
   printf("--\n");
}

Point3D Shot::getWallIntersectionPoint(BoundingWall* wall) {
   if (!isBeam) {
      return Object3D::getWallIntersectionPoint(wall);
   }

   Point3D toReturn(*position);
   Point3D pointOnPlane = wall->normal.scalarMultiply(-wall->wallSize);

   // Calculate intersection between beam and wall.
   Vector3D normalizedDirection = velocity->getNormalized();
   Vector3D wallNormal = wall->normal.scalarMultiply(-1); // To make it face away from the center.
   double rayDotNormal = normalizedDirection.dot(wallNormal);
   if (rayDotNormal <= 0) {
      wall->actuallyHit = false;
      return toReturn;
   }

   Vector3D rayOriginToPointOnPlane(*position, pointOnPlane);

   double distance = rayOriginToPointOnPlane.dot(wallNormal) / rayDotNormal;
   if (distance < 0) {
      wall->actuallyHit = false;
      return toReturn;
   }

   toReturn.addUpdate(normalizedDirection.scalarMultiply(distance));
   return toReturn;
}

double Shot::getDamage(Object3D* collidedObject) {
   return damage;
}

void Shot::save(ast::Entity* ent) {
   Object3D::save(ent);
   ent->set_ownerid(ownerid);
   ent->set_lifetime(lifetime);
   ent->set_timefired(timeFired);
   ent->set_weaponindex(weaponIndex);
   ent->set_damage(damage);

}

void Shot::load(const ast::Entity& ent) {
   Object3D::load(ent);

   if (ent.has_ownerid()) {
      ownerid = ent.ownerid();
      owner = dynamic_cast<AsteroidShip*>(
       gameState->custodian[ownerid]);
   }

   if (ent.has_lifetime())
      lifetime = ent.lifetime();

   if (ent.has_timefired())
      timeFired = ent.timefired();

   if (ent.has_weaponindex())
      weaponIndex = ent.weaponindex();

   if (ent.has_damage())
      damage = ent.damage();

}
