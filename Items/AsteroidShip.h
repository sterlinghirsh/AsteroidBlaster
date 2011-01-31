/**
 * AsteroidShip.h
 * Sterling Hirsh
 * This is the ship that a player plays from or can see.
 */

#ifndef __ASTEROIDSHIP_H__
#define __ASTEROIDSHIP_H__

#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"
#include "Shots/AsteroidShot.h"
#include "Graphics/GlutUtility.h"
#include "Utility/Object3D.h"
#include <list>

// Later we'll put in a model for the ship.

extern GLfloat headlight_pos[4];
extern GLfloat headlight_amb[4];
extern GLfloat headlight_diff[4];
extern GLfloat headlight_spec[4];

// Incomplete declaration so we can use the pointer.
class AsteroidShot;

class AsteroidShip : public Object3D {
   public:
      AsteroidShip();
      int getHealth();
      int getScore();
      double getAimX();
      double getAimY();
      void updateAcceleration();
      virtual void update(double timeDiff);

      // We'll have to update this later so the AI can use it.
      void updateShotDirection(double xOffset, double yOffset);
      void keepFiring();
      virtual void draw();
      // This will be gone soon.
      void checkAsteroidCollisions(std::list<Asteroid3D*>& asteroids);
      virtual void handleCollision(Object3D* other);

      /**
       * These are going to be the functions that either the local player
       * or the AI player or the networked player will call.
       * Essentially, this is the ship's API.
       * 
       * We give accelerateForward, Right, Up either -1, 0, or 1 to mean
       * backward, none, or forward. You can either fully accelerate or not.
       * Rotation is the opposite. You supply a value, this clamps it and sets it.
       * setBrake accepts a bool. true brakes, false does not brake.
       * fire works the same as setBrake.
       * selectWeapon selects the weapon corresponding to the int weaponType.
       * We will need to prevent weapons that the player does not own from getting selected.
       */
      void accelerateForward(int direction);
      void accelerateRight(int direction);
      void accelerateUp(int direction);
      void setBrake(bool doBrake);
      void setYawSpeed(double yawAmountIn);
      void setRollSpeed(double rollAmount);
      void setPitchSpeed(double pitchAmount);
      void fire(bool startFiring);
      void selectWeapon(int weaponType);

      // Change this to something reasonable once we have a radar.
      // void getRararData();
      // void getAllVisibleAsteroids();

   private:
      Vector3D shotDirection; // If we shoot a shot, where will it go?

      double brakeFactor;
      double shipRadius; // Units, this is the distance from the center to assume that we're hitting something.

      // These could get changed with upgrades.
      double maxForwardAccel;
      double maxUpAccel;
      double maxRightAccel;
      double maxYawSpeed;
      double maxPitchSpeed;
      double maxRollSpeed;

      double shotPhi; // Radians
      double shotTheta; // Radians
      double shotSpeed; // Units per second
      double timeOfLastShot; // Seconds
      double timeOfLastBeam; // Seconds

      double curForwardAccel; // Units per second^2
      double curRightAccel;
      double curUpAccel;
      
      double yawSpeed; // Rad/s
      double pitchSpeed;
      double rollSpeed;
      double maxSpeed; // Units per second

      bool isBraking;
      bool isFiring;
      int score, health;
      int lastGunFired;
      int currentWeapon;

      void updateShotDirectionVector();
      void brake(double brakeFactorIn);
      void forwardAcceleration(double newAcc);
      void rightAcceleration(double newAcc);
      void upAcceleration(double newAcc);

      double xOff, yOff;
};

#endif
