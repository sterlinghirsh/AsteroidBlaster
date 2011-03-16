/**
 * Anti Inertia
 * Slows down everything that collides with the beam
 * @author Ryuho Kudo
 * @date 3-16-11
 */

#ifndef __ANTIINERTIASHOT_H__
#define __ANTIINERTIASHOT_H__

#include "Shots/Shot.h"

class AntiInertiaShot : public Shot {
   public:
      AntiInertiaShot(Point3D& posIn, Vector3D dirIn, AsteroidShip* const ownerIn);
      virtual void draw();
      virtual void drawGlow();
      virtual void drawShot(bool isGlow);
      virtual void update(double timeDiff);
      int framesAlive;
      double angle;
      double length;
      double farRadius;
      bool detectCollision(Drawable* other, bool checkOther);
      void handleCollision(Drawable* other);
};

#endif
