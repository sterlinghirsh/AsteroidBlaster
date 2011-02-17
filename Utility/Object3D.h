/**
 * Object3D
 * Sterling Hirsh / Taylor Arnicar
 * 1-16-11
 * CPE 476
 */

#ifndef __OBJECT3D_H__
#define __OBJECT3D_H__

#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"
#include "Graphics/GlutUtility.h"
#include "Utility/Custodian.h"

// Incomplete class declaration so we can have the pointer to it.
class Custodian;

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
      bool shouldRemove;
      double angle;
      double radius;
      double cullRadius;
      bool shouldConstrain;
      unsigned int minXRank, maxXRank;
      bool shouldDrawInMinimap;
      // Specifies whether or not this Object should be culled by VFC.
      bool shouldBeCulled;

      Object3D(double x, double y, double z, GLuint displayListIn);
      virtual ~Object3D();
      virtual void update(double timeDifference);
      virtual void draw();
      virtual void drawInMinimap();
      virtual void setYawSpeed(double radiansPerSecond);
      virtual void setPitchSpeed(double radiansPerSecond);
      virtual void setRollSpeed(double radiansPerSecond);
      virtual bool detectCollision(Object3D* other, bool checkOther = true);
      virtual void drawBoundingBox();
      virtual void drawBoundingSphere();
      virtual void handleCollision(Object3D* other);
      virtual void debug();
      virtual void setTargeted(bool a);
      virtual bool isTargeted();
      double getCullRadius();

      void setCustodian(Custodian* cust);
      void glRotate(bool doTranspose = true);
   private:

   protected:
      double yawSpeed, pitchSpeed, rollSpeed;
      GLuint displayList;
      Custodian* custodian;
      bool targeted;

      void yaw(double angle);
      void roll(double angle);
      void pitch(double angle);
      void updateBoundingBox();
};

#endif
