/**
 * Sterling Hirsh
 * CPE 476
 * 1-16-10
 *
 * Mesh3D: An object to store and display a 3D mesh.
 */

#include "Graphics/Mesh3D.h"
#include "Utility/Vector3D.h"
#include <time.h>

Mesh3D::Mesh3D() : drawNormals(false) {
   xMax = xMin = yMax = yMin = zMax = zMin = 0;
   tick_time = 0.0;
}

void Mesh3D::tick(double ms) {
   tick_time += ms;
}

void Mesh3D::drawTextured(bool drawSmooth, GLuint tex) {
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, tex);
   //draw(drawSmooth, true);
   //for (int i = 0; i < faces.size(); i++) {
   int nFaces;
   if (drawAnim) {
      nFaces = std::min((int)(tick_time / 0.02), (int)faces.size());
   } else {
      nFaces = (int)faces.size();
   }
   for (int i = 0; i < nFaces; i++) {
      faces[i].drawFace(drawSmooth, true);
   }
   glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh3D::drawLines(bool drawSmooth) {
   glDisable(GL_LIGHTING);
   int nFaces;
   if (drawAnim) {
      nFaces = std::min((int)(tick_time / 0.02), (int)faces.size());
   } else {
      nFaces = (int)faces.size();
   }
   for (int i = 0; i < nFaces; i++) {
      faces[i].drawLines();
   }
   glEnable(GL_LIGHTING);
}

void Mesh3D::draw(bool drawSmooth, bool drawTex) {
   glDisable(GL_LIGHTING);
   for (int i = 0; i < faces.size(); i++) {
      faces[i].drawFace(drawSmooth);
   }
   glEnable(GL_LIGHTING);
}

void Mesh3D::drawPoints(bool drawSmooth, bool drawTex) {
   for (unsigned int i = 0; i < faces.size(); ++i) {
      if (!drawSmooth)
         faces[i].normal.addNormal();
      /*
      Vector3D v1 = Vector3D(points[faces[i].p1], points[faces[i].p2]);
      Vector3D v2 = Vector3D(points[faces[i].p1], points[faces[i].p3]);
      double x1 = 0.0;
      double x2 = v1.getLength();
      double x3 = v1.dot(v2) / (v1.getLength());
      double y1 = 0.0;
      double y2 = 0.0;
      double y3 = 1.0;
      double scale = std::max(std::max(x2, x3), 1.0);
      printf("1: (%f, %f)\n", x1, y1);
      printf("2: (%f, %f)\n", x2, y2);
      printf("3: (%f, %f)\n", x3, y3);
      */
      /*
      double x1 = 0.0;
      double x2 = 0.5;
      double x3 = 1.0;
      double y1 = 0.0;
      double y2 = 1.0;
      double y3 = 0.0;
      */
      //points[faces[i].p1].draw(drawSmooth, drawTex);
      /*
      faces[i].p1.draw(drawSmooth);
      if (drawTex)
         glTexCoord2d(x1, y1);
      //points[faces[i].p2].draw(drawSmooth, drawTex);
      faces[i].p2.draw(drawSmooth);
      if (drawTex)
         glTexCoord2d(x2, y2);
      //points[faces[i].p3].draw(drawSmooth, drawTex);
      faces[i].p3.draw(drawSmooth);
      if (drawTex)
         glTexCoord2d(x3, y3);
      */
      faces[i].draw(drawSmooth, drawTex);
   }
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

void Mesh3D::addFace(int p1, int p2, int p3) {
   //Face3D face(p1 - 1, p2 - 1, p3 - 1);
   MeshFace face;
   face = MeshFace(points[p1 - 1], points[p2 - 1], points[p3 - 1]);
   Vector3D v1(points[p2 - 1].x - points[p1 - 1].x,
         points[p2 - 1].y - points[p1 - 1].y,
         points[p2 - 1].z - points[p1 - 1].z);
   Vector3D v2(points[p3 - 1].x - points[p1 - 1].x,
         points[p3 - 1].y - points[p1 - 1].y,
         points[p3 - 1].z - points[p1 - 1].z);
   face.normal = v1.cross(v2);
   face.normal.normalize();

   faces.push_back(face);
   points[p1 - 1].normal.addUpdate(face.normal);
   points[p2 - 1].normal.addUpdate(face.normal);
   points[p3 - 1].normal.addUpdate(face.normal);
   points[p1 - 1].normal.normalize();
   points[p2 - 1].normal.normalize();
   points[p3 - 1].normal.normalize();
}
