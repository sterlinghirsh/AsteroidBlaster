/**
 * ProgressBar.cpp
 * Displays a progress bar on the hud, for things like cooldown or health.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/22/2011
 */

#include "HUD/ProgressBar.h"
#include "Utility/GlobalUtility.h"
#include "Graphics/Texture.h"

ProgressBar::ProgressBar(float _height, float _width, float _x, float _y) :
   height(_height), width(_width), x(_x), y(_y), amount(0), outerBoxThickness(0.01f), hasIcon(false), isHorizontal(false), xSkew(0.0), ySkew(0.0) {
      centered = false;
   }

void ProgressBar::setAmount(float _amount) {
   amount = (float) clamp(_amount, 0, 1);
}

void ProgressBar::setHorizontal(bool horiz) {
   isHorizontal = horiz;
}

void ProgressBar::setSkew(float _xSkew, float _ySkew) {
   xSkew = _xSkew;
   ySkew = _ySkew;
}

void ProgressBar::draw() {
   //float drawHeight = amount * height;
   float drawHeight;
   float drawWidth;
   if (isHorizontal) {
      drawHeight = height;
      drawWidth = amount * width;
   } else {
      drawHeight = amount * height;
      drawWidth = width;
   }

   float backgroundZOffset = 0.0f; // Some small negative number.
   glPushMatrix();
   glTranslatef(x, y, 0);

   if (!centered) {
      glTranslatef(width / 2, -height / 2, 0);
   }
   


   glDisable(GL_LIGHTING);
   // Offsets for bottom left (bl), bottom right (br), top left (tl),
   // and top right (tr) corners, based on xSkew and ySkew.
   float leftScale, rightScale, bottomScale, topScale;
   /*
      leftScale = 1.0f - (1.0f / 2.0f) * (xSkew + 1.0f);
      rightScale = 1.0f + (1.0f / 2.0f) * (xSkew + 1.0f);
      bottomScale = 1.0f - (1.0f / 2.0f) * (ySkew + 1.0f);
      topScale = 1.0f + (1.0f / 2.0f) * (ySkew + 1.0f);
      */
   leftScale = (1.0f - xSkew) / 2.0f;
   rightScale = (1.0f + xSkew) / 2.0f;
   bottomScale = (1.0f - ySkew) / 2.0f;
   topScale = (1.0f + ySkew) / 2.0f;

   glBegin(GL_QUADS);
   // First draw black box.
   glColor3f(0, 0, 0);
   glVertex3f(-(outerBoxThickness + (width * bottomScale)),
         -(outerBoxThickness + (height * leftScale)),
         backgroundZOffset);
   glVertex3f(-(outerBoxThickness + (width * topScale)),
         (outerBoxThickness + (height * leftScale)),
         backgroundZOffset);
   glVertex3f((outerBoxThickness + (width * topScale)),
         (outerBoxThickness + (height * rightScale)),
         backgroundZOffset);
   glVertex3f((outerBoxThickness + (width * bottomScale)),
         -(outerBoxThickness + (height * rightScale)),
         backgroundZOffset);
   glEnd();

   backgroundZOffset += 0.1f;
   
   glEnable(GL_TEXTURE_2D);

   if (isHorizontal) {
      glBindTexture(GL_TEXTURE_2D, Texture::getTexture("weaponbarbackgroundHoriz2"));
   } else {
      glBindTexture(GL_TEXTURE_2D, Texture::getTexture("weaponbarbackgroundVert2"));
   }

   glBegin(GL_QUADS);

   // Now background of the inside.
   glColor3f(1, 1, 1);

   if (isHorizontal) {
      glTexCoord2d( 0.0, 1.0 );
      glVertex3f(-(width * bottomScale),
            -(drawHeight * leftScale),
            backgroundZOffset);
      
      glTexCoord2d( 0.0, 0.0 );
      glVertex3f(-(width * topScale),
            (drawHeight * leftScale),
            backgroundZOffset);
      
      glTexCoord2d( 1.0, 0.0 );
      glVertex3f(((width) * topScale),
            (drawHeight * rightScale),
            backgroundZOffset);

      glTexCoord2d( 1.0, 1.0 );
      glVertex3f(((width) * bottomScale),
            -(drawHeight * rightScale),
            backgroundZOffset);
   } else {
      glTexCoord2f( 0.0f, 1.0f );
      glVertex3f(-(drawWidth * bottomScale),
            -(height * leftScale),
            backgroundZOffset);
      glTexCoord2f( 0.0f, 0.0f );
      glVertex3f(-(drawWidth * topScale),
            ((height) * leftScale),
            backgroundZOffset);
      glTexCoord2f( 1.0f, 0.0f );
      glVertex3f((drawWidth * topScale),
            ((height) * rightScale),
            backgroundZOffset);
      glTexCoord2f( 1.0f, 1.0f );
      glVertex3f((drawWidth * bottomScale),
            -(height * rightScale),
            backgroundZOffset);
   }

   glEnd();

   if (isHorizontal) {
      glBindTexture(GL_TEXTURE_2D, Texture::getTexture("weaponbarbackgroundHoriz"));
   } else {
      glBindTexture(GL_TEXTURE_2D, Texture::getTexture("weaponbarbackgroundVert"));
   }

   glBegin(GL_QUADS);
   
   backgroundZOffset += 0.1f;
   // Color goes from red to green.
   glColor3f(2 * (1 - amount), 2 * amount, 0);
   if (isHorizontal) {
      glTexCoord2f( 0.0f, 0.0f );
      glVertex3f(-(width * bottomScale),
            -(drawHeight * leftScale),
            backgroundZOffset);
      glTexCoord2f( 0.0f, 1.0f );
      glVertex3f(-(width * topScale),
            (drawHeight * leftScale),
            backgroundZOffset);
      glTexCoord2f( 1.0f, 1.0f );
      glVertex3f(((drawWidth * 2 - width) * topScale),
            (drawHeight * rightScale),
            backgroundZOffset);
      glTexCoord2f( 1.0f, 0.0f );
      glVertex3f(((drawWidth * 2 - width) * bottomScale),
            -(drawHeight * rightScale),
            backgroundZOffset);
   } else {
      glTexCoord2f( 0.0f, 0.8f );
      glVertex3f(-(drawWidth * bottomScale),
            -(height * leftScale),
            backgroundZOffset);
      glTexCoord2f( 0.0f, 0.0f );
      glVertex3f(-(drawWidth * topScale),
            ((drawHeight * 2 - height) * leftScale),
            backgroundZOffset);
      glTexCoord2f( 1.0f, 0.0f );
      glVertex3f((drawWidth * topScale),
            ((drawHeight * 2 - height) * rightScale),
            backgroundZOffset);
      glTexCoord2f( 1.0f, 0.8f );
      glVertex3f((drawWidth * bottomScale),
            -(height * rightScale),
            backgroundZOffset);
   }
   glEnd();
   glDisable(GL_TEXTURE_2D);

   if (hasIcon) {
      glEnable(GL_TEXTURE_2D);
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
            GL_CLAMP );
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
            GL_CLAMP );
      glBindTexture(GL_TEXTURE_2D, Texture::getTexture(icon));
      /* Really Nice Perspective Calculations */
      glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
      glColor3f(1, 1, 1);
      glDisable(GL_CULL_FACE);

      //glTranslatef(width/2, (-width * 2), 0);
      if (!isHorizontal) {
         glTranslatef(width/2, (-height / 2 - width * 2), 0);
      } else {
         glTranslatef(height/2, (-height * 2), 0);
      }
      glScalef(width, width, width);
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      glBegin(GL_QUADS);
      /* Front Face */
      /* Bottom Left Of The Texture and Quad */

      glTexCoord2f( 0.0f, 1.0f ); glVertex2f( -1.0f, -1.0f);
      /* Bottom Right Of The Texture and Quad */
      glTexCoord2f( 1.0f, 1.0f ); glVertex2f(  1.0f, -1.0f);
      /* Top Right Of The Texture and Quad */
      glTexCoord2f( 1.0f, 0.0f ); glVertex2f(  1.0f,  1.0f);
      /* Top Left Of The Texture and Quad */
      glTexCoord2f( 0.0f, 0.0f ); glVertex2f( -1.0f,  1.0f);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   }
   glPopMatrix();
}

void ProgressBar::setIcon(std::string _texture) {
   hasIcon = true;
   icon = _texture;
}

void ProgressBar::setCentered(bool isCentered) {
   centered = isCentered;
}
