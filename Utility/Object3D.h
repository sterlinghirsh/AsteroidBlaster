/**
 * Object3D
 * Sterling Hirsh / Taylor Arnicar
 * 1-16-22
 * CPE 476
 */

#ifndef __OBJECT3D_H__
#define __OBJECT3D_H__

#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"
#include "Graphics/GlutUtility.h"
#include "Graphics/Camera.h"

class Object3D {
   public:
      double minX, minY, minZ, maxX, maxY, maxZ;
      Point3D* position;
      Vector3D* velocity;
      Vector3D* acceleration;
      Vector3D* axis;
      GLuint displayList;
      Vector3D *up, *right, *forward;
      bool lockUpVector;
      double yawSpeed, pitchSpeed, rollSpeed;
      double angle;
      Point3D* corners[8];

      Object3D(double x, double y, double z, GLuint displayListIn);
      virtual ~Object3D();
      virtual void update(double timeDifference);
      virtual void draw();
      virtual void setCamera(Camera& viewCamera);
      virtual void setYawSpeed(double radiansPerSecond);
      virtual void setPitchSpeed(double radiansPerSecond);
      virtual void setRollSpeed(double radiansPerSecond);
      virtual bool detectCollision(Object3D* other, bool checkOther = true);
      virtual void handleCollision(Object3D* other);
      virtual void drawBoundingBox();

   private:
      void yaw(double angle);
      void roll(double angle);
      void pitch(double angle);

   protected:
      virtual void setCorners(bool deleteFirst = true);
};

#endif
