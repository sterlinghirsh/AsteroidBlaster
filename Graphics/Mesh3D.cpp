/**
 * Sterling Hirsh
 * CPE 476
 * 1-16-10
 *
 * Mesh3D: An object to store and display a 3D mesh.
 */

#include "Graphics/Mesh3D.h"
#include "Utility/Vector3D.h"
#include "Graphics/Texture.h"
#include <time.h>

Mesh3D::Mesh3D() : drawNormals(false) {
   xMax = xMin = yMax = yMin = zMax = zMin = 0;
   tick_time = 0.0;
}

Mesh3D::~Mesh3D() {
   //printf("deleting faces.\n");
   for (unsigned i = 0; i < faces.size(); ++i) {
      delete faces[i];
   }
   faces.empty();
   //printf("finished deleting faces.\n");
}

void Mesh3D::tick(double ms) {
   tick_time += ms;
}

void Mesh3D::setFaceColor(float r, float g, float b) {
   for (unsigned i = 0; i < faces.size(); i++) {
      faces[i]->setFaceColor(r, g, b);
   }
}

void Mesh3D::setLineColor(float r, float g, float b) {
   for (unsigned i = 0; i < faces.size(); i++) {
      faces[i]->setLineColor(r, g, b);
   }
}

void Mesh3D::drawTextured(bool drawSmooth, GLuint tex) {
   glDisable(GL_BLEND);

   int nFaces;
   if (drawAnim) {
      nFaces = std::min((int)(tick_time / 0.02), (int)faces.size());
   } else {
      nFaces = (int)faces.size();
   }
   if (gameSettings->drawDeferred) {
      //fboBegin();
      GLenum buffers[] = {ALBEDO_BUFFER, NORMAL_BUFFER, GLOW_BUFFER, NOLIGHT_BUFFER};
      glDrawBuffers(4, buffers);
      glUseProgram(gBufferShader);
   }

   //glUseProgram(0);
   glEnable(GL_TEXTURE_2D);
   
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, tex);
   /*
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, Texture::getTexture("depthTex"));
   */

   /*
   GLint texLoc = glGetUniformLocation(gBufferShader, "texIn");
   glUniform1i(texLoc, 0);
   GLint depthLoc = glGetUniformLocation(gBufferShader, "depth");
   glUniform1i(depthLoc, 1);
   GLint farLoc = glGetUniformLocation(gBufferShader, "far");
   //glUniform1f(farLoc, farClip);
   glUniform1f(farLoc, 100.0);
   */

   glEnable(GL_LIGHTING);
   for (int i = 0; i < nFaces; i++) {
      faces[i]->drawFace(drawSmooth, true);
   }

   glDisable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, 0);
   glActiveTexture(GL_TEXTURE0);
   for (int i = 0; i < nFaces; i++) {
      faces[i]->drawLines();
   }

   glUseProgram(0);

   glEnable(GL_BLEND);
}

void Mesh3D::drawLines(bool drawSmooth) {
   glDisable(GL_LIGHTING);
   if (gameSettings->drawDeferred) {
      //fboBegin();
      //GLenum buffers[] = {GL_NONE, GL_NONE, GLOW_BUFFER, NOLIGHT_BUFFER};
      //glDrawBuffers(4, buffers);
      GLenum buffers[] = {GLOW_BUFFER, NOLIGHT_BUFFER};
      glDrawBuffers(2, buffers);

      //glUseProgram(gBufferShader);
   }

   int nFaces;
   if (drawAnim) {
      nFaces = std::min((int)(tick_time / 0.02), (int)faces.size());
   } else {
      nFaces = (int)faces.size();
   }
   for (int i = 0; i < nFaces; i++) {
      faces[i]->drawLines();
   }

   if (gameSettings->drawDeferred) {
      //glUseProgram(0);
      //fboEnd();
   }
   glEnable(GL_LIGHTING);
}

void Mesh3D::draw(bool drawSmooth, bool drawTex) {
   drawTextured(drawSmooth, 0);
}

/**
 * Inserts a point and returns its index.
 */
int Mesh3D::addPoint(double x, double y, double z) {
   MeshPoint point(x, y, z);
   points.push_back(point);
   if (x > xMax) xMax = x;
   if (x < xMin) xMin = x;
   if (y > xMax) yMax = y;
   if (y < xMin) yMin = y;
   if (z > xMax) zMax = z;
   if (z < xMin) zMin = z;
   return (int) points.size();
}

int Mesh3D::addPoint(MeshPoint* p) {
   return addPoint(p->x, p->y, p->z);
}

void Mesh3D::addFace(int p1, int p2, int p3, const GameState* _gameState) {
   MeshFace* face = new MeshFace(points[p1 - 1], points[p2 - 1], points[p3 - 1], _gameState);
   Vector3D v1(points[p2 - 1].x - points[p1 - 1].x,
         points[p2 - 1].y - points[p1 - 1].y,
         points[p2 - 1].z - points[p1 - 1].z);
   Vector3D v2(points[p3 - 1].x - points[p1 - 1].x,
         points[p3 - 1].y - points[p1 - 1].y,
         points[p3 - 1].z - points[p1 - 1].z);
   face->normal = v1.cross(v2);
   face->normal.normalize();
   //face.setFaceColor(1.0f, 0.0f, 0.0f);
   face->setLineColor(1.0f, 1.0f, 1.0f);

   points[p1 - 1].normal.addUpdate(face->normal);
   points[p2 - 1].normal.addUpdate(face->normal);
   points[p3 - 1].normal.addUpdate(face->normal);
   points[p1 - 1].normal.normalize();
   points[p2 - 1].normal.normalize();
   points[p3 - 1].normal.normalize();

   faces.push_back(face);
}
