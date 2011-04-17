/**
 * A class to store a face in a Mesh3D.
 * Chris Brenton
 * 4/12/11
 */

#ifndef __MESHPOINT_H
#define __MESHPOINT_H

#define FADE_TIME 1.0f
#define EXPLOSION_SCALE 10.0

#include "Graphics/MeshPoint.h"
#include "Utility/Vector3D.h"

class MeshFace {
   public:
      MeshFace() {};
      MeshFace(MeshPoint& _p1, MeshPoint& _p2, MeshPoint& _p3);
      ~MeshFace() {};
      void offsetBy(double scale);
      void moveBy(Vector3D move);
      void draw(bool drawSmooth = false, bool drawTex = false);
      void drawLines();
      void drawFace(bool drawSmooth = false, bool drawTex = false);
      void setFaceColor(float _r, float _g, float _b);
      void setLineColor(float _r, float _g, float _b);
      MeshPoint p1, p2, p3;
      Vector3D normal;
      Point3D offset;
      float faceR, faceG, faceB;
      float lineR, lineG, lineB;
      float alphaDiff;
};

#endif
