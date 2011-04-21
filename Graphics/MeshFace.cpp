/**
 * A class to store a face in a Mesh3D.
 * Chris Brenton
 * 4/12/11
 */

#include "Graphics/MeshFace.h"
#include "Items/Drawable.h"
#include "Utility/GameState.h"

#define ASTEROID3D_LINE_W 0.5

MeshFace::MeshFace(MeshPoint& _p1, MeshPoint& _p2, MeshPoint& _p3, const GameState* _gameState) :
 Object3D(_gameState),
 p1(_p1), p2(_p2), p3(_p3) {
   setFaceColor(0.0f, 0.0f, 0.0f);
   setLineColor(1.0f, 1.0f, 1.0f);
   offsetBy(0.0);
   texture = 0;
   textured = false;
   timeExploded = 0;
   lifetime = 1;
}

MeshFace::~MeshFace() {
   // Do nothing.
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
}

void MeshFace::moveBy(Vector3D move) {
   offset.x = move.x;
   offset.y = move.y;
   offset.z = move.z;
}

void MeshFace::draw() {
   double shipDist = position->distanceFrom(*gameState->ship->position);
   GLfloat lineW = (GLfloat) ((gameState->worldSize / shipDist * ASTEROID3D_LINE_W + 1.0) / 1);
   
   glEnable(GL_COLOR_MATERIAL);
   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   setMaterial(Rock);
   glLineWidth(lineW);
   glPolygonOffset(1.0f, 1.0f);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glPushMatrix();
   position->glTranslate();
   glRotated(angle, axis->x, axis->y, axis->z);

   double scaleAmount = sqrt((lifetime - (doubleTime() - timeExploded)) / lifetime);
   glScaled(scaleAmount, scaleAmount, scaleAmount);

   // Draw lines
   glEnable(GL_POLYGON_OFFSET_LINE);
      drawLines();
   glDisable(GL_POLYGON_OFFSET_LINE);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glPolygonOffset(-1.0f, -1.0f);

   glEnable(GL_TEXTURE_2D);
   if (textured) {
      glBindTexture(GL_TEXTURE_2D, texture);
   }

   // Draw fill
   glEnable(GL_POLYGON_OFFSET_FILL);

      // rotate, scale?
      // Probably more goes here.
      drawFace(false, true);

   glPopMatrix();
   
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_POLYGON_OFFSET_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glLineWidth(1);
   glDisable(GL_COLOR_MATERIAL);
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
   float curAlpha = timeExploded == 0 ? 1.0f : 
    (GLfloat) sqrt((lifetime - (doubleTime() - timeExploded)));

   glColor4f(lineR, lineG, lineB, curAlpha);
   glBegin(GL_LINE_LOOP);
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
   float curAlpha = timeExploded == 0 ? 1.0f : 
    (GLfloat) sqrt((lifetime - (doubleTime() - timeExploded)));

   glColor4f(faceR, faceG, faceB, curAlpha);
   glBegin(GL_TRIANGLES);
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

void MeshFace::nullPointers() {
   Object3D::nullPointers();
   // Nothing here.
}

void MeshFace::debug() {
   printf("MeshFace: (pos, p1, p2, p3, velocity)\n");
   position->print();
   p1.print();
   p2.print();
   p3.print();
   velocity->print();
}

/**
 * This only applies to the draw() method. Not if it's in a mesh.
 */
void MeshFace::setTexture(GLuint _tex) {
   texture = _tex;
   textured = true;
}

void MeshFace::update(double timeDiff) {
   Object3D::update(timeDiff);
   // Something?
   if (doubleTime() - timeExploded > lifetime) {
      shouldRemove = true;
   }
}

void MeshFace::drawGlow() {
   double shipDist = position->distanceFrom(*gameState->ship->position);
   GLfloat lineW = (GLfloat) ((gameState->worldSize / shipDist * ASTEROID3D_LINE_W + 1.0) / 2);
   
   glEnable(GL_COLOR_MATERIAL);
   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   setMaterial(Rock);
   glLineWidth(lineW);
   glPolygonOffset(1.0f, 1.0f);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glPushMatrix();
   position->glTranslate();
   glRotated(angle, axis->x, axis->y, axis->z);

   double scaleAmount = sqrt((lifetime - (doubleTime() - timeExploded)) / lifetime);
   glScaled(scaleAmount, scaleAmount, scaleAmount);

   // Draw lines
   glEnable(GL_POLYGON_OFFSET_LINE);
      drawLines();
   glDisable(GL_POLYGON_OFFSET_LINE);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glPolygonOffset(-1.0f, -1.0f);

   glEnable(GL_TEXTURE_2D);
   if (textured) {
      glBindTexture(GL_TEXTURE_2D, texture);
   }

   /*
   // Draw fill
   glEnable(GL_POLYGON_OFFSET_FILL);

      // rotate, scale?
      // Probably more goes here.
      drawFace(false, true);

   */
   glPopMatrix();
   
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_POLYGON_OFFSET_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glLineWidth(1);
   glDisable(GL_COLOR_MATERIAL);
}
