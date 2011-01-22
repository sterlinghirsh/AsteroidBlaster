/**
 * Player: It's you!
 * Sterling Hirsh / Taylor Arnicar
 * 1-17-11
 * CPE 476
 */

#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"
#include "Utility/Object3D.h"
#include "Items/Enemy.h"

class Player : public Object3D {
   public:
      int score;
      double forwardVelocity;

      Player(bool lockUp);
      virtual ~Player();

      virtual void draw();
      virtual void setForwardVelocity(double newVelocity);
      virtual void update(double timeDifference);
      virtual void reset();
      virtual void handleCollision(Object3D* other);
      
};
