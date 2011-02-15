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
#include "Graphics/Particle.h"
#include "Graphics/Camera.h"
#include "Items/BoundingSpace.h"
#include "Utility/BitmapTextDisplay.h"
#include "Utility/GameState.h"
#include "Utility/InputManager.h"


#include "SDL.h"
#pragma comment(lib,"SDL.lib")
#pragma comment(lib,"SDLmain.lib") 

using namespace std;

// Constant value
const double WORLD_SIZE = 80.00;

// True while the game is in play.
bool running = true;
bool fullScreen = false;

// SDL globals
SDL_Surface* gDrawSurface = NULL;
const SDL_VideoInfo* vidinfo = NULL;

int TextureImporter::curTexID;
std::map<string, int> TextureImporter::texIDMap;

// Fonts
int *fontsArr[] = {
   (int*)GLUT_BITMAP_8_BY_13,
   (int*)GLUT_BITMAP_9_BY_15,
   (int*)GLUT_BITMAP_TIMES_ROMAN_10,
   (int*)GLUT_BITMAP_TIMES_ROMAN_24,
   (int*)GLUT_BITMAP_HELVETICA_10,
   (int*)GLUT_BITMAP_HELVETICA_12,
   (int*)GLUT_BITMAP_HELVETICA_18,
};
int fontSpot = 0; // TODO: What is this?

// Pointer to the global gamestate object.
GameState* gameState = NULL;
// This handles all the input.
InputManager* inputManager = NULL;

double displayTime = 0;
// This double contains the FPS to be printed to the screen each frame.

// TODO: Move this out of here.
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
  //glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);
}


void display() {
   double startTime = doubleTime();
   
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);

   // Draw the main screen
   glPushMatrix();
      gameState->draw();
      Sprite::drawSprites();
      Particle::drawParticles();
      //drawCrosshair();

   glPopMatrix();
   
   // Draw the minimap
   glPushMatrix();
      /* Set the viewport to 3/4 of the way across the screen on the bottom.
       * It should take up the bottom right corner of the screen.
       */
      glViewport (GW*0.66, 0, GW/3, GH/3);
      glMatrixMode (GL_PROJECTION);		/* Select The Projection Matrix */
      glLoadIdentity ();							/* Reset The Projection Matrix */

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

      // eye, lookAt, and up vectors
      gluLookAt(0, 2, 5, 0, 0, 0,  0, 1, 0);
      
      //glLoadIdentity ();								/* Reset The Modelview Matrix */
      
      glColor3f(0.1, 0.8, 0.1);
      // Draw a flat cylinder with radius 2 at top and bottom, 0.5 height, 2 slice, and 2 stack.
      glPushMatrix();
         glRotatef(-90.0,1.0f,0.0f,0.0f);	/* Rotate By 0 On The X-Axis */
         glDisable(GL_LIGHTING);
            gluDisk(gluNewQuadric(),1.8,1.9,35,35);
            gluDisk(gluNewQuadric(),2.1,2.2,35,35);
            gluDisk(gluNewQuadric(),2.4,2.5,35,35);
         glEnable(GL_LIGHTING);
      glPopMatrix();
      
      // Draw objects in the map.
      gameState->drawInMinimap();

      //glEnd();						/* Done Drawing The Textured Quad */	
      
   glPopMatrix();



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
   glEnable( GL_TEXTURE_2D );
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
   
   //initialize glut
   glutInit(&argc, argv);
   
   // Initialize stuff related to GL/SDL
   initSDL();
   
   // Preload texture.
   new TextureImporter("Images/SkybusterExplosion.bmp");
   // get particle texture
   Particle::texture = (new TextureImporter("Images/particle.bmp"))->texID;

   //setup glew and GLSL
#ifdef __APPLE__
#else
   glewInit();
   if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && 
      GL_EXT_geometry_shader4)
      printf("Ready for GLSL\n");
   else {
      printf("Not enough GLSL support\n");
      exit(1);
   }

#endif

   
   //load the shader files
   shader1 = setShaders( (char *) "./Shaders/toon.vert", (char *) "./Shaders/toon.frag", (char *) "./Shaders/toon.geom");

   //set the quadradic up
   quadric = gluNewQuadric();
   gluQuadricNormals(quadric, GLU_SMOOTH);


   //Initialize gameState
   gameState = new GameState(WORLD_SIZE);
   //Initialize the input manager
   inputManager = new InputManager();
   //Connect the input manager to the gameState
   inputManager->addReceiver(gameState);
   SDL_Event event;
   
   while (running) {
      if (gameState->getMenuMode()) {
         gameState->menuFunc();
      } else if (gameState->isGameRunning()) {
         timerFunc();
         display();
      }
      
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
   return 0;
}
