/**
 * Basic reader for mesh files made from example code from 471.
 * Sterling Hirsh / Taylor Arnicar
 * 1-18-11
 * CPE 476
 */

#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include <istream>
#include "Graphics/Face3D.h"
#include "Graphics/MeshPoint.h"
#include "Graphics/Mesh3D.h"
#include "Graphics/GlutUtility.h"

class MeshImporter {
   public:
      //for computing the center point and extent of the model
      Mesh3D mesh;
      float cx, cy, cz;
      float max_x, max_y, max_z, min_x, min_y, min_z;
      float max_extent;
      int p1, p2, p3;

      MeshImporter(char* filename);
      void readLine(char* str);
      void readStream(std::istream& is);
      void ReadFile(char* filename);
      void draw();

};

#endif
