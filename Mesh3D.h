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
#include "MeshPoint.h"
#include "Face3D.h"
#include "Vector3D.h"

class Mesh3D {
   public:
      std::vector<MeshPoint> points;
      std::vector<Face3D> faces;
      bool drawNormals;
      double xMax, xMin, yMax, yMin, zMax, zMin;

      Mesh3D() : drawNormals(false) {
         xMax = xMin = yMax = yMin = zMax = zMin = 0;
      }

      void draw(bool drawSmooth) {
         glBegin(GL_TRIANGLES);
         for (unsigned int i = 0; i < faces.size(); ++i) {
            if (!drawSmooth)
               faces[i].normal.addNormal();
            points[faces[i].p1].draw(drawSmooth);
            points[faces[i].p2].draw(drawSmooth);
            points[faces[i].p3].draw(drawSmooth);
         }
         glEnd();

         if (drawNormals) {
            glDisable(GL_LIGHTING);
            if (drawSmooth) {
               glColor3f(0, 1, 0);
               for (unsigned int i = 0; i < points.size(); ++i) {
                  points[i].normal.draw();
               }
            } else {
               glColor3f(0, 1, 1);
               for (unsigned int i = 0; i < faces.size(); ++i) {
                  faces[i].normal.draw();
               }
            }
            glEnable(GL_LIGHTING);
         }
      }

      /**
       * Inserts a point and returns its index.
       */
      int addPoint(double x, double y, double z) {
         MeshPoint point(x, y, z);
         points.push_back(point);
         if (x > xMax) xMax = x;
         if (x < xMin) xMin = x;
         if (y > xMax) yMax = y;
         if (y < xMin) yMin = y;
         if (z > xMax) zMax = z;
         if (z < xMin) zMin = z;
         return points.size();
      }

      int addPoint(MeshPoint* p) {
         return addPoint(p->x, p->y, p->z);
      }

      void addFace(int p1, int p2, int p3) {
         Face3D face(p1 - 1, p2 - 1, p3 - 1);
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
};

#endif
