/**
 * ProgressBar.cpp
 * Displays a progress bar on the hud, for things like cooldown or health.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/22/2011
 */

#include "HUD/ProgressBar.h"
#include "Utility/GlobalUtility.h"
#include "Utility/Texture.h"

ProgressBar::ProgressBar(float _height, float _width, float _x, float _y) :
 height(_height), width(_width), x(_x), y(_y), amount(0), outerBoxThickness(0.01), hasIcon(false) {
}

void ProgressBar::setAmount(float _amount) {
   amount = clamp(_amount, 0, 1);
}

void ProgressBar::draw() {
   float drawHeight = amount * height;
   const float backgroundZOffset = -0.01; // Some small negative number.
   glPushMatrix();
      glTranslatef(x, y, 0);
      setMaterial(BlackSolid);
      glBegin(GL_QUADS);
      // First draw black box.
      glColor3f(0, 0, 0);
      glVertex3f(-outerBoxThickness, height + outerBoxThickness, backgroundZOffset);
      glVertex3f(width + outerBoxThickness, height + outerBoxThickness, backgroundZOffset);
      glVertex3f(width + outerBoxThickness, -outerBoxThickness, backgroundZOffset);
      glVertex3f(-outerBoxThickness, -outerBoxThickness, backgroundZOffset);

      // Color goes from red to green.
      glColor3f(2 * (1 - amount), 2 * amount, 0);
      
      glVertex3f(0, drawHeight, 0); // Top left
      glVertex3f(width, drawHeight, 0); // top Right
      glVertex3f(width, 0, 0); // bottom Right
      glVertex3f(0, 0, 0); // Bottom left
      glEnd();
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

         glTranslatef(width/2, (-width * 2), 0);
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
