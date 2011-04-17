/**
 * A class to store a face in a Mesh3D.
 * Chris Brenton
 * 4/12/11
 */

#include "Graphics/MeshFace.h"

MeshFace::MeshFace(MeshPoint& _p1, MeshPoint& _p2, MeshPoint& _p3) :
   p1(_p1), p2(_p2), p3(_p3) {
      setFaceColor(0.3f, 0.3f, 1.0f);
      setLineColor(0.7f, 0.7f, 1.0f);
      alphaDiff = 0.0;
      offsetBy(0.0);
   }

void MeshFace::setFaceColor(float _r, float _g, float _b) {
   faceR = _r;
   faceG = _g;
   faceB = _b;
}

void MeshFace::setLineColor(float _r, float _g, float _b) {
   lineR = _r;
   lineG = _g;
   lineB = _b;
}

void MeshFace::offsetBy(double scale) {
   moveBy(normal.scalarMultiply(scale * EXPLOSION_SCALE));
   float tmp = alphaDiff;
   alphaDiff = (float)scale;
   if (tmp < alphaDiff) {
      //printf("BIG trouble in little asteroid town\n");
   }
}

void MeshFace::moveBy(Vector3D move) {
   offset.x = move.x;
   offset.y = move.y;
   offset.z = move.z;
}

void MeshFace::draw(bool drawSmooth, bool drawTex) {
   drawLines();
   //drawFace(drawSmooth, drawTex);
}

void MeshFace::drawLines() {
   MeshPoint p1_tmp = MeshPoint(p1.x + offset.x,
         p1.y + offset.y,
         p1.z + offset.z);
   MeshPoint p2_tmp = MeshPoint(p2.x + offset.x,
         p2.y + offset.y,
         p2.z + offset.z);
   MeshPoint p3_tmp = MeshPoint(p3.x + offset.x,
         p3.y + offset.y,
         p3.z + offset.z);
   glBegin(GL_LINE_LOOP);
   glColor4f(lineR, lineG, lineB, 1.0f - alphaDiff / FADE_TIME);
   p1_tmp.draw();
   p2_tmp.draw();
   p3_tmp.draw();
   glEnd();
}

void MeshFace::drawFace(bool drawSmooth, bool drawTex) {
   double x1 = 0.0;
   double x2 = 0.5;
   double x3 = 1.0;
   double y1 = 0.0;
   double y2 = 1.0;
   double y3 = 0.0;
   MeshPoint p1_tmp = MeshPoint(p1.x + offset.x,
         p1.y + offset.y,
         p1.z + offset.z);
   MeshPoint p2_tmp = MeshPoint(p2.x + offset.x,
         p2.y + offset.y,
         p2.z + offset.z);
   MeshPoint p3_tmp = MeshPoint(p3.x + offset.x,
         p3.y + offset.y,
         p3.z + offset.z);
   glBegin(GL_TRIANGLES);
   //glColor4f(faceR, faceG, faceB, 1.0f - alphaDiff / FADE_TIME);
   p1_tmp.draw();
   if (drawTex)
      glTexCoord2d(x1, y1);
   p2_tmp.draw();
   if (drawTex)
      glTexCoord2d(x2, y2);
   p3_tmp.draw();
   if (drawTex)
      glTexCoord2d(x3, y3);
   glEnd();
}
