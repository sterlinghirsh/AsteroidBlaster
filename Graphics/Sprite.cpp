/**
 * Sterling Hirsh
 * Texture Importer
 * 11/29/2010
 * This loads an image and can move through parts of it on a rectangle.
 */


#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include <math.h>
#include "Utility/GameState.h"

using namespace std;

list<Sprite*> Sprite::sprites;

Sprite::Sprite(unsigned int texID, int framesXIn, int framesYIn, double fpsIn, 
 Point3D posIn, double drawWidth, double drawHeight, const GameState* _gameState) : Drawable(_gameState) {
   position->clone(&posIn);
   framesX = framesXIn;
   framesY = framesYIn;
   framesPerSecond = fpsIn;
   width = drawWidth;
   height = drawHeight;
   frameWidth = 1.0 / framesX;
   frameHeight = 1.0 / framesY;
   textureID = texID;
   curMaterial = WhiteSolid;
   totalFrames = framesX * framesY;
   startTime = doubleTime();
   oneShot = true;
   shouldRemove = false;
}

void Sprite::Add(unsigned int texID, int framesXIn, int framesYIn, double fpsIn, 
 Point3D posIn, double drawWidth, double drawHeight, const GameState* _gameState) {
 
   sprites.push_back(new Sprite(texID, framesXIn, framesYIn, fpsIn,
               posIn, drawWidth, drawHeight, _gameState));
}
 
void Sprite::draw() {
   // Update the current frame.
   
   glDepthMask(false);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   //glDepthFunc(GL_ALWAYS);
   glDepthFunc(GL_LESS);
   double curTime = doubleTime();
   int curFrame = (int) floor((curTime - startTime) * framesPerSecond);
   if (curFrame >= totalFrames && oneShot) {
      shouldRemove = true;
   }
   glDisable(GL_LIGHTING);
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();
   position->glTranslate();
   
   double topLeftX = (curFrame % framesX) * frameWidth;
   double topLeftY = 1 - ((curFrame / framesX) * frameHeight);
   
   glDisable(GL_COLOR_MATERIAL);
  // setMaterial(curMaterial);
   glColor3f(1, 1, 1);
   glBindTexture(GL_TEXTURE_2D, textureID);
   
   glDisable(GL_CULL_FACE);
   glUseProgram(billboardShader);
   GLint sizeLoc = glGetUniformLocation(billboardShader, "size");
   glUniform1f(sizeLoc, 1);
   glBegin(GL_QUADS);
   glTexCoord2d(topLeftX, topLeftY);
   glVertex3d(-width/2, height/2, 0);

   glTexCoord2d(topLeftX + frameWidth, topLeftY);
   glVertex3d(width/2, height/2, 0);

   glTexCoord2d(topLeftX + frameWidth, topLeftY - frameHeight);
   glVertex3d(width/2, -height/2, 0);

   glTexCoord2d(topLeftX, topLeftY - frameHeight);
   glVertex3d(-width/2, -height/2, 0);

   glEnd();
   glUseProgram(0);
   glEnable(GL_CULL_FACE);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);
   glDepthFunc(GL_LEQUAL);
   // This is kind of a hack, I think.
   glDepthMask(true);
   //glClear(GL_DEPTH_BUFFER_BIT);
}

/**
 * Static method that draws all the sprites in Sprite::sprites.
 */
void Sprite::updateSprites(double timeDiff) {
   list<Sprite*>::iterator sprite = Sprite::sprites.begin();

   while(sprite != Sprite::sprites.end()) {
      if ((*sprite)->shouldRemove) {
         delete *sprite;
         sprite = Sprite::sprites.erase(sprite);
      } else {
         sprite++;
      }
   }
}

// serialize function
std::string Sprite::serialize() {
   return "Sprite\n";
}

