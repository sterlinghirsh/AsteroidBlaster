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
#include "Utility/InputManager.h"

#include "SDL.h"

using namespace std;

//constant value
const double WORLD_SIZE = 80.00;


bool running = true;

//SDL
SDL_Surface* gDrawSurface = NULL;

int TextureImporter::curTexID;
std::map<string, int> TextureImporter::texIDMap;

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
InputManager* inputManager = NULL;

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
   glVertex3f(gameState->getMouseX() + crosshairSizeX, gameState->getMouseY() + thicknessY, 0);
   glVertex3f(gameState->getMouseX() - crosshairSizeX, gameState->getMouseY() + thicknessY, 0);
   glVertex3f(gameState->getMouseX() - crosshairSizeX, gameState->getMouseY() - thicknessY, 0);
   glVertex3f(gameState->getMouseX() + crosshairSizeX, gameState->getMouseY() - thicknessY, 0);
   
   glVertex3f(gameState->getMouseX() + thicknessX, gameState->getMouseY() - crosshairSizeY, 0);
   glVertex3f(gameState->getMouseX() + thicknessX, gameState->getMouseY() + crosshairSizeY, 0);
   glVertex3f(gameState->getMouseX() - thicknessX, gameState->getMouseY() + crosshairSizeY, 0);
   glVertex3f(gameState->getMouseX() - thicknessX, gameState->getMouseY() - crosshairSizeY, 0);
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
      fprintf(stderr, "Failed ti initialize SDL Video!\n");
      exit(1);
   }

   // tell system which functions to process when exit() call is made
   atexit(SDL_Quit);

   // get optimal video settings
   vidinfo = SDL_GetVideoInfo();

   if (!vidinfo) {
      fprintf(stderr, "Couldn't get video information!\n%s\n", SDL_GetError());
      exit(1);
   }

   // set opengl attributes
   SDL_GL_SetAttribute(SDL_GL_RED_SIZE,      5);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,    5);
   SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,     5);
#ifdef __APPLE__
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,    32);
#else
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,    16);
#endif
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,  1);

   // get a framebuffer
   gDrawSurface = SDL_SetVideoMode(GW, GH, vidinfo->vfmt->BitsPerPixel, SDL_OPENGL);

   if (!gDrawSurface) {
      fprintf(stderr, "Couldn't set video mode!\n%s\n", SDL_GetError());
      exit(1);
   }

   //set the timer
   SDL_Init(SDL_INIT_TIMER);

   //disable the cursor   
   SDL_ShowCursor(SDL_DISABLE);

   //set the title
   SDL_WM_SetCaption("Asteroid Blaster", 0);   

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

   gameState->update(timeDiff);

   gameState->checkCollisions();
   
   lastTime = curTime;
   
   // Stats
   if (floor(curTime) > lastSecond) {
      lastSecond = floor(curTime);
   }
}

int main(int argc, char* argv[]) {
   srand(time(NULL));
   GW = 800;
   GH = 600;
   
   glutInit(&argc, argv);
   
   //initialize stuff related to GL/SDL
   initSDL();
   
   // Preload texture.
   new TextureImporter("Images/SkybusterExplosion.bmp");

   materials(Rock);
   quadric = gluNewQuadric();
   gluQuadricNormals(quadric, GLU_SMOOTH);

   gameState = new GameState(WORLD_SIZE);
   inputManager = new InputManager();
   inputManager->addReceiver(gameState);

   while (running) {
      if(gameState->isGameRunning()) {
         timerFunc();
         display();
      }
      SDL_Event event;
      float mouseButtonDown = 0;
      while (SDL_PollEvent(&event)) {
         if (event.type == SDL_KEYDOWN &&  event.key.keysym.sym == SDLK_F1) {
            SDL_WM_ToggleFullScreen( gDrawSurface );
         }
         if (event.type == SDL_KEYDOWN &&  event.key.keysym.sym == SDLK_ESCAPE) {
            exit(0);
         }
         else{
            inputManager->update(event);
         }
      }
      
   }
   delete inputManager;
}
