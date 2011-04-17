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
#include "Graphics/MeshFace.h"

class Mesh3D {
   public:
      std::vector<MeshPoint> points;
      //std::vector<Face3D> faces;
      std::vector<MeshFace> faces;
      bool drawNormals;
      double xMax, xMin, yMax, yMin, zMax, zMin;

      double tick_time;
      void tick(double ms);

      Mesh3D();
      void drawTextured(bool drawSmooth, GLuint tex);
      void drawLines(bool drawSmooth);
      void draw(bool drawSmooth = true, bool drawTex = true);
      void drawPoints(bool drawSmooth, bool drawTex = true);
      int addPoint(double x, double y, double z);
      int addPoint(MeshPoint* p);
      void addFace(int p1, int p2, int p3);
      bool drawAnim;
      void setLineColor(float r, float g, float b);
};

#endif
