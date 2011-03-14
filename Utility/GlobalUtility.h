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
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include "SDL.h"
#include "Utility/InputManager.h"
#include "Menus/MainMenu.h"
#include "Menus/StoreMenu.h"
#include "Menus/CreditsMenu.h"

#define VERT_FOV 50.0

class GameState;

// the window width and window height
extern int GW, GH;
extern int flopY(int);
extern unsigned long curFrame;
extern bool drawPerspective;
extern GLUquadricObj *quadric;
extern GLuint tractorBeamShader;
extern GLuint fadeShader;
extern GLuint elecShader;
extern GLuint lawnShader;
extern GLuint hBlurShader;
extern GLuint vBlurShader;
extern GLuint ramShader;
extern GLuint particleShader;
extern SDL_Surface* gDrawSurface;
extern const SDL_VideoInfo* vidinfo;
extern InputManager* inputManager;
extern MainMenu* mainMenu;
extern StoreMenu* storeMenu;
extern CreditsMenu* creditsMenu;
extern GameState* gameState;




double p2wx(int);
double p2wy(int);
double p2wHeight(int);
double p2wWidth(int);
double p2ix(int);
double p2iy(int);
void reshape(GLsizei w, GLsizei h);
double distance3D(double x1, double y1, double z1, double x2 = 0,
 double y2 = 0, double z2 = 0);
double distance2D(double x1, double y1, double x2 = 0, double y2 = 0);
double clamp(double, double, double);
double randdouble();
GLuint setShaders(char * vert, char * frag, char * geom);
GLuint setShaders(char * vert, char * frag);
GLuint setShaders(char * vert);
char *textFileRead(char *fn);
void printShaderLog(GLuint obj);
void printProgramLog(GLuint obj);
double doubleTime();
void useOrtho();
void usePerspective();
void drawCylinder(double radius, double length);
void getBrightColor(double hue, float& r, float& g, float& b);

// Used for defining a material
typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess[1];
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

void setMaterial(materialStruct material);
void updateDoubleTime();
void toggleGrabMode();

struct Color {
   GLfloat r, g, b, a;
   Color(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a = 1) :
      r(_r), g(_g), b(_b), a(_a) {}
   void setColor() {
      glColor4f(r, g, b, a);
   }
   void setColorWithAlpha(GLfloat tempAlpha) {
      glColor4f(r, g, b, tempAlpha);
   }
};


#endif
