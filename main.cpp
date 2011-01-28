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

#include "SDL.h"

#define KEY_DOWN(a) ((_keys && _prevKeys) && _keys[(a)] && !_prevKeys[(a)])
#define KEY_UP(a) ((_keys && _prevKeys) && !_keys[(a)] && _prevKeys[(a)])

Uint8* _keys = NULL;
Uint8* _prevKeys = NULL;

using namespace std;

#define WORLD_SIZE 80.0
bool running = true;


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

GLboolean CheckKeys() {
   if (!_keys || !_prevKeys)
      return false;
   if (_keys[SDLK_ESCAPE]) {
      exit(0);
   }
   
   if (KEY_UP(SDLK_r)) {
         return true;
   }

   // If the shift key is down, yaw instead of rolling on the mouse x.
   mouseXYaw = _keys[SDLK_LSHIFT] || _keys[SDLK_RSHIFT];
   
   if (_keys[SDLK_w]) {
      gameState->ship->accelerateForward(1);
   } else if (_keys[SDLK_s]) {
      gameState->ship->accelerateForward(-1);
   } else {
      gameState->ship->accelerateForward(0);
   }

   if (_keys[SDLK_a]) {
      gameState->ship->accelerateRight(-1);
   } else if (_keys[SDLK_d]) {
      gameState->ship->accelerateRight(1);
   } else {
      gameState->ship->accelerateRight(0);
   }

   if (_keys[SDLK_SPACE]) {
      gameState->ship->accelerateUp(1);
   } else if (_keys[SDLK_LCTRL]) {
      gameState->ship->accelerateUp(-1);
   } else {
      gameState->ship->accelerateUp(0);
   }

   if (_keys[SDLK_b]) {
      gameState->ship->setBrake(true);
   } else {
      gameState->ship->setBrake(false);
   }

   if (KEY_DOWN(SDLK_f)) {
      gameState->FPSText->setFont(fontsArr[fontSpot++]);
      if (fontSpot > 7)
         fontSpot = 0;
   }
   
   return false;
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
   gameState->ship->setYawSpeed(mouseXYaw ? -rollAmount : 0);
   
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
   int numKeys = 0;
   srand(time(NULL));
   GW = 800;
   GH = 600;
   //set up my window
   glutInit(&argc, argv);
   
   //SDL INITIALIZATIONS
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "Unable to initialize SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	if (SDL_SetVideoMode(GW, GH, 0, SDL_OPENGL) == NULL) {
		cout << "Unable to create OpenGL scene: " << SDL_GetError() << endl;
		exit(2);
	}

	SDL_Init(SDL_INIT_TIMER);
   //SDL_AddTimer(Uint32 (100), spawnGameObj, param);
   //SDL_AddTimer(Uint32 (10), gameObjStep, param);

   SDL_WM_SetCaption("Asteroid Blaster", 0);
  

   glClearColor(0.0, 0.0, 0.0, 1.0);
   startx = starty = 0;

   glEnable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_LIGHTING);
   glEnable(GL_BLEND);
   init_light();
   init_tex();
   SDL_ShowCursor(SDL_DISABLE);

   // Preload texture.
   new TextureImporter("Images/SkybusterExplosion.bmp");

   materials(Rock);
   quadric = gluNewQuadric();
   gluQuadricNormals(quadric, GLU_SMOOTH);

   gameState = new GameState(WORLD_SIZE);
   _keys = SDL_GetKeyState(&numKeys);
   _prevKeys = new Uint8[numKeys];

   while (running) {
      if(gameState->isGameRunning()) {
         timerFunc();
         display();
      }
      
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
      
      _keys = SDL_GetKeyState(&numKeys);
      if (CheckKeys()) {
         delete gameState;
         gameState = new GameState(WORLD_SIZE);
      }
      memcpy(_prevKeys, _keys, sizeof(Uint8) * numKeys);
   }
   delete _prevKeys;
}
