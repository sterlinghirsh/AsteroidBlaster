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
Uint8* _keys;

using namespace std;

#define WORLD_SIZE 80.0
bool running = true;


const double angleScale = 1;

double xdouble = 0, ydouble = 0;
double rollAmount = 0, pitchAmount = 0;
double startx, starty;
int TextureImporter::curTexID;
std::map<string, int> TextureImporter::texIDMap;

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

  SDL_GL_SwapBuffers();
  displayTime += doubleTime() - startTime;
  ++curFrame;
}

GLboolean CheckKeys()
{
  bool moved = false;
  if (_keys[SDLK_ESCAPE])
  {
    return true;
  }
  
   if (_keys[SDLK_a]) {
      gameState->ship->startYaw(1);
   }
   else if(_keys[SDLK_d]){
      gameState->ship->startYaw(-1);
   }
   else{
      gameState->ship->startYaw(0);
   }
   
   
   if (_keys[SDLK_w]) {
      gameState->ship->forwardAcceleration(10);
   }
   else if(_keys[SDLK_s]){
      gameState->ship->forwardAcceleration(-10);
   }
   else{
      gameState->ship->forwardAcceleration(0);
   }

   if (_keys[SDLK_q]) {
      gameState->ship->rightAcceleration(-10);
   }
   else if(_keys[SDLK_e]){
      gameState->ship->rightAcceleration(10);
   }
   else{
      gameState->ship->rightAcceleration(0);
   }

   if (_keys[SDLK_SPACE]) {
      gameState->ship->upAcceleration(10);
   }
   else if(_keys[SDLK_c]) {
      gameState->ship->upAcceleration(-10);
   }
   else{
      gameState->ship->upAcceleration(0);
   }

   if (_keys[SDLK_b]) {
      gameState->ship->brake(2);
   }
   else{
      gameState->ship->brake(0);
   }
   
  return false;
}



void timerFunc() {
   static unsigned long lastSecond = 0;
   static unsigned int frames = 0;
   static double lastTime = 0;
   static double totalTime = 0;
   static Point3D lastShipPosition(0, 0, 0);
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
   
   lastTime = curTime;
   // Stats
   if (floor(curTime) > lastSecond) {
      frames = 0;
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
   if(event.type == SDL_MOUSEBUTTONDOWN)
   {
      printf("Mouse button %d pressed at (%d,%d)\n",event.button.button, event.button.x, event.button.y);
      if(event.button.button == 1){
         gameState->ship->fireLasers(p2wx(event.button.x), p2wy(event.button.y), 0);         
      }
      else if(event.button.button == 3){
         gameState->ship->fireLasers(p2wx(event.button.x), p2wy(event.button.y), 1);         
      }
   }
   if (event.type == SDL_MOUSEBUTTONUP)
   {
      printf("Mouse button %d up at (%d,%d)\n",event.button.button, event.button.x, event.button.y);
      gameState->ship->stopLasers(event.button.button);
      if(event.button.button == 1){
         gameState->ship->stopLasers(0);      
      }
      else if(event.button.button == 3){
         gameState->ship->stopLasers(1);
      }
   }
}

int main(int argc, char* argv[]) {

   srand(time(NULL));
   GW = 1024;
   GH = 768;
   //set up my window
   glutInit(&argc, argv);
   
   //SDL INITIALIZATIONS
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "Unable to initialize SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	if (SDL_SetVideoMode(GW, GH, 0, SDL_OPENGL) == NULL)
	{
		cout << "Unable to create OpenGL scene: " << SDL_GetError() << endl;
		exit(2);
	}

	SDL_Init(SDL_INIT_TIMER);
   //SDL_AddTimer(Uint32 (100), spawnGameObj, param);
   //SDL_AddTimer(Uint32 (10), gameObjStep, param);

   SDL_WM_SetCaption("Asteroid Blaster", 0);
  

   glClearColor(0.0, 0.0, 0.0, 1.0);
   startx = starty = 0;

   glEnable(GL_DEPTH_TEST);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_LIGHTING);
   glEnable(GL_BLEND);
   init_light();
   init_tex();
   SDL_ShowCursor(SDL_DISABLE);

   // Preload texture.
   new TextureImporter("Images/SkybusterExplosion.bmp");
   //glutFullScreen();

   materials(Rock);
   quadric = gluNewQuadric();
   gluQuadricNormals(quadric, GLU_SMOOTH);

   gameState = new GameState(WORLD_SIZE);

   while (running) {
      timerFunc();
      display();

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
      _keys = SDL_GetKeyState(NULL);
      if (CheckKeys()) {
         running = 0;
      }

   }
}
