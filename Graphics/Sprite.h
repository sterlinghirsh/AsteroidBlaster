/**
 * Sterling Hirsh
 * Texture Importer
 * 11/29/2010
 * This loads an image and can move through parts of it on a rectangle.
 */

#ifndef __SPRITE_H__
#define __SPRITE_H__


#include "Items/Drawable.h"
#include <string>
#include <list>

class Sprite : public Drawable {
   public:
      unsigned int textureID;
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
      static std::list<Sprite*> sprites;
      static void updateSprites(double timeDiff);
      bool oneShot;

      Sprite(unsigned int texID, int framesXIn, int framesYIn, double fpsIn, 
       Point3D posIn, double drawWidth, double drawHeight);
      static void Add(unsigned int texID, int framesXIn, int framesYIn, double fpsIn, 
       Point3D posIn, double drawWidth, double drawHeight);
      virtual void draw();
};

#endif
