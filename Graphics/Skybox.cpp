/**
 * Class for drawing a skybox
 * Sterling Hirsh
 * CPE 476
 * 1-16-11
 */

#include "Graphics/Skybox.h"
#include "Utility/GlobalUtility.h"
#include "Utility/Texture.h"

Skybox::Skybox() {
   curMaterial = WhiteSolid;
}

void Skybox::draw(Camera* camera) {
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_LIGHTING);
   glPushMatrix();
   glLoadIdentity();
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_CULL_FACE);
   camera->setCamera(false);
   glScalef(3, 3, 3);
   drawcube();

   glDisable(GL_TEXTURE_2D);
   glEnable(GL_CULL_FACE);
   glPopMatrix();
   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
}

void Skybox::drawcube() {
  glColor3f(1.0, 1.0, 1.0); //white
  setMaterial(curMaterial);
  glBindTexture(GL_TEXTURE_2D, Texture::getTexture("stars.bmp"));
  glBegin(GL_QUADS);
  
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, 0.5, 0.5); //v5
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, 0.5, 0.5); //v6
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, -0.5, 0.5); //v7
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, -0.5, 0.5); //v8
  
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, -0.5, -0.5); //v1
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, -0.5, -0.5); //v2
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, 0.5, -0.5); //v3
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, 0.5, -0.5); //v4
  
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, -0.5, -0.5); //v1
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-0.5, 0.5, -0.5); //v4
  glTexCoord2f(1.0, 1.0);
  glVertex3f(-0.5, 0.5, 0.5); //v5
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, -0.5, 0.5); //v8
   
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, 0.5, -0.5); //v4
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, 0.5, -0.5); //v3
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, 0.5, 0.5); //v6
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, 0.5, 0.5); //v5
  
  glTexCoord2f(0.0, 0.0);
  glVertex3f(0.5, 0.5, -0.5); //v3
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, -0.5, -0.5); //v2
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, -0.5, 0.5); //v7
  glTexCoord2f(1.0, 0.0);
  glVertex3f(0.5, 0.5, 0.5); //v6
 
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-0.5, -0.5, -0.5); //v1
  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.5, -0.5, -0.5); //v2
  glTexCoord2f(1.0, 1.0);
  glVertex3f(0.5, -0.5, 0.5); //v7
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-0.5, -0.5, 0.5); //v8
  glEnd();
}
