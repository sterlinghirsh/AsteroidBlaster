/**
 * Asteroid Blaster -- IN 3D!
 * Graphics Team: Sterling Hirsh, Taylor Arnicar, Ryuho Kudo, Jake Juszak, Chris Brenton
 * AI Team: Taylor Arnicar, Mike Smith, Sean Ghiocel
 * Justin Kuehn, 
 * Final Project for CPE 476 - Winter & Spring 2011
 */
#include <math.h>
#include <list>
#include <sstream>

#include "Graphics/GlutUtility.h"
#include "Utility/Point3D.h"
#include "Items/Asteroid3D.h"
#include "Items/AsteroidShip.h"
#include "Graphics/TextureImporter.h"
#include "Graphics/Skybox.h"
#include "Graphics/Sprite.h"
#include "Graphics/Camera.h"
#include "Items/BoundingSpace.h"
#include "Utility/BitmapTextDisplay.h"
#include "Utility/GameState.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

#define WORLD_SIZE 80.0

double xdouble = 0, ydouble = 0;
double rollAmount = 0, pitchAmount = 0;
double startx, starty;
int TextureImporter::curTexID;
std::map<string, int> TextureImporter::texIDMap;

/* All of the text objects to be drawn each frame. If you want more text drawn, declare it here,
   update it in updateText(), and make it draw in drawAllText().
*/

GameState* gameState = NULL;

double displayTime = 0;
// This double contains the FPS to be printed to the screen each frame.

void init_light() {
   glEnable(GL_LIGHT0);
   // headlight_amb is defined in Asteroidship->h
   glLightfv(GL_LIGHT0, GL_AMBIENT, headlight_amb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, headlight_diff);
   glLightfv(GL_LIGHT0, GL_SPECULAR, headlight_spec);
   glLightfv(GL_LIGHT0, GL_POSITION, headlight_pos);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);
}

void drawSprites() {
   list<Sprite*>::iterator sprite = Sprite::sprites.begin();
   for (; sprite != Sprite::sprites.end(); sprite++) {
      if (!(*sprite)->draw(gameState->ship->position)) {
         sprite = Sprite::sprites.erase(sprite);
         continue;
      }
   }
}

/* Draw the bounding box grid on the world.
*/
void drawGrid() {
   const double wall = WORLD_SIZE / 2;
   const double alpha = 1;
   glDisable(GL_LIGHTING);
   glBegin(GL_LINES);
   for (int i = -wall; i <= wall; i += WORLD_SIZE / 40) {
      glColor4f(0.0, 1.0, 0.0, alpha);
      // Floor
      glVertex3f(-wall, -wall, i);
      glVertex3f(wall, -wall, i);
      glVertex3f(i, -wall, -wall);
      glVertex3f(i, -wall, wall);

      // Ceiling
      glColor4f(0.0, 0.0, 1.0, alpha);
      glVertex3f(-wall, wall, i);
      glVertex3f(wall, wall, i);
      glVertex3f(i, wall, -wall);
      glVertex3f(i, wall, wall);

      // Left Wall
      glColor4f(1.0, 0.0, 1.0, alpha);
      glVertex3f(-wall, -wall, i);
      glVertex3f(-wall, wall, i);
      glVertex3f(-wall, i, -wall);
      glVertex3f(-wall, i, wall);

      // Right Wall
      glColor4f(0.0, 1.0, 1.0, alpha);
      glVertex3f(wall, -wall, i);
      glVertex3f(wall, wall, i);
      glVertex3f(wall, i, -wall);
      glVertex3f(wall, i, wall);

      // Back Wall
      glColor4f(1.0, 1.0, 0.0, alpha);
      glVertex3f(-wall, i, -wall);
      glVertex3f(wall, i, -wall);
      glVertex3f(i, -wall, -wall);
      glVertex3f(i, wall, -wall);

      // Front Wall
      glColor4f(1, 0, 0, alpha);
      glVertex3f(-wall, i, wall);
      glVertex3f(wall, i, wall);
      glVertex3f(i, -wall, wall);
      glVertex3f(i, wall, wall);
   }
   glEnd();
   glEnable(GL_LIGHTING);
}

void drawCrosshair() {
   double crosshairSizeX = 0.05;
   double crosshairSizeY = 0.05;
   double thicknessX = 0.01;
   double thicknessY = 0.01;
   glPushMatrix();
   glLoadIdentity();
   materials(WhiteTransparent);
   useOrtho();
   glBegin(GL_QUADS);
   glVertex3f(xdouble - crosshairSizeX, ydouble + thicknessY, 0);
   glVertex3f(xdouble + crosshairSizeX, ydouble + thicknessY, 0);
   glVertex3f(xdouble + crosshairSizeX, ydouble - thicknessY, 0);
   glVertex3f(xdouble - crosshairSizeX, ydouble - thicknessY, 0);
   glVertex3f(xdouble + thicknessX, ydouble - crosshairSizeY, 0);
   glVertex3f(xdouble + thicknessX, ydouble + crosshairSizeY, 0);
   glVertex3f(xdouble - thicknessX, ydouble + crosshairSizeY, 0);
   glVertex3f(xdouble - thicknessX, ydouble - crosshairSizeY, 0);
   glEnd();
   usePerspective();
   glPopMatrix();
}

void display() {
  double startTime = doubleTime();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  
  glPushMatrix();
  gameState->draw();

  drawGrid();
  drawSprites();
  drawCrosshair();

  glPopMatrix();

  glutSwapBuffers();
  displayTime += doubleTime() - startTime;
  ++curFrame;
}

void keyboard(unsigned char key, int x, int y ) {
  switch( key ) {
    case 'a': gameState->ship->startYaw(1); break;
    case 'd': gameState->ship->startYaw(-1); break;
    case 'w': gameState->ship->forwardAcceleration(10); break;
    case 's': gameState->ship->forwardAcceleration(-10); break;
    case 'q': gameState->ship->rightAcceleration(-10); break;
    case 'e': gameState->ship->rightAcceleration(10); break;
    case ' ': gameState->ship->upAcceleration(10); break;
    case 'c': gameState->ship->upAcceleration(-10); break;
    case 'b': gameState->ship->brake(2); break;
  }
}

void keyUp(unsigned char key, int x, int y) {
   switch (key) {
      case 'a':
      case 'd': gameState->ship->startYaw(0); break;
      case 'w':
      case 's': gameState->ship->forwardAcceleration(0); break;
      case 'q': 
      case 'e': gameState->ship->rightAcceleration(0); break;
      case ' ': 
      case 'c': gameState->ship->upAcceleration(0); break;
      case 'b': gameState->ship->brake(0); break;
   }
}

void passiveMouse(int x, int y) {
   xdouble = p2wx(x);
   ydouble = p2wy(y);
   const double angleScale = 1;
   rollAmount = clamp(xdouble * fabs(xdouble) * angleScale, -1, 1);
   pitchAmount = clamp(-ydouble * fabs(ydouble) * angleScale, -1, 1);
}

void mouseMove(int x, int y) {
   passiveMouse(x, y);
   gameState->ship->updateShotDirection(p2wx(x), p2wy(y));
}

void timerFunc() {
   static unsigned long lastSecond = 0;
   static unsigned int frames = 0;
   static double lastTime = 0;
   static double totalTime = 0;
   static Point3D lastShipPosition(0, 0, 0);
   double startTime;
   double curTime = doubleTime();
   if (lastTime == 0) {
      lastTime = curTime;
      return;
   }
   double timeDiff = curTime - lastTime;
   gameState->curFPS = 1/timeDiff;
   ++frames;
   totalTime += timeDiff;

   gameState->ship->updatePosition(timeDiff, rollAmount, pitchAmount);
   gameState->ship->keepFiring();
   
   gameState->update(timeDiff);

   gameState->checkCollisions();

   glutPostRedisplay();
   
   lastTime = curTime;
   // Stats
   if (floor(curTime) > lastSecond) {
      frames = 0;
      lastSecond = floor(curTime);
   }
}

void mouse(int button, int state, int x, int y) {
   if (button == GLUT_LEFT_BUTTON) {
      if (state == GLUT_DOWN)
         gameState->ship->fireLasers(p2wx(x), p2wy(y), 0);
      else
         gameState->ship->stopLasers(0);
   }
   
   if (button == GLUT_RIGHT_BUTTON) {
      if (state == GLUT_DOWN)
         gameState->ship->fireLasers(p2wx(x), p2wy(y), 1);
      else
         gameState->ship->stopLasers(1);
   }
}

int main(int argc, char* argv[]) {

   srand(time(NULL));
  
  //set up my window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   GW = 800;
   GH = 600;
  glutInitWindowSize(GW, GH); 
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Asteroid Blaster");
  glClearColor(0.0, 0.0, 0.0, 1.0);
  startx = starty = 0;
  
  
  //register glut callback functions
  glutDisplayFunc( display );
  glutReshapeFunc( reshape );
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyUp);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMove);
  glutPassiveMotionFunc(passiveMouse);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

   
  glEnable(GL_LIGHTING);
  glEnable(GL_BLEND);
  
  init_light();
  init_tex();
  glutSetCursor(GLUT_CURSOR_NONE);

  // Preload texture.
  new TextureImporter("Images/SkybusterExplosion.bmp");
  //glutFullScreen();

  materials(Rock);
  quadric = gluNewQuadric();
  gluQuadricNormals(quadric, GLU_SMOOTH);
  glutIdleFunc(timerFunc);

  gameState = new GameState(WORLD_SIZE);
  
  glutMainLoop();
}
