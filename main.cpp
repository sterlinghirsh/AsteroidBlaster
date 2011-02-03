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

// Constant value
const double WORLD_SIZE = 80.00;

bool running = true;
bool fullScreen = false;

// SDL
SDL_Surface* gDrawSurface = NULL;
const SDL_VideoInfo* vidinfo = NULL;

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

// Give the gameState (used for Radar)
GameState* getGameState() {
   return gameState;
}

void init_light() {
   glEnable(GL_LIGHT0);
   // headlight_amb is defined in Asteroidship.h
   glLightfv(GL_LIGHT0, GL_AMBIENT, headlight_amb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, headlight_diff);
   glLightfv(GL_LIGHT0, GL_SPECULAR, headlight_spec);
   glLightfv(GL_LIGHT0, GL_POSITION, headlight_pos);
  //glShadeModel(GL_SMOOTH);
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
      
      // Make it white
      materials(WhiteSolid);
      glColor3f(1, 1, 1);
      useOrtho();
      glDisable(GL_LIGHTING);
      glBegin(GL_QUADS);
      glVertex3f(gameState->ship->getAimX() + crosshairSizeX, gameState->ship->getAimY() + thicknessY, 0);
      glVertex3f(gameState->ship->getAimX() - crosshairSizeX, gameState->ship->getAimY() + thicknessY, 0);
      glVertex3f(gameState->ship->getAimX() - crosshairSizeX, gameState->ship->getAimY() - thicknessY, 0);
      glVertex3f(gameState->ship->getAimX() + crosshairSizeX, gameState->ship->getAimY() - thicknessY, 0);
      
      glVertex3f(gameState->ship->getAimX() + thicknessX, gameState->ship->getAimY() - crosshairSizeY, 0);
      glVertex3f(gameState->ship->getAimX() + thicknessX, gameState->ship->getAimY() + crosshairSizeY, 0);
      glVertex3f(gameState->ship->getAimX() - thicknessX, gameState->ship->getAimY() + crosshairSizeY, 0);
      glVertex3f(gameState->ship->getAimX() - thicknessX, gameState->ship->getAimY() - crosshairSizeY, 0);
      glEnd();
      glEnable(GL_LIGHTING);
      usePerspective();
   glPopMatrix();
}

void display() {
   double startTime = doubleTime();
   
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);

   // Loop to draw the main viewport and the minimap
   for (int loop = 0; loop < 2; loop++)
   {
      // Draw the main screen
      if(loop == 0)
      {
         glPushMatrix();
         gameState->draw();

         drawSprites();
         drawCrosshair();

         glPopMatrix();
      }
      
      // Draw the minimap
      if(loop == 1)
      {
         glPushMatrix();
            /* Set the viewport to 3/4 of the way across the screen on the bottom.
             * It should take up the bottom right corner of the screen.
             */
            glViewport (GW*0.66, 0, GW/3, GH/3);
            glMatrixMode (GL_PROJECTION);		/* Select The Projection Matrix */
            glLoadIdentity ();							/* Reset The Projection Matrix */
            /* Set Up Ortho Mode To Fit 1/4 The Screen (Size Of A Viewport) */
	            //gluOrtho2D(0, GW/2, GH/2, 0);
            //useOrtho();

            // Keep our aspect ratio relative to the global width and height
            gluPerspective(VERT_FOV, (double)GW/GH, 1, 200);

            glMatrixMode (GL_MODELVIEW);		/* Select The Projection Matrix */
            glLoadIdentity ();								/* Reset The Modelview Matrix */

            glClear (GL_DEPTH_BUFFER_BIT);		/* Clear Depth Buffer */

            GLUquadricObj *quadric;			/* The Quadric Object */
            quadric=gluNewQuadric();									/* Create A Pointer To The Quadric Object */
            gluQuadricNormals(quadric, GLU_SMOOTH);		/* Create Smooth Normals */
            gluQuadricTexture(quadric, GL_TRUE);			/* Create Texture Coords */

      materials(WhiteSolid);
//             glTranslatef(0.0f,0.0f,-5.0f);	/* Move 14 Units Into The Screen */

            glRotatef(0.0,1.0f,0.0f,0.0f);	/* Rotate By 0 On The X-Axis */
            glRotatef(0,0.0f,1.0f,0.0f);	/* Rotate By 0 On The Y-Axis */
            glRotatef(0,0.0f,0.0f,1.0f);	/* Rotate By 0 On The Z-Axis */

            // eye, lookAt, and up vectors
            gluLookAt(0, 2, 5, 0, 0, 0,  0, 1, 0);
            //glBegin(GL_QUADS);	/* Begin Drawing A Single Quad */
            /*glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);*/
                        //glDisable(GL_DEPTH_TEST);
            gluSphere(quadric,1.0f,16,16);	/* Draw A Sphere */
                        //glEnable(GL_DEPTH_TEST);
            
            //glLoadIdentity ();								/* Reset The Modelview Matrix */
            //glTranslatef(0.0f,0.5f,0.0f);
            
            glColor3f(0.1, 0.8, 0.1);
            // Draw a flat cylinder with radius 2 at top and bottom, 0.5 height, 2 slice, and 2 stack.
            //gluCylinder(gluNewQuadric(),2.5,3,0.4,8,8);
            glPushMatrix();
               glRotatef(-90.0,1.0f,0.0f,0.0f);	/* Rotate By 0 On The X-Axis */
               glDisable(GL_LIGHTING);
                  gluDisk(gluNewQuadric(),1.8,1.9,35,35);
                  gluDisk(gluNewQuadric(),2.1,2.2,35,35);
                  gluDisk(gluNewQuadric(),2.4,2.5,35,35);
               glEnable(GL_LIGHTING);
            glPopMatrix();


            //glEnd();						/* Done Drawing The Textured Quad */	
			
         glPopMatrix();
      }
   }

   // Flush The GL Rendering Pipeline - this doesn't seem strictly necessary
	// glFlush ();
	
   SDL_GL_SwapBuffers();
   displayTime += doubleTime() - startTime;
   ++curFrame;
}

void initSDL() {
   // Initialize the SDL video system
   if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr, "Failed to initialize SDL Video!\n");
      exit(1);
   }

   // Tell system which functions to process when exit() call is made
   atexit(SDL_Quit);

   // Get optimal video settings
   vidinfo = SDL_GetVideoInfo();

   if (!vidinfo) {
      fprintf(stderr, "Couldn't get video settings information!\n%s\n", SDL_GetError());
      exit(1);
   }

   // Set opengl attributes
   SDL_GL_SetAttribute(SDL_GL_RED_SIZE,      5);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,    5);
   SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,     5);
#ifdef __APPLE__
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,    32);
#else
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,    16);
#endif
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,  1);

   // Get a framebuffer
   gDrawSurface = SDL_SetVideoMode(GW, GH, vidinfo->vfmt->BitsPerPixel, SDL_OPENGL);

   if (!gDrawSurface) {
      fprintf(stderr, "Couldn't set video mode!\n%s\n", SDL_GetError());
      exit(1);
   }
   
   // Try to create a child window (for the minimap)
   //SDL_Window SDL_OpenChildWindow(gDrawSurface, 10, 10, 50, 50)

   // Set the timer
   SDL_Init(SDL_INIT_TIMER);

   // Disable the cursor   
   SDL_ShowCursor(SDL_DISABLE);

   // Set the title
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
   
   // Get the time
   double curTime = doubleTime();

   if (lastTime == 0) {
      lastTime = curTime;
      return;
   }

   double timeDiff = curTime - lastTime;
   gameState->setCurFPS(1 / timeDiff);

   gameState->update(timeDiff);

   // Check for all collisions
   gameState->checkCollisions();
   
   lastTime = curTime;
   
   // Timing statistics
   if (floor(curTime) > lastSecond) {
      lastSecond = floor(curTime);
   }
}

int main(int argc, char* argv[]) {
   srand(time(NULL));
   GW = 800;
   GH = 600;
   // Used for toggle full screens
   int oldGW = 800;
   int oldGH = 600;
   
   glutInit(&argc, argv);
   
   // Initialize stuff related to GL/SDL
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
      // This causes a valgrind error because event isn't initialized (I think).
      while (SDL_PollEvent(&event)) {
         if (event.type == SDL_KEYDOWN &&  event.key.keysym.sym == SDLK_F1) {
            if(!fullScreen) {
               oldGW = GW;
               oldGH = GH;
               GW = 1280;
               GH = 1024;
               gDrawSurface = SDL_SetVideoMode(GW, GH, vidinfo->vfmt->BitsPerPixel, SDL_OPENGL);
               SDL_WM_ToggleFullScreen( gDrawSurface );
               fullScreen = !fullScreen;
            }
            else {
               GW = oldGW;
               GH = oldGH;
               SDL_WM_ToggleFullScreen( gDrawSurface );
               gDrawSurface = SDL_SetVideoMode(GW, GH, vidinfo->vfmt->BitsPerPixel, SDL_OPENGL);
               fullScreen = !fullScreen;
            }
         }
         else {
            inputManager->update(event);
         }
      }
      
   }
   delete inputManager;
}
