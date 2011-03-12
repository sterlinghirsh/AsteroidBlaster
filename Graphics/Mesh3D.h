/**
 * Sterling Hirsh
 * CPE 471
 * 10-19-10
 *
 * Mesh3D: An object to store and display a 3D mesh.
 */

#ifndef __MESH3D_H__
#define __MESH3D_H__

#include <vector>
#include "Graphics/MeshPoint.h"
#include "Graphics/Face3D.h"

class Mesh3D {
   public:
      std::vector<MeshPoint> points;
      std::vector<Face3D> faces;
      bool drawNormals;
      double xMax, xMin, yMax, yMin, zMax, zMin;

      Mesh3D();
      void drawTextured(bool drawSmooth, GLuint tex);
      void drawLines(bool drawSmooth);
      void draw(bool drawSmooth);
      void draw(bool drawSmooth, bool drawTex);
      void drawPoints(bool drawSmooth);
      void drawPoints(bool drawSmooth, bool drawTex);
      int addPoint(double x, double y, double z, double texX, double texY);
      int addPoint(double x, double y, double z);
      int addPoint(MeshPoint* p);
      void addFace(int p1, int p2, int p3);
};

#endif
