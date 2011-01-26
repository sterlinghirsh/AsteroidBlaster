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

class Object3D {
   public:
      double minX, minY, minZ, maxX, maxY, maxZ;
      Point3D* minPosition;
      Point3D* maxPosition;
      Point3D* position;
      Vector3D* velocity;
      Vector3D* acceleration;
      Vector3D* axis;
      Vector3D *up, *right, *forward;
      bool lockUpVector;
      double angle;
      unsigned int minXRank, minYRank, minZRank, maxXRank, maxYRank, maxZRank;

      Object3D(double x, double y, double z, GLuint displayListIn);
      virtual ~Object3D();
      virtual void update(double timeDifference);
      virtual void draw();
      virtual void setYawSpeed(double radiansPerSecond);
      virtual void setPitchSpeed(double radiansPerSecond);
      virtual void setRollSpeed(double radiansPerSecond);
      virtual bool detectCollision(Object3D* other, bool checkOther = true);
      virtual void drawBoundingBox();

   private:

   protected:
      double yawSpeed, pitchSpeed, rollSpeed;
      GLuint displayList;

      void yaw(double angle);
      void roll(double angle);
      void pitch(double angle);
      void updateBoundingBox();
      virtual void handleCollision(Object3D* other);
};

#endif
