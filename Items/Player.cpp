/**
 * Player: It's you!
 * Sterling Hirsh / Taylor Arnicar
 * 1-17-11
 * CPE 476
 */

#include "Items/Player.h"

Player::Player(bool lockUp) : Object3D(0, 0, 0, 0) {
   lockUpVector = lockUp;
   forwardVelocity = 0;
   velocity = new Vector3D(0, 0, 0);
   score = 0;
}

Player::~Player() {
   // Do nothing;
}

void Player::reset() {
   forwardVelocity = 0;
   velocity->updateMagnitude(0, 0, 0);
   position->x = position->z = 0;
   // Reset the player's view
   right->updateMagnitude(1, 0, 0);
   up->updateMagnitude(0, 1, 0);
   forward->updateMagnitude(0, 0, 1);
}

void Player::update(double timeDifference) {
   Object3D::update(timeDifference);
   velocity->updateMagnitude(*forward);
   velocity->updateMagnitude(velocity->xMag, 0, velocity->zMag);
   velocity->setLength(forwardVelocity);
}

void Player::draw() {
   // Do nothing;
}

void Player::setForwardVelocity(double newVelocity) {
   forwardVelocity = newVelocity;
}

void Player::handleCollision(Object3D* other) {
   Enemy* enemy;
   if ((enemy = dynamic_cast<Enemy*>(other)) != NULL && !enemy->wasHit) {
      ++score;
   }
}
