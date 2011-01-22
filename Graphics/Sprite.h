/**
 * Sterling Hirsh
 * Texture Importer
 * 11/29/2010
 * This loads an image and can move through parts of it on a rectangle.
 */

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Graphics/TextureImporter.h"
#include "Utility/Point3D.h"
#include <string>
#include <list>

class Sprite {
   public:
      TextureImporter* texture;
      int framesX;
      int framesY;
      int totalFrames;
      double framesPerSecond;
      double width;
      double height;
      double frameWidth;
      double frameHeight;
      double startTime;
      materialStruct curMaterial;
      Point3D *position;
      static std::list<Sprite*> sprites;
      bool oneShot;

      Sprite(std::string filename, int framesXIn, int framesYIn, double fpsIn, 
       Point3D* posIn, double drawWidth, double drawHeight);
      bool draw(Point3D* eyePoint);
};

#endif
