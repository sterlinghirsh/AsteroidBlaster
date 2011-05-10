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
   //public variables------------------------------
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
      bool oneShot;
      static std::list<Sprite*> sprites;
      
   //private variables------------------------------
   private:
   
   
   //public functions------------------------------
   public:
      //constructor
      Sprite(unsigned int texID, int framesXIn, int framesYIn, double fpsIn, 
       Point3D posIn, double drawWidth, double drawHeight, const GameState* _gameState);
      
      //virtual functions 
      virtual void draw();
      virtual std::string serialize();
            
      //add function, adds to the sprite list
      static void Add(unsigned int texID, int framesXIn, int framesYIn, double fpsIn, 
       Point3D posIn, double drawWidth, double drawHeight, const GameState* _gameState);
      
      static void updateSprites(double timeDiff);
      virtual void update(double timeDiff) {};
      
   //private functions------------------------------
   private:
};

#endif
