/**
 * Screen.h
 * A minimap that displays in the corner of the screen.
 * @author Sterling Hirsh
 * @date 2/23/2011
 */

#include "HUD/Screen.h"
#include "Utility/GlobalUtility.h"
#include "Graphics/Texture.h"

Screen::Screen(int x, int y, GLuint tex):
   _x(x), _y(y) {
      _tex = tex;
      hidden = false;
   }

/**
 * This draws the display.
 */
void Screen::draw() {
   // Draw the minimap
   glPushMatrix();

   //glViewport ((GLint) ((float) gameSettings->GW * (1.0f - displaySize)), 0, (GLsizei) ((float) gameSettings->GW * displaySize), (GLsizei) ((float) gameSettings->GH * displaySize));
   //glViewport ((GLint) ((float) gameSettings->GW - (float) _x), 0, (GLsizei) ((float) gameSettings->GW / 4), (GLsizei) ((float) gameSettings->GH / 4));
   glViewport ((GLint) ((float) gameSettings->GW * (0.7) - _x * (gameSettings->GW / 3)), (gameSettings->GH - (_y + 1) * (gameSettings->GH / 3)), (GLsizei) ((float) gameSettings->GW / 3), (GLsizei) ((float) gameSettings->GW / 3));
   glMatrixMode (GL_PROJECTION);      /* Select The Projection Matrix */
   glLoadIdentity ();                     /* Reset The Projection Matrix */

   // Keep our aspect ratio relative to the global width and height
   gluPerspective(VERT_FOV, (double)gameSettings->GW/gameSettings->GH, 0.5, 20);

   glMatrixMode (GL_MODELVIEW);      /* Select The Projection Matrix */
   glLoadIdentity ();                        /* Reset The Modelview Matrix */

   glClear (GL_DEPTH_BUFFER_BIT);      /* Clear Depth Buffer */

   setMaterial(WhiteSolid);

   // eye, lookAt, and up vectors
   //gluLookAt(0, 2, 5, 0, 0, 0,  0, 1, 0);
   gluLookAt(0, 0, 5, 0, 0, 0,  0, 1, 0);
   glDisable(GL_LIGHTING);

   glEnable(GL_TEXTURE_2D);
   //glBindTexture(GL_TEXTURE_2D, Texture::getTexture("Logo.png"));
   glBindTexture(GL_TEXTURE_2D, _tex);
   //glBindTexture(GL_TEXTURE_2D, Texture::getTexture("bloomTex"));
   glEnable(GL_COLOR_MATERIAL);
   glColor3f(1, 1, 1);
   float aspect = (float) gameSettings->GH / (float) gameSettings->GW;
   float screenX = 2.0;
   float screenY = screenX * aspect;
   //float maxTexX = (float)texSize / (float)gameSettings->GW;
   //float maxTexY = (float)texSize / (float)gameSettings->GH;
   float maxTexX = (float)gameSettings->GW / (float)texSize;
   float maxTexY = (float)gameSettings->GH / (float)texSize;
   glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(-screenX, -screenY, 0.0);
   glTexCoord2f(maxTexX, 0.0);
   glVertex3f(screenX, -screenY, 0.0);
   glTexCoord2f(maxTexX, maxTexY);
   glVertex3f(screenX, screenY, 0.0);
   glTexCoord2f(0.0, maxTexY);
   glVertex3f(-screenX, screenY, 0.0);
   glEnd();
   glBindTexture(GL_TEXTURE_2D, 0);
   glDisable(GL_TEXTURE_2D);

   glPopMatrix();

   // Draw the sphere around it.
   glEnable(GL_LIGHTING);
   //glDisable(GL_LIGHT1); // Reenable the normal light.
   //glEnable(GL_LIGHT0);
   glPopMatrix();
}

/**
 * Update aspects of the minimap.
 */
void Screen::update(double timeDiff) {
}

void Screen::toggle() {
   if (hidden) {
      hidden = false;
   } else {
      hidden = true;
   }
}

bool Screen::isEnabled() {
   return (!hidden);
}
