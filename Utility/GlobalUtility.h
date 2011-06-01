/**
 * Sterling Hirsh
 * GlobalUtility.h
 * A collection of functions that make working with glut possible.
 * Note that global variables GW, GH, mouseX, and mouseY are defined here.
 */
#ifndef __GLOBALUTILITY_H__
#define __GLOBALUTILITY_H__

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <list>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include "SDL.h"
#include "SDL_ttf.h"

#include "Utility/Constants.h"
#include "Utility/GameSettings.h"

#include <math.h>
#include <algorithm>
#include <string>

// Forward declaration
class GameState;
class MainMenu;
class StoreMenu;
class SettingsMenu;
class HelpMenu;
class CreditsMenu;
class Input;
class InputManager;

// extern variables actually defined in .cpp
extern GameSettings* gameSettings;
extern int flopY(int);
extern int texSize;
extern unsigned long curFrame;
extern bool drawPerspective;
extern bool showBloomScreen;
extern bool cameraFollow;
extern bool running;
extern GLUquadricObj *quadric;
extern GLuint tractorBeamShader;
extern GLuint fadeShader;
extern GLuint elecShader;
extern GLuint lawnShader;
extern GLuint hBlurShader;
extern GLuint vBlurShader;
extern GLuint ramShader;
extern GLuint tractorFade;
extern GLuint billboardShader;
extern GLuint shipXShader;
extern GLuint shipYShader;
extern GLuint backShader;
extern GLuint hitShader;
extern GLuint explosionShader;
extern GLuint ringShader;
extern GLuint timeBombShader;
extern GLuint fbo;
extern GLuint depthbuffer;
extern SDL_Surface* gDrawSurface;
extern const SDL_VideoInfo* vidinfo;
extern InputManager* inputManager;
extern MainMenu* mainMenu;
extern StoreMenu* storeMenu;
extern HelpMenu* helpMenu;
extern SettingsMenu* settingsMenu;
extern CreditsMenu* creditsMenu;
extern Input* chat;
extern TTF_Font* hudFont;
extern TTF_Font* menuFont;
extern double currentTime;

//Used for server/client stuff
extern std::string ipAddress;
extern std::string portNumber;

extern int TRACTOR_WEAPON_INDEX;
extern int BLASTER_WEAPON_INDEX;
extern int RAILGUN_WEAPON_INDEX;
extern int ELECTRICITY_WEAPON_INDEX;
extern int TIMEDBOMBER_WEAPON_INDEX;
extern int REMOTEBOMBER_WEAPON_INDEX;
extern int ENERGY_WEAPON_INDEX;
extern int RAM_WEAPON_INDEX;
extern int HOMINGMISSILE_WEAPON_INDEX;

extern int NUMBER_OF_WEAPONS;

// Global functions used throughout the program
double p2wx(int);
double p2wy(int);
double p2wHeight(int);
double p2wWidth(int);
double p2ix(int);
double p2iy(int);
void reshape(GLsizei w, GLsizei h);
double randdouble();
GLuint setShaders(char * vert, char * frag, char * geom);
GLuint setShaders(char * vert, char * frag);
GLuint setShaders(char * vert);
char *textFileRead(char *fn);
void printShaderLog(GLuint obj);
void printProgramLog(GLuint obj);

int minutesRemaining(double secondsRemaining);
int secondsRemainder(double secondsRemaining);
void useOrtho();
void usePerspective();
void drawCylinder(double radius, double length);
void getBrightColor(double hue, float& r, float& g, float& b);
void setupVideo();
void drawScreenQuad(int tex);
void initFbo();
void fboBegin(int buffer);
void fboEnd();
void fboClear(int buffer);
void clearAllBuffers();
int nextPowerOfTwo(int num);
/**
 * Find the index which has the maximum value in an array of doubles.
 * length is the length of arr.
 */
int maxValuedIndexInArray(double arr[], int length);


// Used for defining a material
typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess[1];
  GLfloat emissive[4];
} materialStruct;


extern materialStruct GreenShiny;
extern materialStruct RedShiny;
extern materialStruct GreenTransparent;
extern materialStruct BlueShiny;
extern materialStruct RedFlat;
extern materialStruct RedTransparent;
extern materialStruct YellowFlat;
extern materialStruct YellowTransparent;
extern materialStruct WhiteTransparent;
extern materialStruct GrayTransparent;
extern materialStruct WhiteSolid;
extern materialStruct BlackSolid;
extern materialStruct OrangeSolid;
extern materialStruct CyanSolid;
extern materialStruct Rock;
extern materialStruct CrystalMaterial;

void setMaterial(materialStruct material);
void updateDoubleTime();
void toggleGrabMode();
void toggleFullScreen();

inline double randdouble() {
   return (double)rand() / (double) RAND_MAX;
}

inline double doubleTime() {
   return currentTime;
}

inline double clamp(double num, double minVal, double maxVal) {
   return (std::max)(minVal, (std::min)(num, maxVal));
}

inline double distance3D(double x1, double y1, double z1, double x2,
      double y2, double z2) {
   return sqrt(
         (x2 - x1) * (x2 - x1) +
         (y2 - y1) * (y2 - y1) +
         (z2 - z1) * (z2 - z1));
}

inline double distance3D(double x1, double y1, double z1) {
   return sqrt(x1 * x1 + y1 * y1 + z1 * z1);
}

inline double distance2D(double x1, double y1, double x2, double y2) {
   return sqrt(
         (x2 - x1) * (x2 - x1) +
         (y2 - y1) * (y2 - y1));
}

inline double distance2D(double x1, double y1) {
   return sqrt(x1 * x1 + y1 * y1);
}

struct Color {
   GLfloat r, g, b, a;
   Color(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a = 1) :
      r(_r), g(_g), b(_b), a(_a) {}
   inline void setColor() {
      glColor4f(r, g, b, a);
   }
   inline void setColorWithAlpha(GLfloat tempAlpha) {
      glColor4f(r, g, b, tempAlpha);
   }
};

#endif
