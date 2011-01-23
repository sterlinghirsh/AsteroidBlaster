/**
 * Asteroids -- IN 3D!
 * Sterling Hirsh
 * Final Project for CPE 471 Fall 2010
 */
#include <math.h>
#include <list>

#include "Graphics/GlutUtility.h"
#include "Graphics/Face3D.h"
#include "Utility/Point3D.h"
#include "Items/Asteroid3D.h"
#include "Items/AsteroidShip.h"
#include "Graphics/TextureImporter.h"
#include "Graphics/Skybox.h"
#include "Graphics/Sprite.h"
#include "Graphics/Camera.h"
#include "Items/BoundingSpace.h"

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
list<Sprite*> Sprite::sprites;
std::map<string, int> TextureImporter::texIDMap;

list<Asteroid3D*> asteroids;

AsteroidShip *ship = NULL;

Skybox* skybox = NULL;
Camera* camera = NULL;
BoundingSpace* cube = NULL;

double displayTime = 0;

void init_light() {
   glEnable(GL_LIGHT0);
   // headlight_amb is defined in Asteroidship->h
   glLightfv(GL_LIGHT0, GL_AMBIENT, headlight_amb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, headlight_diff);
   glLightfv(GL_LIGHT0, GL_SPECULAR, headlight_spec);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);
}

void drawAsteroids() {
   materials(Rock);
   list<Asteroid3D*>::iterator asteroid = asteroids.begin();
   for (asteroid = asteroids.begin(); asteroid != asteroids.end(); asteroid++) {
      (*asteroid)->draw(true);
   }
}

void drawSprites() {
   list<Sprite*>::iterator sprite = Sprite::sprites.begin();
   for (; sprite != Sprite::sprites.end(); sprite++) {
      if (!(*sprite)->draw(ship->position)) {
         sprite = Sprite::sprites.erase(sprite);
         continue;
      }
   }
}

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
  skybox->draw(camera);

  camera->setCamera(true);
  ship->draw();
  drawAsteroids();
  drawGrid();
  drawSprites();
  drawCrosshair();

  glPopMatrix();

  glutSwapBuffers();
  displayTime += doubleTime() - startTime;
  ++curFrame;
}

void checkCollisions() {
   ship->checkAsteroidCollisions(asteroids);
}

void keyboard(unsigned char key, int x, int y ) {
  switch( key ) {
    case 'a': ship->startYaw(1); break;
    case 'd': ship->startYaw(-1); break;
    case 'w': ship->forwardAcceleration(10); break;
    case 's': ship->forwardAcceleration(-10); break;
    case 'q': ship->rightAcceleration(-10); break;
    case 'e': ship->rightAcceleration(10); break;
    case ' ': ship->upAcceleration(10); break;
    case 'c': ship->upAcceleration(-10); break;
    case 'b': ship->brake(2); break;
  }

  glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y) {
   switch (key) {
      case 'a':
      case 'd': ship->startYaw(0); break;
      case 'w':
      case 's': ship->forwardAcceleration(0); break;
      case 'q': 
      case 'e': ship->rightAcceleration(0); break;
      case ' ': 
      case 'c': ship->upAcceleration(0); break;
      case 'b': ship->brake(0); break;
   }
   glutPostRedisplay();
}

void passiveMouse(int x, int y) {
   xdouble = p2wx(x);
   ydouble = p2wy(y);
   double angleScale = 1;
   rollAmount = clamp(xdouble * fabs(xdouble) * angleScale, -1, 1);
   pitchAmount = clamp(-ydouble * fabs(ydouble) * angleScale, -1, 1);
}

void mouseMove(int x, int y) {
   passiveMouse(x, y);
   ship->updateShotDirection(p2wx(x), p2wy(y));
}

void timerFunc() {
   static unsigned long lastSecond = 0;
   static unsigned int frames = 0;
   static double lastTime = 0;
   static double firingTime = 0;
   static double collisionDetectionTime = 0;
   static double asteroidUpdateTime = 0;
   static double updatePositionTime = 0;
   static double totalTime = 0;
   static Point3D lastShipPosition(0, 0, 0);
   double startTime;
   double curTime = doubleTime();
   if (lastTime == 0) {
      lastTime = curTime;
      return;
   }
   double timeDiff = curTime - lastTime;
   //printf("curTime: %f, lastTime: %f, timeDiff: %f\n", curTime, lastTime, timeDiff);
   ++frames;
   totalTime += timeDiff;

   startTime = doubleTime();
   ship->updatePosition(timeDiff, rollAmount, pitchAmount);
   updatePositionTime += doubleTime() - startTime;
   startTime = doubleTime();
   ship->keepFiring();
   firingTime += doubleTime() - startTime;
   
   startTime = doubleTime();
   list<Asteroid3D*>::iterator asteroid = asteroids.begin();
   for (asteroid = asteroids.begin(); asteroid != asteroids.end(); ++asteroid)
      (*asteroid)->updatePosition(timeDiff);

   cube->constrain(ship);
   asteroidUpdateTime += doubleTime() - startTime;
   
   startTime = doubleTime();
   checkCollisions();
   collisionDetectionTime += doubleTime() - startTime;

   glutPostRedisplay();
   
   lastTime = curTime;
   // Stats
   if (floor(curTime) > lastSecond) {
      /*printf("FPS: %u, updPos: %f, colDetect: %f, astUpd: %f, firing: %f, disp: %f, total: %f\n",
       frames,
       100 * updatePositionTime ,
       100 * collisionDetectionTime ,
       100 * asteroidUpdateTime ,
       100 * firingTime ,
       100 * displayTime,
       100 * totalTime);
       */

      Vector3D actualShipSpeed(lastShipPosition, *ship->position);
      /*printf("ActualShipSpeed: %f, NominalShipspeed: %f\n", actualShipSpeed.getLength(),
       ship->velocity.getLength());
       */
      // Copy
      lastShipPosition = *ship->position;

      frames = 0;
      updatePositionTime = collisionDetectionTime = asteroidUpdateTime = firingTime = displayTime = totalTime = 0;
      lastSecond = floor(curTime);
   }
}

void mouse(int button, int state, int x, int y) {
   if (button == GLUT_LEFT_BUTTON) {
      if (state == GLUT_DOWN)
         ship->fireLasers(p2wx(x), p2wy(y), 0);
      else
         ship->stopLasers(0);
   }
   
   if (button == GLUT_RIGHT_BUTTON) {
      if (state == GLUT_DOWN)
         ship->fireLasers(p2wx(x), p2wy(y), 1);
      else
         ship->stopLasers(1);
   }
   glutPostRedisplay();
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
  for (int i = 0; i < 15; ++i) {
    asteroids.push_back(new Asteroid3D(10 + (10 * randdouble()), WORLD_SIZE));
  }
  
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
   skybox = new Skybox("Images/stars.bmp");
  glutSetCursor(GLUT_CURSOR_NONE);

  // Preload texture.
  new TextureImporter("Images/SkybusterExplosion.bmp");
  //glutFullScreen();
  ship = new AsteroidShip(GL_LIGHT0, WORLD_SIZE);
  camera = new Camera(ship);
  cube = new BoundingSpace(WORLD_SIZE / 2, 0, 0, 0);

  materials(Rock);
  quadric = gluNewQuadric();
  gluQuadricNormals(quadric, GLU_SMOOTH);
  glutIdleFunc(timerFunc);
  

  
  glutMainLoop();
}
