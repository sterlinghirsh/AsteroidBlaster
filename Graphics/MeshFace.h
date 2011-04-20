/**
 * A class to store a face in a Mesh3D.
 * Chris Brenton
 * 4/12/11
 */

#ifndef __MESHFACE_H__
#define __MESHFACE_H__

#define EXPLOSION_SCALE 10.0

#include "Graphics/MeshPoint.h"
#include "Utility/Vector3D.h"
#include "Items/Object3D.h"

class MeshFace : public Object3D {
   public:
      MeshFace() : Object3D(NULL) {};
      MeshFace(MeshPoint& _p1, MeshPoint& _p2, MeshPoint& _p3, const GameState* _gameState);
      virtual ~MeshFace();
      void offsetBy(double scale);
      void moveBy(Vector3D move);
      virtual void draw(bool drawSmooth, bool drawTex = false);
      virtual void draw();
      virtual void drawGlow();
      void drawLines();
      void drawFace(bool drawSmooth = false, bool drawTex = false);
      void setFaceColor(float _r, float _g, float _b);
      void setLineColor(float _r, float _g, float _b);
      MeshPoint p1, p2, p3;
      Vector3D normal;
      Point3D offset;
      float faceR, faceG, faceB;
      float lineR, lineG, lineB;
      virtual void nullPointers();
      virtual void debug();
      GLuint texture;
      bool textured;
      double timeExploded;
      virtual void setTexture(GLuint _tex);
      virtual void update(double timeDiff);
      double lifetime;
};

#endif
