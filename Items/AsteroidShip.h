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
#include "Shots/AsteroidShotBeam.h"
#include "Graphics/GlutUtility.h"
#include "Utility/Object3D.h"
#include <list>

// Later we'll put in a model for the ship.

extern GLfloat headlight_pos[4];
extern GLfloat headlight_amb[4];
extern GLfloat headlight_diff[4];
extern GLfloat headlight_spec[4];

class AsteroidShip : public Object3D {
   public:
      Vector3D shotDirection;
      int headlight; // GL_LIGHT0
      double forwardAccel; // Units per second^2
      double rightAccel;
      double upAccel;
      double yawAmount;
      double maxSpeed; // Units per second
      double rollFactor;
      double pitchFactor;
      double yawFactor;

      double shotPhi; // Radians
      double shotTheta; // Radians
      double shotSpeed; // Units per second
      double timeOfLastShot; // Seconds
      double timeOfLastBeam; // Seconds

      std::list<AsteroidShot*> shots;
      std::list<AsteroidShot*>::iterator shotIter;
      int lastGunFired;
      double worldSize;
      bool fireShots;
      bool fireBeams;

      double shipRadius; // Units, this is the distance from the center to assume that we're hitting something.
      
      AsteroidShip(int headlightIn, double worldSizeIn);
      void startYaw(double yawAmountIn);
      void updateAcceleration();
      void brake(double brakeFactorIn);
      void forwardAcceleration(double newAcc);
      void rightAcceleration(double newAcc);
      void upAcceleration(double newAcc);
      void updatePosition(double timeDiff, double rollAmount, double pitchAmount);
      void updateShotDirection(double xOffset, double yOffset);
      void keepFiring();
      void fireLasers(double xOffset, double yOffset, int weapon);
      void stopLasers(int weapon);
      virtual void draw();
      void checkAsteroidCollisions(std::list<Asteroid3D*>& asteroids);
   private:
      double brakeFactor;
      void updateShotDirectionVector();
};

#endif
