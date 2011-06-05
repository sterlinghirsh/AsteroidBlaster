/**
 * A class to store a face in a Mesh3D.
 * Chris Brenton
 * 4/12/11
 */

#include "Graphics/MeshFace.h"
#include "Utility/GameState.h"
// TODO: a better way.
#include "Items/AsteroidShip.h"
#include "Items/BoundingSpace.h"

#define ASTEROID3D_LINE_W 0.5
#define MAX_INDEPENDENT_FACES 500

std::list<MeshFace*> MeshFace::independentFaces;

MeshFace::MeshFace(MeshPoint& _p1, MeshPoint& _p2, MeshPoint& _p3, const GameState* _gameState) :
   Drawable(_gameState),
   p1(_p1), p2(_p2), p3(_p3) {
      setFaceColor(0.0f, 0.0f, 0.0f);
      setLineColor(1.0f, 1.0f, 1.0f);
      offsetBy(0.0);
      texture = 0;
      textured = false;
      timeExploded = 0;
      lifetime = 2;
      velocity = new Vector3D(0, 0, 0);
      minX = minY = minZ = -0.5;
      maxX = maxY = maxZ =  0.5;
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
   //unused variable
   //GLfloat lineW = (GLfloat) ((gameState->worldSize / shipDist * ASTEROID3D_LINE_W + 1.0) / 1);

   //glEnable(GL_COLOR_MATERIAL);
   glDisable(GL_CULL_FACE);
   //glDisable(GL_LIGHTING);
   //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   //setMaterial(Rock);
   //glLineWidth(lineW);
   glPolygonOffset(1.0f, 1.0f);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glPushMatrix();
   position->glTranslate();
   glRotated(angle, axis->x, axis->y, axis->z);

   //double scaleAmount = sqrt((lifetime - (doubleTime() - timeExploded)) / lifetime);
   double scaleAmount = clamp(2.0 * (lifetime - (doubleTime() - timeExploded)) / lifetime, 0, 1);
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
   drawFace(true, true);

   glPopMatrix();

   glDisable(GL_TEXTURE_2D);
   glDisable(GL_POLYGON_OFFSET_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   //glLineWidth(1);
   //glDisable(GL_COLOR_MATERIAL);
}

void MeshFace::draw(bool drawSmooth, bool drawTex) {
   drawLines();
   //drawFace(drawSmooth, drawTex);
}

void MeshFace::drawLines() {
   double shipDist = p1.distanceFrom(*gameState->ship->position);
   GLfloat lineW = (GLfloat) ((gameState->worldSize / shipDist * ASTEROID3D_LINE_W + 1.0) / 2);

   float curAlpha = timeExploded == 0 ? 1.0f :
      (GLfloat) sqrt((lifetime - (doubleTime() - timeExploded)));

   if (timeExploded > 0.0) {
      //fboBegin();
      glDisable(GL_CULL_FACE);
      if (gameSettings->drawDeferred) {
         GLenum buffers[] = {NOLIGHT_BUFFER, GLOW_BUFFER};
         glDrawBuffers(2, buffers);
      }
   }

   glLineWidth(lineW);
   glColor4f(lineR, lineG, lineB, curAlpha);
   //if (!gameSettings->drawDeferred) {
   //normal.glColor(curAlpha);
   //}
   glBegin(GL_LINE_LOOP);
   p1.draw();
   p2.draw();
   p3.draw();
   glEnd();
   if (timeExploded > 0.0) {
      glEnable(GL_CULL_FACE);
      //fboEnd();
   }
   glLineWidth(1.0);
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

   //if (timeExploded > 0.0 && gameSettings->drawDeferred) {
   if (timeExploded > 0.0) {
      //fboBegin();
      glDisable(GL_CULL_FACE);
      if (gameSettings->drawDeferred) {
         GLenum buffers[] = {NOLIGHT_BUFFER, GLOW_BUFFER};
         glDrawBuffers(2, buffers);
      }
   }

   if (drawSmooth)
      normal.addNormal();
   if (gameSettings->drawDeferred) {
      glColor4f(faceR, faceG, faceB, curAlpha);
   } else {
      normal.glColor(curAlpha);
   }
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
   if (timeExploded > 0.0) {
      glEnable(GL_CULL_FACE);
      //fboEnd();
   }
}

void MeshFace::nullPointers() {
   Drawable::nullPointers();
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
   angle += rotationSpeed * timeDiff;

   gameState->cube->constrain(this);
   velocity->scalarMultiply(timeDiff).movePoint(*position);

   minPosition->clone(position);
   maxPosition->clone(position);
   minPosition->offsetBy(minX, minY, minZ);
   maxPosition->offsetBy(maxX, maxY, maxZ);

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

   double scaleAmount = clamp(2.0 * (lifetime - (doubleTime() - timeExploded)) / lifetime, 0, 1);
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

void MeshFace::Add(MeshFace* face) {
   independentFaces.push_back(face);
}

void MeshFace::updateIndependentFaces(double timeDifference) {
   while (independentFaces.size() >= MAX_INDEPENDENT_FACES) {
      delete independentFaces.front();
      independentFaces.pop_front();
   }

   std::list<MeshFace*>::iterator face = MeshFace::independentFaces.begin();

   while(face != MeshFace::independentFaces.end()) {
      if (*face != NULL) {
         (*face)->update(timeDifference);
         if ((*face)->shouldRemove) {
            delete *face;
            face = MeshFace::independentFaces.erase(face);
         } else {
            ++face;
         }
      } else {
         // Don't delete a NULL pointer...
         face = MeshFace::independentFaces.erase(face);
      }
   }
}

void MeshFace::Clear() {
   std::list<MeshFace*>::iterator face = MeshFace::independentFaces.begin();

   while(face != MeshFace::independentFaces.end()) {
      delete *face;
      face = MeshFace::independentFaces.erase(face);
   }
}
