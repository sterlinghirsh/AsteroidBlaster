/**
 * Drawable. Any item that is to be drawn to the screen.
 * Taylor Arnicar
 * 3-6-11
 * CPE 476
 */

#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "Utility/CollisionTypes.h"
#include "Utility/Point3D.h"

// Incomplete class declaration so we can have the pointer to it.
class Custodian;
class BoundingWall;
class GameState;

class Drawable {
   //public variables------------------------------
   public:
      //pointer to things owned by this class
      Point3D* position;
      Vector3D* velocity;
      Point3D* minPosition;
      Point3D* maxPosition;
      CollisionType* collisionType;
      
      //pointer to things not owned by this class
      const GameState* gameState;
      Custodian* custodian;
      
      double radius; // TODO
      bool shouldRemove; // True when custodian should remove this.
      double cullRadius; // TODO
      bool shouldBeCulled; // Specifies whether or not this Object should be culled by VFC.
      unsigned int minXRank, maxXRank; // TODO
      bool shouldConstrain; // TODO
      bool shouldDrawInMinimap; // TODO
      // This might not belong here.
      double minX, minY, minZ, maxX, maxY, maxZ;
      

      
      
   //private variables------------------------------
   private:
   
   
   //public functions------------------------------
   public:
      //constructors
      Drawable();
      Drawable(const GameState* _gameState);
      
      //destructor
      virtual ~Drawable();
      
      //draw() is here to be overwritten by all subclasses.
      virtual void draw() = 0;
      
      //drawGlow() is here to be overwritten by all subclasses.
      virtual void drawGlow() {return;}
      
      //To be overwritten by all subclasses. This is an empty stub only.
      virtual void update(double timeDifference) {return;}
      
      virtual void drawInMinimap();
      virtual void hitWall(BoundingWall* wall);
      virtual void debug();
      virtual void nullPointers();
      virtual Point3D getWallIntersectionPoint(BoundingWall* wall);
      virtual Point3D& getMinPosition() const;
      virtual Point3D& getMaxPosition() const;
      
      // serialize 
      virtual std::string serialize();
      
      double unrootedDist(Point3D *other); //Comparator used for sorting by the z axis.
      double unrootedDist(Drawable *other);
      
      //Returns the radius of the object which should be used for View Frustum Culling.
      double getCullRadius();
      
      
   //private functions------------------------------
   private:


};

#endif
