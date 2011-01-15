/**
 * Class for drawing a skybox
 * Sterling Hirsh
 * 471
 * 11/7/2010
 */

#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Graphics/TextureImporter.h"
#include "Items/AsteroidShip.h"
#include <string>

class Skybox {
   public:
      materialStruct curMaterial;
      TextureImporter* tex;
      Skybox(std::string filename) {
         materialStruct WhiteSolid = {
           {1, 1, 1, 1},
           {0.0, 0.0, 0.0, 0.0},
           {0.0, 0.0, 0.0, 0.0},
           {0.0}
         };
         curMaterial = WhiteSolid;

         tex = new TextureImporter(filename);
      }

      void draw(AsteroidShip& ship) {
         glDisable(GL_DEPTH_TEST);
         glDisable(GL_LIGHTING);
         glPushMatrix();
         glLoadIdentity();
         glEnable(GL_TEXTURE_2D);
         ship.setCamera(false);
         glScalef(3, 3, 3);
         drawcube();

         glDisable(GL_TEXTURE_2D);
         glPopMatrix();
         glEnable(GL_LIGHTING);
         glEnable(GL_DEPTH_TEST);
      }
   private:
      void drawcube() {
        glColor3f(1.0, 1.0, 1.0); //white
        materials(curMaterial);
        glBindTexture(GL_TEXTURE_2D, tex->texID);
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
};

#endif
