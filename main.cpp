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
#include "Graphics/Skybox.h"
#include "Graphics/Sprite.h"
#include "Graphics/Camera.h"
#include "Particles/Particle.h"
#include "Items/BoundingSpace.h"
#include "Utility/Text.h"
#include "Utility/GameState.h"
#include "Utility/StoreMenu.h"
#include "Utility/InputManager.h"
#include "Utility/Matrix4.h"
#include "Utility/Music.h"
#include "Utility/SoundEffect.h"
#include "Utility/Texture.h"

#include "SDL.h"
#include "SDL_mixer.h"

using namespace std;

// Constant value
const double WORLD_SIZE = 80.00;

// True while the game is in play.
bool running = true;
bool fullScreen = false;


// the absolute time update was last called
static double lastUpdateTime = 0;
// the absolute time update was last called
static double lastDrawTime = 0;
   
// This double contains the FPS to be printed to the screen each frame.

// TODO: Move this out of here.
GLfloat headlight_pos[4] = {WORLD_SIZE / 2, WORLD_SIZE / 2, WORLD_SIZE / 2, 1};
GLfloat headlight_amb[4] = {0.1, 0.1, 0.1, 1};
GLfloat headlight_diff[4] = {1, 1, 1, 1.0};
GLfloat headlight_spec[4] = {1, 1, 1, 1.0};

GLfloat minimaplight_pos[4] = {0, 0, 0, 1};
GLfloat minimaplight_amb[4] = {1, 1, 1, 0.5};
GLfloat minimaplight_diff[4] = {1, 1, 1, 0.5};
GLfloat minimaplight_spec[4] = {1, 1, 1, 0.5};

GLfloat ambientlight_pos[4] = {0, 0, 0, 1};
GLfloat ambientlight_amb[4] = {1, 1, 1, 0.5};
GLfloat ambientlight_diff[4] = {0, 0, 0, 0.5};
GLfloat ambientlight_spec[4] = {0, 0, 0, 0.5};

void init() {
    /* Flags to pass to SDL_SetVideoMode */
    int videoFlags;

   // Initialize the SDL video/audio system
   if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO)<0) {
      std::cerr << "Failed to initialize SDL Video/Audio!" << std::endl;
      exit(1);
   }

   // Tell system which functions to process when exit() call is made
   // THIS IS COMMENTED OUT BECAUSE THE EXIT TIME INCREASES BY 3-4 SECONDS
   //atexit(SDL_Quit);

   // Get optimal video settings
   vidinfo = SDL_GetVideoInfo();

   if (!vidinfo) {
      std::cerr << "Couldn't get video settings information! " << SDL_GetError() << std::endl;
      exit(1);
   }
   
   if (TTF_Init()) {
      std::cerr << TTF_GetError() << std::endl;
      exit(1);
   }
   
   //allocate channels for sound effects
   //SoundEffect::numChannels = Mix_AllocateChannels(32);
   SoundEffect::numChannels = Mix_AllocateChannels(16);

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


   /* get the flags to pass to SDL_SetVideoMode */
   videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
   videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
   videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
   videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

   /* This checks to see if surfaces can be stored in memory */
   if ( vidinfo->hw_available ) {
      videoFlags |= SDL_HWSURFACE;
   } else {
      videoFlags |= SDL_SWSURFACE;
   }

   /* This checks if hardware blits can be done */
   if ( vidinfo->blit_hw ) {
      videoFlags |= SDL_HWACCEL;
   }

   /* Sets up OpenGL double buffering */
   SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

   // Get a surface
   gDrawSurface = SDL_SetVideoMode(GW, GH, vidinfo->vfmt->BitsPerPixel, videoFlags);

   if (!gDrawSurface) {
      fprintf(stderr, "Couldn't set video mode!\n%s\n", SDL_GetError());
      exit(1);
   }

   // Set the timer
   SDL_Init(SDL_INIT_TIMER);

   // Disable the cursor
   SDL_ShowCursor(SDL_DISABLE);

   // Grab the input
   // Disabled for debug purposes.
   SDL_WM_GrabInput(SDL_GRAB_OFF);

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



   //GL setting calls
   //This enables alpha transparency so that things are see through
   glEnable(GL_BLEND);
   
   //set the background to be black
   glClearColor(0.0, 0.0, 0.0, 1.0);
   //glClearColor(1.0, 1.0, 1.0, 1.0);

   //initialize some GL stuff
   //glEnable(GL_DEPTH_TEST);
   //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   
   
   //glEnable( GL_TEXTURE_2D );

   // enables lighting so that minimap's sphere can have color
   glEnable(GL_LIGHTING);

   glEnable(GL_LINE_SMOOTH);
   //glEnable(GL_MULTISAMPLE_ARB);
   glHint(GL_LINE_SMOOTH_HINT, GL_PERSPECTIVE_CORRECTION_HINT);
   //glEnable(GL_POLYGON_SMOOTH);

   //initialize light
   //glEnable(GL_LIGHT0);
   //glLightfv(GL_LIGHT0, GL_AMBIENT, headlight_amb);
   //glLightfv(GL_LIGHT0, GL_DIFFUSE, headlight_diff);
   //glLightfv(GL_LIGHT0, GL_SPECULAR, headlight_spec);
   //glLightfv(GL_LIGHT0, GL_POSITION, headlight_pos);


   glEnable(GL_LIGHT1);
   glLightfv(GL_LIGHT1, GL_AMBIENT, minimaplight_amb);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, minimaplight_diff);
   glLightfv(GL_LIGHT1, GL_SPECULAR, minimaplight_spec);
   glLightfv(GL_LIGHT1, GL_POSITION, minimaplight_pos);

   //glEnable(GL_LIGHT2);
   //glLightfv(GL_LIGHT2, GL_AMBIENT, ambientlight_amb);
   //glLightfv(GL_LIGHT2, GL_DIFFUSE, ambientlight_diff);
   //glLightfv(GL_LIGHT2, GL_SPECULAR, ambientlight_spec);
   //glLightfv(GL_LIGHT2, GL_POSITION, ambientlight_pos);
   //glShadeModel(GL_SMOOTH);
   //glEnable(GL_NORMALIZE);

   //initialize textures
   //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   //glBlendEquation(GL_FUNC_ADD);

}

void update() {
   // get the current absolute time
   double curTime = doubleTime();

   if (lastUpdateTime == 0) {
      lastUpdateTime = curTime;
      return;
   }

   // the time difference since last update call
   double timeDiff = curTime - lastUpdateTime;
   
   gameState->update(timeDiff);


   lastUpdateTime = curTime;
   ++curFrame;
}

void draw() {
   // get the current absolute time
   double curTime = doubleTime();

   // the time difference since last update call
   double timeDiff = curTime - lastDrawTime;
   
   gameState->setCurFPS(1 / timeDiff);
   
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   usePerspective();

   glMatrixMode(GL_MODELVIEW);


   // Draw glowing objects to a texture for the bloom effect.
   gameState->aspect = (float)GW/(float)GH;
   if (gameState->bloom) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glPushMatrix();
      gameState->drawGlow();
      glBindTexture(GL_TEXTURE_2D, gameState->hTexture);

      glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
            0,0, GW, GH, 0);
      glPopMatrix();

      if (gameState->bloom) {
         gameState->hBlur();
         gameState->vBlur();
      }
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   usePerspective();
   // Draw the main screen
   glPushMatrix();
   if (gameState->reg) {
      gameState->draw();
      //gameState->drawGlow();
   }
   Sprite::drawSprites();
   //Particle::drawParticles();
   //drawCrosshair();
   glPopMatrix();

   glPushMatrix();
   // Draw the hud
   glClear(GL_DEPTH_BUFFER_BIT);
   gameState->drawHud();
   if (gameState->bloom) {
      gameState->drawBloom();
   }
   /*
   if (inputManager->bloom || inputManager->bloom1) {
      //printf("SPECUBLOOM\n");
      gameState->drawBloom(inputManager->bloom, inputManager->bloom1);
   }
   */
   glPopMatrix();
   // Flush The GL Rendering Pipeline - this doesn't seem strictly necessary
   gameState->drawMinimap();

   SDL_GL_SwapBuffers();
   lastDrawTime = curTime;
}

int main(int argc, char* argv[]) {
   srand(time(NULL));
   GW = 800;
   GH = 600;
   updateDoubleTime();

   // Initialize GL/SDL/glew/GLSL related things
   init();

   
   //loading textures
   Texture::Add("Images/Logo.png", "Logo.png");
   Texture::Add("Images/StoreLogo.png", "StoreLogo");
   Texture::Add("Images/AsteroidExplosion.png", "AsteroidExplosion");
   Texture::Add("Images/particle.png", "Particle");
   Texture::Add("Images/starsdark.bmp", "starsdark.png");
   Texture::Add("Images/AsteroidSurface.png", "AsteroidSurface");

   Particle::initDisplayList();
   
   //load the shader files
   elecShader = setShaders( (char *) "./Shaders/elec.vert", (char *) "./Shaders/elec.frag");
   ramShader = setShaders( (char *) "./Shaders/ram.vert", (char *) "./Shaders/ram.frag");
   tractorBeamShader = setShaders( (char *) "./Shaders/toon.vert", (char *) "./Shaders/toon.frag");
   lawnShader = setShaders( (char *) "./Shaders/lawn.vert", (char *) "./Shaders/lawn.frag", (char *) "./Shaders/lawn.geom");
   fadeShader = setShaders( (char *) "./Shaders/fade.vert", (char *) "./Shaders/fade.frag");
   hBlurShader = setShaders( (char *) "./Shaders/gauss.vert", (char *) "./Shaders/hblur.frag");
   vBlurShader = setShaders( (char *) "./Shaders/gauss.vert", (char *) "./Shaders/vblur.frag");
   particleShader = setShaders( (char *) "./Shaders/particle.vert", (char *) "./Shaders/particle.frag");
   //hBlurShader = setShaders( (char *) "./Shaders/old/hblur.vert", (char *) "./Shaders/hblur.frag");
   //vBlurShader = setShaders( (char *) "./Shaders/old/vblur.vert", (char *) "./Shaders/vblur.frag");

   //load and start BGM
   Music::Add("Sounds/8-bit3.ogg","8-bit3.ogg");
   Music::Add("Sounds/Asteroids2.ogg", "Asteroids2.ogg");
   Music::Add("Sounds/Careless_Whisper.ogg","Careless_Whisper.ogg");
   Music::playMusic("8-bit3.ogg");
   //Music::playMusic("Asteroids2.ogg");

   //load sound effects
   //SoundEffect::Add("Sounds/blaster1.wav");
   SoundEffect::Add("Sounds/blaster2.wav", "blaster2.wav");
   SoundEffect::Add("Sounds/BlasterShot2.wav", "BlasterShot2.wav");
   //SoundEffect::Add("Sounds/railgun1.wav");
   SoundEffect::Add("Sounds/Rail2.wav", "Rail2.wav");
   SoundEffect::Add("Sounds/Explosion1.wav", "Explosion1.wav");
   SoundEffect::Add("Sounds/CrystalCollect.wav", "CrystalCollect.wav");
   SoundEffect::Add("Sounds/TractorBeam.wav", "TractorBeam.wav");
   SoundEffect::Add("Sounds/Pikachu.wav", "Pikachu.wav");
   SoundEffect::Add("Sounds/ShipEngine.wav", "ShipEngine.wav");
   SoundEffect::Add("Sounds/GameOver.wav", "GameOver.wav");
   SoundEffect::Add("Sounds/ShipHit.wav", "ShipHit.wav");
   SoundEffect::Add("Sounds/BlasterHit.wav", "BlasterHit.wav");

   //get the quadradic up
   quadric = gluNewQuadric();
   //set the quadradic up
   gluQuadricNormals(quadric, GLU_SMOOTH);
   gluQuadricTexture(quadric, GL_TRUE); // Create Texture Coords


   // Initialize the gameState
   gameState = new GameState(WORLD_SIZE);
   
   // Initialize the menus
   mainMenu = new MainMenu();
   storeMenu = new StoreMenu();
   creditsMenu = new CreditsMenu();
   //turn the menu on for the inial menu display
   mainMenu->menuActive = true;
   
   
   
   //Initialize the input manager
   inputManager = new InputManager();
   //Connect the input manager to the gameState
   inputManager->addReceiver(gameState);
   inputManager->addReceiver(mainMenu);
   inputManager->addReceiver(storeMenu);
   inputManager->addReceiver(creditsMenu);

   //declare the event that will be reused
   SDL_Event* event = new SDL_Event();

   while (running) {
      updateDoubleTime();
      if (mainMenu->menuActive) {
         SDL_ShowCursor(SDL_ENABLE);
         mainMenu->draw();
         //update();
         lastUpdateTime = doubleTime();
      } else if (storeMenu->menuActive) {
         SDL_ShowCursor(SDL_ENABLE);
         storeMenu->draw();
         //update();
         lastUpdateTime = doubleTime();
      } else if (creditsMenu->menuActive) {
         SDL_ShowCursor(SDL_ENABLE);
         creditsMenu->draw();
         //update();
         lastUpdateTime = doubleTime();
      } else if (gameState->isGameRunning()) {
         update();
         draw();
      }
      while (SDL_PollEvent(event)) {
         inputManager->update(*event);
      }
   }
   delete inputManager;
   return 0;
}
