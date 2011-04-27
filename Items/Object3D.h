/**
 * Object3D
 * Sterling Hirsh / Taylor Arnicar
 * 1-16-11
 * CPE 476
 */

#ifndef __OBJECT3D_H__
#define __OBJECT3D_H__

#include "Items/Drawable.h"
#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"
#include "Utility/GlobalUtility.h"
#include <queue>

class Object3D : public Drawable {
   public:
      Vector3D* acceleration;
      Vector3D* axis;
      Vector3D *up, *right, *forward;
      bool lockUpVector;
      double angle;

      Object3D(const GameState* _gameState);
      virtual ~Object3D();
      virtual void update(double timeDifference);
      virtual void setYawSpeed(double radiansPerSecond);
      virtual void setPitchSpeed(double radiansPerSecond);
      virtual void setRollSpeed(double radiansPerSecond);
      virtual void drawBoundingBox();
      virtual void drawBoundingSphere();
      virtual void setTargeted(bool a);
      virtual bool isTargeted();

      virtual void draw();
      virtual void drawGlow();

      void glRotate(bool doTranspose = true);
      virtual void addAcceleration(Vector3D* newAccel);
      virtual void addInstantAcceleration(Vector3D* newAccel);

      virtual void nullPointers();
      double rotationSpeed;
      virtual Point3D* sphereCollideWithRay(const Point3D& origin, Vector3D direction, double* hitDistance = NULL);
   private:

   protected:
      double yawSpeed, pitchSpeed, rollSpeed;
      GLuint displayList;
      std::queue<Vector3D*> accelerations;
      std::queue<Vector3D*> instantAccelerations;
      bool targeted;

      void yaw(double angle);
      void roll(double angle);
      void pitch(double angle);
      void updateBoundingBox();

      virtual void updateAcceleration(double timeDiff);

};

#endif
