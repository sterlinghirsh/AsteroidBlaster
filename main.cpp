/**
 * Asteroid Blaster -- IN 3D!
 * Graphics Team: Sterling Hirsh, Taylor Arnicar, Ryuho Kudo, Jake Juszak, Chris Brenton
 * AI Team: Taylor Arnicar, Mike Smith, Sean Ghiocel, Justin Kuehn
 * Final Project for CPE 476 - Winter & Spring 2011
 */
#include <math.h>
#include <list>
#include <sstream>

#include "Utility/GlobalUtility.h"
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
#include "Utility/Menu.h"
#include "Utility/InputManager.h"
#include "Utility/Matrix4.h"
#include "Utility/Music.h"



#include "SDL.h"
#include "SDL_mixer.h"

using namespace std;

// Constant value
const double WORLD_SIZE = 80.00;

// True while the game is in play.
bool running = true;
bool fullScreen = false;

// SDL globals
const SDL_VideoInfo* vidinfo = NULL;

int TextureImporter::curTexID;
std::map<string, int> TextureImporter::texIDMap;
double minimapSizeFactor = 0.5;

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
// Pointer to the global menu object.
Menu* menu = NULL;
// This handles all the input.
InputManager* inputManager = NULL;

double displayTime = 0;
// This double contains the FPS to be printed to the screen each frame.

// TODO: Move this out of here.
GLfloat headlight_pos[4] = {WORLD_SIZE / 2, WORLD_SIZE / 2, WORLD_SIZE / 2, 1};
GLfloat headlight_amb[4] = {0.1, 0.1, 0.1, 1};
GLfloat headlight_diff[4] = {1, 1, 1, 1.0};
GLfloat headlight_spec[4] = {1, 1, 1, 1.0};

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
      
      glViewport (GW * (1 - minimapSizeFactor), 0, GW * minimapSizeFactor,
       GH * minimapSizeFactor);
      glMatrixMode (GL_PROJECTION);      /* Select The Projection Matrix */
      glLoadIdentity ();                     /* Reset The Projection Matrix */

      // Keep our aspect ratio relative to the global width and height
      gluPerspective(VERT_FOV, (double)GW/GH, 0.5, 20);

      glMatrixMode (GL_MODELVIEW);      /* Select The Projection Matrix */
      glLoadIdentity ();                        /* Reset The Modelview Matrix */

      glClear (GL_DEPTH_BUFFER_BIT);      /* Clear Depth Buffer */

      setMaterial(WhiteSolid);

      // eye, lookAt, and up vectors
      gluLookAt(0, 2, 5, 0, 0, 0,  0, 1, 0);
      
      // Draw objects in the map.
      gameState->drawInMinimap();

   glPopMatrix();
   // Flush The GL Rendering Pipeline - this doesn't seem strictly necessary
   SDL_GL_SwapBuffers();
   displayTime += doubleTime() - startTime;
   ++curFrame;
}

void init() {
   // Initialize the SDL video/audio system
   if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO)<0) {
      std::cerr << "Failed to initialize SDL Video/Audio!" << std::endl;
      exit(1);
   }

   // Tell system which functions to process when exit() call is made
   atexit(SDL_Quit);

   // Get optimal video settings
   vidinfo = SDL_GetVideoInfo();

   if (!vidinfo) {
      std::cerr << "Couldn't get video settings information! " << SDL_GetError() << std::endl;
      exit(1);
   }

   
   // we play no samples, so deallocate the default 8 channels... 
   Mix_AllocateChannels(0);
   
   if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)<0) {
      std::cerr << "Mix_OpenAudio Failed!" << std::endl;
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

   //set the background to be black
   glClearColor(0.0, 0.0, 0.0, 1.0);
   
   //initialize some GL stuff
   glEnable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_LIGHTING);
   glEnable(GL_BLEND);
   glEnable( GL_TEXTURE_2D );


   //initialize light
   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_AMBIENT, headlight_amb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, headlight_diff);
   glLightfv(GL_LIGHT0, GL_SPECULAR, headlight_spec);
   glLightfv(GL_LIGHT0, GL_POSITION, headlight_pos);
   //glShadeModel(GL_SMOOTH);
   glEnable(GL_NORMALIZE);

   //initialize textures
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

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
   
   // Initialize GL/SDL/glew/GLSL related things
   init();


   // Load textures and shaders
   new TextureImporter("Images/SkybusterExplosion.bmp");
   // get particle texture
   Particle::texture = (new TextureImporter("Images/particle.bmp"))->texID;
   //load the shader files
   shader1 = setShaders( (char *) "./Shaders/toon.vert", (char *) "./Shaders/toon.frag", (char *) "./Shaders/toon.geom");

   
   Music::Add("Sounds/Mists_of_Time-4T.ogg");
   Music::playMusic(0);
   
   //get the quadradic up
   quadric = gluNewQuadric();
   //set the quadradic up
   gluQuadricNormals(quadric, GLU_SMOOTH);
   gluQuadricTexture(quadric, GL_TRUE); // Create Texture Coords


   //Initialize gameState
   gameState = new GameState(WORLD_SIZE);
   //initialize menu
   menu = new Menu();
   
   //Initialize the input manager   
   inputManager = new InputManager();
   //Connect the input manager to the gameState
   inputManager->addReceiver(gameState);
   inputManager->addReceiver(menu);
   
   //declare the event that will be reused
   SDL_Event event;
   
   while (running) {
      if (menu->isActive()) {
         menu->draw();
      } else if (gameState->isGameRunning()) {
         timerFunc();
         display();
      }
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
