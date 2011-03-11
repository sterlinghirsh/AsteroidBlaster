/**
 * Minimap.h
 * A minimap that displays in the corner of the screen.
 * @author Sterling Hirsh
 * @date 2/23/2011
 */

#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "Items/AsteroidShip.h"
#include <list>
class AsteroidShip;

class Minimap {
   public:
      float displaySize;
      float zoomLevel;
      AsteroidShip* ship;

      int adjustZoomDirection;
      int adjustDisplaySizeDirection;

      // Used for interpolating display sizes.
      double targetZoomLevel;
      double targetDisplaySize;

      double oldDisplaySize;

      Minimap(AsteroidShip* _ship);
      void draw();
      void drawLines(std::list<Object3D*>* objects);
      void setDisplaySize(float newDisplaySize);
      void setZoomLevel(float newZoomLevel);
      void adjustZoom(double changeAmount);
      void adjustDisplaySize(double changeAmount);
      void update(double timediff);
      void toggle();
   private:
      bool hidden;
};

#endif
