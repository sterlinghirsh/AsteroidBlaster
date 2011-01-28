/**
 * Asteroid Blaster -- IN 3D!
 * Graphics Team: Sterling Hirsh, Taylor Arnicar, Ryuho Kudo, Jake Juszak, Chris Brenton
 * AI Team: Taylor Arnicar, Mike Smith, Sean Ghiocel, Justin Kuehn
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
#include "Utility/KeyboardManager.h"

#include "SDL.h"

using namespace std;

#define WORLD_SIZE 80.0
bool running = true;

//SDL
SDL_Surface* gDrawSurface = NULL;

const double angleScale = 1;

double xdouble = 0, ydouble = 0;
// TODO: Come up with better names for these.
double rollAmount = 0, pitchAmount = 0, yawAmount = 0;
double startx, starty;
int TextureImporter::curTexID;
std::map<string, int> TextureImporter::texIDMap;
bool mouseXYaw = false;

int *fontsArr[] = {
   (int*)GLUT_BITMAP_8_BY_13,
   (int*)GLUT_BITMAP_9_BY_15,
   (int*)GLUT_BITMAP_TIMES_ROMAN_10,
   (int*)GLUT_BITMAP_TIMES_ROMAN_24,
   (int*)GLUT_BITMAP_HELVETICA_10,
   (int*)GLUT_BITMAP_HELVETICA_12,
   (int*)GLUT_BITMAP_HELVETICA_18,
};
int fontSpot = 0;

GameState* gameState = NULL;
KeyboardManager* keyboardManager = NULL;

double displayTime = 0;
// This double contains the FPS to be printed to the screen each frame.

GLfloat headlight_pos[4] = {WORLD_SIZE / 2, WORLD_SIZE / 2, WORLD_SIZE / 2, 1};
GLfloat headlight_amb[4] = {0.1, 0.1, 0.1, 1};
GLfloat headlight_diff[4] = {1, 1, 1, 1.0};
GLfloat headlight_spec[4] = {1, 1, 1, 1.0};

void init_light() {
   glEnable(GL_LIGHT0);
   // headlight_amb is defined in Asteroidship.h
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

void drawCrosshair() {
   double crosshairSizeX = 0.05;
   double crosshairSizeY = 0.05;
   double thicknessX = 0.01;
   double thicknessY = 0.01;
   glPushMatrix();
   glLoadIdentity();
   materials(WhiteSolid);
   glColor3f(1, 1, 1);
   useOrtho();
   glDisable(GL_LIGHTING);
   glBegin(GL_QUADS);
   glVertex3f(xdouble + crosshairSizeX, ydouble + thicknessY, 0);
   glVertex3f(xdouble - crosshairSizeX, ydouble + thicknessY, 0);
   glVertex3f(xdouble - crosshairSizeX, ydouble - thicknessY, 0);
   glVertex3f(xdouble + crosshairSizeX, ydouble - thicknessY, 0);
   
   glVertex3f(xdouble + thicknessX, ydouble - crosshairSizeY, 0);
   glVertex3f(xdouble + thicknessX, ydouble + crosshairSizeY, 0);
   glVertex3f(xdouble - thicknessX, ydouble + crosshairSizeY, 0);
   glVertex3f(xdouble - thicknessX, ydouble - crosshairSizeY, 0);
   glEnd();
   glEnable(GL_LIGHTING);
   usePerspective();
   glPopMatrix();
}

void display() {
  double startTime = doubleTime();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  
  glPushMatrix();
  gameState->draw();

  drawSprites();
  drawCrosshair();

  glPopMatrix();

  SDL_GL_SwapBuffers();
  displayTime += doubleTime() - startTime;
  ++curFrame;
}

void initSDL() {
   // init video system
   const SDL_VideoInfo* vidinfo;
   if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
     fprintf(stderr,"Failed to initialize SDL Video!\n");
     exit(1);
   }

   // tell system which funciton to process when exit() call is made
   atexit(SDL_Quit);

   // get optimal video settings
   vidinfo = SDL_GetVideoInfo();

   if(!vidinfo) {
     fprintf(stderr,"Coudn't get video information!\n%s\n", SDL_GetError());
     exit(1);
   }

   // set opengl attributes
   SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        5);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      5);
   SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       5);
#ifdef __APPLE__
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      32);
#else
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      16);
#endif
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,    1);

   // get a framebuffer
   gDrawSurface = SDL_SetVideoMode(GW, GH, vidinfo->vfmt->BitsPerPixel, SDL_OPENGL);

   if( !gDrawSurface )
   {
     fprintf(stderr,"Couldn't set video mode!\n%s\n", SDL_GetError());
     exit(1);
   }
   
   //set the timer
	SDL_Init(SDL_INIT_TIMER);
   //SDL_AddTimer(Uint32 (100), spawnGameObj, param);
   //SDL_AddTimer(Uint32 (10), gameObjStep, param);

   //disable the cursor
   SDL_ShowCursor(SDL_DISABLE);
   
   //set the title
   SDL_WM_SetCaption("Asteroid Blaster", 0);

   // set opengl viewport and perspective view
   //glViewport(0,0,width,height);
   //glMatrixMode(GL_PROJECTION);
   //glLoadIdentity();
   //gluPerspective( 120, 4.0f / 3.0f, .00001, 100);
   //glMatrixMode(GL_MODELVIEW);
   
   
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glEnable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_LIGHTING);
   glEnable(GL_BLEND);
   init_light();
   init_tex();

}


void timerFunc() {
   static unsigned long lastSecond = 0;
   static double lastTime = 0;
   
   double curTime = doubleTime();

   if (lastTime == 0) {
      lastTime = curTime;
      return;
   }

   double timeDiff = curTime - lastTime;
   gameState->setCurFPS(1 / timeDiff);

   // Use mouse x (rollAmount) for yaw if mouseXYaw is true.
   gameState->ship->setPitchSpeed(pitchAmount);
   gameState->ship->setRollSpeed(mouseXYaw ? 0 : rollAmount);
   //gameState->ship->setYawSpeed(mouseXYaw ? -rollAmount : 0);
   
   gameState->update(timeDiff);

   gameState->checkCollisions();
   
   lastTime = curTime;
   // Stats
   if (floor(curTime) > lastSecond) {
      lastSecond = floor(curTime);
   }
}

void mouseUpdate(int x, int y){
   xdouble = p2wx(x);
   ydouble = p2wy(y);
   gameState->ship->updateShotDirection(xdouble, ydouble);
   rollAmount = clamp(xdouble * fabs(xdouble) * angleScale, -1, 1);
   pitchAmount = clamp(-ydouble * fabs(ydouble) * angleScale, -1, 1);
}

void mouseButton(SDL_Event event){
   if(event.type == SDL_MOUSEBUTTONDOWN) {
      printf("Mouse button %d pressed at (%d,%d)\n",event.button.button, event.button.x, event.button.y);
      gameState->ship->updateShotDirection(p2wx(event.button.x), p2wy(event.button.y));
      if(event.button.button == 1){
         gameState->ship->selectWeapon(0);         
      } else if (event.button.button == 3) {
         gameState->ship->selectWeapon(1);
      }
      gameState->ship->fire(true);
   }
   if (event.type == SDL_MOUSEBUTTONUP) {
      printf("Mouse button %d up at (%d,%d)\n",event.button.button, event.button.x, event.button.y);
      gameState->ship->fire(false);
   }
}

int main(int argc, char* argv[]) {
   srand(time(NULL));
   GW = 800;
   GH = 600;
   startx = starty = 0;
   
   
   glutInit(&argc, argv);
   
   //initialize stuff related to GL/SDL
   initSDL();
   
   // Preload texture.
   new TextureImporter("Images/SkybusterExplosion.bmp");

   materials(Rock);
   quadric = gluNewQuadric();
   gluQuadricNormals(quadric, GLU_SMOOTH);

   gameState = new GameState(WORLD_SIZE);
   keyboardManager = new KeyboardManager(gameState);

   while (running) {
      if(gameState->isGameRunning()) {
         timerFunc();
         display();
      }
      keyboardManager->update();
      SDL_Event event;
      float mouseButtonDown = 0;
      while (SDL_PollEvent(&event)) {
         if (event.type == SDL_QUIT) {
           running = 0;
         }
         if (event.type == SDL_MOUSEMOTION) {
            mouseUpdate(event.button.x,event.button.y);
         }     
         if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
            mouseButton(event);
         }
      }
      
   }
   delete keyboardManager;
}
