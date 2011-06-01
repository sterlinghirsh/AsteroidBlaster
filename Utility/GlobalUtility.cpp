/**
 * Sterling Hirsh
 * GlobalUtility.cpp
 * Includes global variables and
 * helper functions globally accessable.
 */

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <cmath>
#endif

#include "GlobalUtility.h"

#ifndef WIN32
#include <sys/time.h>
#endif

#include "Utility/GameSettings.h"
#include "Utility/GameState.h"
#include "Graphics/Texture.h"

//#include "SDL_video.h"

//global variables
GameSettings* gameSettings;
//int texSize = nextPowerOfTwo(std::max(gameSettings->GW, gameSettings->GH));
int texSize = 512;
unsigned long curFrame = 0;
bool drawPerspective = true;
bool showBloomScreen = false;
bool cameraFollow = true;
bool running = true;
GLUquadricObj *quadric = NULL;
GLuint tractorBeamShader = 0;
GLuint fadeShader = 0;
GLuint elecShader = 0;
GLuint ramShader = 0;
GLuint hBlurShader = 0;
GLuint vBlurShader = 0;
GLuint lawnShader = 0;
GLuint tractorFade = 0;
GLuint billboardShader = 0;
GLuint shipXShader = 0;
GLuint shipYShader = 0;
GLuint backShader = 0;
GLuint hitShader = 0;
GLuint explosionShader = 0;
GLuint ringShader = 0;
GLuint timeBombShader = 0;
GLuint fbo = 0;
GLuint depthbuffer = 0;
SDL_Surface* gDrawSurface = NULL;
const SDL_VideoInfo* vidinfo = NULL;
double currentTime = -1;

// Inputmanager used by all the main classes
InputManager* inputManager = NULL;

// The menu classes
MainMenu* mainMenu = NULL;
StoreMenu* storeMenu = NULL;
SettingsMenu* settingsMenu = NULL;
HelpMenu* helpMenu = NULL;
CreditsMenu* creditsMenu = NULL;

// What?
Input* chat = NULL;

// Fonts used in this game
TTF_Font* hudFont = NULL;
TTF_Font* menuFont = NULL;

//Used for server/client stuff
std::string ipAddress = "";
std::string portNumber = "";

using namespace std;


int flopY(int yIn) {
   return (gameSettings->GH - 1) - yIn;
}

double p2wx(int xp) {
   double d = ((gameSettings->GW - 1.0) / 2.0);
   double c = ((1 - gameSettings->GW) * gameSettings->GH) / (-2.0 * gameSettings->GW);
   return (xp - d) / c;
}

double p2wy(int yp) {
   double e = ((gameSettings->GH - 1) / 2.0);
   double f = e;
   return (flopY(yp) - f) / e;
}

double p2wHeight(int heightPixels) {
   return p2wy(0) - p2wy(heightPixels);
}

double p2wWidth(int widthPixels) {
   return p2wx(widthPixels) - p2wx(0);
}

double p2ix(int xp) {
   return p2wx(xp) * gameSettings->GH / (double) gameSettings->GW;
}

double p2iy(int yp) {
   return p2wy(yp);
}

int w2px(double xw) {
   int answer = 0;
   double d = ((gameSettings->GW - 1.0) / 2.0);
   double c = ((1 - gameSettings->GW) * gameSettings->GH) / (-2.0 * gameSettings->GW);
#ifdef WIN32
   answer = (int)floor(((xw * c) + d+0.5));
#else
   answer = (int) round((xw * c) + d);
#endif

   return answer;
}

int w2py(double yw) {
   int answer = 0;
   double e = ((gameSettings->GH - 1) / 2.0);
   double f = e;

#ifdef WIN32
   answer = flopY((int)(floor((yw * e) + f)+0.5));
#else
   answer = flopY((int) round((yw * e) + f));
#endif

   return answer;
}

/* Maintain Aspect Ratio */
void reshape (GLsizei w, GLsizei h) {
   gameSettings->GW = w;
   gameSettings->GH = h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (drawPerspective) {
      gluPerspective(VERT_FOV, (double)w/h, 0.3, 200);
   } else {
      glOrtho((double)w/-h, (double) w/h, -1, 1, -1, 1);
   }
   glMatrixMode(GL_MODELVIEW);
   glViewport(0, 0, w, h);
}

void usePerspective() {
   drawPerspective = true;
   reshape(gameSettings->GW, gameSettings->GH);
}

void useOrtho() {
   drawPerspective = false;
   reshape(gameSettings->GW, gameSettings->GH);
}

void updateDoubleTime() {
   double answer = 0;
#ifdef WIN32
   SYSTEMTIME st;
   GetSystemTime(&st);
   answer = (double)(st.wSecond) + ((st.wMilliseconds) / 1000.0);
#else
   static struct timeval tp;
   gettimeofday(&tp, NULL);
   answer = (double)(tp.tv_sec) + ((double)(tp.tv_usec) / 1000000.0);
#endif
   currentTime = answer;

}

/**
 * Return how many minutes are left from this number of seconds.
 * If secondsRemaining is < 60, this will return 0.
 */
int minutesRemaining(double secondsRemaining) {
   return (int)secondsRemaining/60;
}

/**
 * The complementary function for minutesRemaining. This tells the remainder seconds
 * left, after the minutes remaining has been calculated.
 */
int secondsRemainder(double secondsRemaining) {
   return ((int)secondsRemaining)%60;
}

void drawCylinder(double radius, double length) {
   glPushMatrix();
   gluDisk(quadric, 0, radius, 32, 1);
   gluCylinder(quadric, radius, radius, length, 10, 2);
   glTranslatef(0, 0, (GLfloat) length);
   gluDisk(quadric, 0, radius, 32, 1);
   glPopMatrix();
}

//sets up a specific material
void setMaterial(materialStruct material) {
   glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, material.shininess);
   glMaterialfv(GL_FRONT, GL_EMISSION, material.emissive);
}

GLdouble objectM[4][4] = {
   {1.0, 0.0, 0.0, 0.0},
   {0.0, 1.0, 0.0, 0.0},
   {0.0, 0.0, 1.0, 0.0},
   {0.0, 0.0, 0.0, 1.0}
};

materialStruct WhiteTransparent = {
   {0.5, 0.5, 0.5, 0.5},
   {0.0, 0.0, 0.0, 0.0},
   {0.0, 0.0, 0.0, 0.0},
   {0.0},
   {0.0, 0.0, 0.0, 1.0}
};

materialStruct GrayTransparent = {
   {1.0f, 1.0f, 1.0f, 1.0f},
   {1.0f, 1.0f, 1.0f, 0.7f},
   {1.0f, 1.0f, 1.0f, 1.0f},
   {4.0f},
   {0.0f, 0.0f, 0.0f, 1.0f}
};

materialStruct WhiteSolid = {
   {1.0, 1.0, 1.0, 1.0},
   {1.0, 1.0, 1.0, 1.0},
   {1.0, 1.0, 1.0, 1.0},
   {1.0},
   {0.0, 0.0, 0.0, 0.0}
};

materialStruct BlackSolid = {
   {0.0, 0.0, 0.0, 1.0},
   {0.0, 0.0, 0.0, 1.0},
   {0.0, 0.0, 0.0, 1.0},
   {0.0},
   {0.0, 0.0, 0.0, 0.0}
};

materialStruct CyanSolid = {
   {0.0, 1.0, 1.0, 1.0},
   {0.0, 1.0, 1.0, 1.0},
   {0.2f, 1.0, 1.0, 1.0},
   {0.0},
   {0.0, 0.0, 0.0, 1.0}
};

materialStruct OrangeSolid = {
   {1.0, .27f, 0.0, 1.0},
   {1.0, .27f, 0.0, 1.0},
   {1.0, .27f, 0.0, 1.0},
   {0.0},
   {0.0, 0.0, 0.0, 1.0}
};

materialStruct GreenShiny = {
   {0.0, 0.3f, 0.0, 1.0},
   {0.0, 0.9f, 0.0, 1.0},
   {0.2f, 1.0, 0.2f, 1.0},
   {8.0},
   {0.0, 0.0, 0.0, 1.0}
};

materialStruct RedShiny = {
   {0.3f, 0.0, 0.0, 1.0},
   {0.9f, 0.0, 0.0, 1.0},
   {1.0, 0.2f, 0.2f, 1.0},
   {8.0},
   {0.0, 0.0, 0.0, 1.0}
};

materialStruct GreenTransparent = {
   {0.0, 0.3f, 0.0, 0.3f},
   {0.0, 0.9f, 0.0, 0.3f},
   {0.2f, 1.0, 0.2f, 0.3f},
   {8.0},
   {0.0, 0.0, 0.0, 1.0}
};


materialStruct BlueShiny = {
   {0.0, 0.0, 0.3f, 1.0},
   {0.0, 0.0, 0.9f, 1.0},
   {0.0, 0.0, 0.0, 1.0},
   {0.0},
   {0.0, 0.0, 0.0, 1.0}
};

materialStruct RedFlat = {
   {0.3f, 0.0, 0.0, 1.0},
   {0.9f, 0.0, 0.0, 1.0},
   {0.0, 0.0, 0.0, 1.0},
   {0.0},
   {0.0, 0.0, 0.0, 1.0}
};

materialStruct RedTransparent = {
   {0.3f, 0.0, 0.0, 0.6f},
   {0.9f, 0.0, 0.0, 0.6f},
   {0.0, 0.0, 0.0, 0.6f},
   {0.0},
   {0.0, 0.0, 0.0, 1.0}
};


materialStruct YellowFlat = {
   {0.3f, 0.3f, 0.0, 1.0},
   {0.9f, 0.9f, 0.0, 1.0},
   {0.0, 0.0, 0.0, 1.0},
   {0.0},
   {0.0, 0.0, 0.0, 1.0}
};

materialStruct YellowTransparent = {
   {0.3f, 0.3f, 0.0, 0.4f},
   {0.9f, 0.9f, 0.0, 0.4f},
   {0.0, 0.0, 0.0, 0.4f},
   {0.0},
   {0.0, 0.0, 0.0, 1.0}
};

materialStruct Rock = {
   {0.0, 0.0, 0.0, 1.0},
   {0.6f, 0.6f, 0.8f, 1.0},
   {0.2f, 0.2f, 0.2f, 1.0},
   {2.0f},
   {0.0, 0.0, 0.0, 1.0}
};

materialStruct CrystalMaterial = {
   {1, 1, 1, 1},
   {0.6f, 1.0, 0.6f, 1},
   {1.0, 1.0, 1.0, 1},
   {2.0f},
   {0.0, 0.0, 0.0, 1.0}
};




char *textFileRead(char *fn) {
   FILE *fp;
   char *content = NULL;

   long int count=0;

   if (fn != NULL) {
      fp = fopen(fn,"rt");

      if (fp != NULL) {

         fseek(fp, 0, SEEK_END);
         count = ftell(fp);
         rewind(fp);

         if (count > 0) {
            content = (char *)malloc(sizeof(char) * (count+1));
            count = fread(content,sizeof(char),count,fp);
            content[count] = '\0';
         }
         fclose(fp);
      }
   }

   if (content == NULL) {
      fprintf(stderr, "ERROR: could not load in file %s\n", fn);
      exit(1);
   }
   return content;
}

GLuint setShaders(char * vert, char * frag, char * geom) {
   GLuint v,f, g, pro;
   char *vs, *fs, *gs;

   v = glCreateShader(GL_VERTEX_SHADER);
   f = glCreateShader(GL_FRAGMENT_SHADER);
   g = glCreateShader(GL_GEOMETRY_SHADER_EXT);

   vs = textFileRead(vert);
   fs = textFileRead(frag);
   gs = textFileRead(geom);

   const char * vv = vs;
   const char * ff = fs;
   const char * gg = gs;

   glShaderSource(v, 1, &vv, NULL);
   glShaderSource(f, 1, &ff, NULL);
   glShaderSource(g, 1, &gg, NULL);

   // We had free(gs) commented, but I don't know why.
   free(vs); free(fs); free(gs);

   glCompileShader(v);
   glCompileShader(f);
   glCompileShader(g);

   //fprintf(stderr, "vertex\n");
   printShaderLog(v);

   //fprintf(stderr, "fragment\n");
   printShaderLog(f);

   //fprintf(stderr, "geometry\n");
   printShaderLog(g);

   pro = glCreateProgram();
   glAttachShader(pro,v);
   glAttachShader(pro,f);
   glAttachShader(pro,g);

   // geometry shader details
   // input: GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT
   // output: GL_POINTS, GL_LINE_STRIP, GL_TRIANGLE_STRIP

   glProgramParameteriEXT(pro,GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES);
   glProgramParameteriEXT(pro,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_LINE_STRIP);
   int temp;
   glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
   glProgramParameteriEXT(pro,GL_GEOMETRY_VERTICES_OUT_EXT,temp);

   glLinkProgram(pro);
   printProgramLog(pro);

   return(pro);
}

GLuint setShaders(char * vert, char * frag) {
   GLuint v,f, pro;
   char *vs, *fs;

   v = glCreateShader(GL_VERTEX_SHADER);
   f = glCreateShader(GL_FRAGMENT_SHADER);

   vs = textFileRead(vert);
   fs = textFileRead(frag);

   const char * vv = vs;
   const char * ff = fs;

   glShaderSource(v, 1, &vv, NULL);
   glShaderSource(f, 1, &ff, NULL);

   free(vs); free(fs);

   glCompileShader(v);
   glCompileShader(f);

   //fprintf(stderr, "vertex\n");
   printShaderLog(v);

   //fprintf(stderr, "fragment\n");
   printShaderLog(f);

   pro = glCreateProgram();
   glAttachShader(pro,v);
   glAttachShader(pro,f);

   glProgramParameteriEXT(pro,GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES);
   glProgramParameteriEXT(pro,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_LINE_STRIP);
   int temp;
   glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
   glProgramParameteriEXT(pro,GL_GEOMETRY_VERTICES_OUT_EXT,temp);

   glLinkProgram(pro);
   printProgramLog(pro);

   return(pro);
}

GLuint setShaders(char * vert) {
   GLuint v, pro;
   char *vs;

   v = glCreateShader(GL_VERTEX_SHADER);

   vs = textFileRead(vert);

   const char * vv = vs;

   glShaderSource(v, 1, &vv, NULL);

   free(vs);

   glCompileShader(v);

   //fprintf(stderr, "vertex\n");
   printShaderLog(v);

   pro = glCreateProgram();
   glAttachShader(pro,v);

   glProgramParameteriEXT(pro,GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES);
   glProgramParameteriEXT(pro,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_LINE_STRIP);
   int temp;
   glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
   glProgramParameteriEXT(pro,GL_GEOMETRY_VERTICES_OUT_EXT,temp);

   glLinkProgram(pro);
   printProgramLog(pro);

   return(pro);
}

void printShaderLog(GLuint obj) {
   GLint infoLogLength = 0;
   GLsizei charsWritten  = 0;
   GLchar *infoLog;

   glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infoLogLength);

   if (infoLogLength > 0){
      infoLog = (char *) malloc(infoLogLength);
      glGetShaderInfoLog(obj, infoLogLength, &charsWritten, infoLog);
      printf("%s\n",infoLog);
      free(infoLog);
   }
}

void printProgramLog(GLuint obj) {
   GLint infoLogLength = 0;
   GLsizei charsWritten  = 0;
   GLchar *infoLog;

   glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infoLogLength);

   if (infoLogLength > 0){
      infoLog = (char *) malloc(infoLogLength);
      glGetProgramInfoLog(obj, infoLogLength, &charsWritten, infoLog);
      printf("%s\n",infoLog);
      free(infoLog);
   }
}

void getBrightColor(double hue, float& r, float& g, float& b) {

   hue = fmod((6.0 * hue), 6.0);
   float colorValue = (float) fmod(hue, 1);
   if (hue < 1) {
      r = 1;
      g = colorValue;
      b = 0;
   } else if (hue < 2) {
      r = 1 - colorValue;
      g = 1;
      b = 0;
   } else if (hue < 3) {
      r = 0;
      g = 1;
      b = colorValue;
   } else if (hue < 4) {
      r = 0;
      g = 1 - colorValue;
      b = 1;
   } else if (hue < 5) {
      r = colorValue;
      g = 0;
      b = 1;
   } else {
      r = 1;
      g = 0;
      b = 1 - colorValue;
   }
}

void setupVideo() {
   /* Flags to pass to SDL_SetVideoMode */
   int videoFlags;

   SDL_FreeSurface(gDrawSurface);

   /* get the flags to pass to SDL_SetVideoMode */
   videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
   videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
   videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
   //videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

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

   if ( gameSettings->fullscreen ) {
      videoFlags |= SDL_FULLSCREEN;
   }

   // Get a surface
   gDrawSurface = SDL_SetVideoMode(gameSettings->GW, gameSettings->GH, vidinfo->vfmt->BitsPerPixel, videoFlags);

   if (!gDrawSurface) {
      fprintf(stderr, "Couldn't set video mode!\n%s\n", SDL_GetError());
      exit(1);
   }
}

void drawScreenQuad(int tex) {
   useOrtho();
   float aspect = (float)gameSettings->GW/(float)gameSettings->GH;
   glPushMatrix();
   glBlendFunc(GL_ONE, GL_ONE);
   glDisable(GL_LIGHTING);
   float minY = -1.0;
   float maxY = 1.0;
   float minX = -1.0f * aspect;
   float maxX = 1.0f * aspect;
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, tex);

   float texMaxX = (float) gameSettings->GW / (float) texSize;
   float texMaxY = (float) gameSettings->GH / (float) texSize;
   glColor4f(1.0, 1.0, 1.0, 0.5);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(minX, minY, 0.0);
   glTexCoord2f(texMaxX, 0.0);
   glVertex3f(maxX, minY, 0.0);
   glTexCoord2f(texMaxX, texMaxY);
   glVertex3f(maxX, maxY, 0.0);
   glTexCoord2f(0.0, texMaxY);
   glVertex3f(minX, maxY, 0.0);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glPopMatrix();

   glColor4f(1.0, 1.0, 1.0, 1.0);
   glBindTexture(GL_TEXTURE_2D, 0);
   glClear(GL_DEPTH_BUFFER_BIT);
}

void initFbo() {
   glGenFramebuffersEXT(1, &fbo);
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

   glGenRenderbuffersEXT(1, &depthbuffer);
   glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);
   glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
         texSize, texSize);
   glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
         GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer);

   int maxbuffers;
   glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxbuffers);
   if (maxbuffers < 4) {
      printf("maxbuffers (%d) less than needed buffers (4). Disabling overlay and bloom.\n", maxbuffers);
      gameSettings->useOverlay = false;
      gameSettings->bloom = false;
   }

   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
         GL_TEXTURE_2D, Texture::getTexture("fboTex"), 0);

   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
         GL_TEXTURE_2D, Texture::getTexture("hblurTex"), 0);

   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT,
         GL_TEXTURE_2D, Texture::getTexture("bloomTex"), 0);

   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT3_EXT,
         GL_TEXTURE_2D, Texture::getTexture("overlayTex"), 0);

   GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

   if (status == GL_FRAMEBUFFER_UNSUPPORTED_EXT) {
      printf("Framebuffers unsupported.\n");
      exit(EXIT_FAILURE);
   } else if (status == GL_FRAMEBUFFER_COMPLETE_EXT) {
      printf("Framebuffers supported.\n");
   }

   useOrtho();
   glDisable(GL_LIGHTING);

   float aspect = (float)gameSettings->GW/(float)gameSettings->GH;
   
   float minY = -1.0;
   float maxY = 1.0;
   float minX = -1.0f * aspect;
   float maxX = 1.0f * aspect;
   
   float texMaxX = (float) gameSettings->GW / (float) texSize;
   float texMaxY = (float) gameSettings->GH / (float) texSize;

   glColor4f(1.0, 1.0, 1.0, 1.0);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(minX, minY, 0.0);
   glTexCoord2f(texMaxX, 0.0);
   glVertex3f(maxX, minY, 0.0);
   glTexCoord2f(texMaxX, texMaxY);
   glVertex3f(maxX, maxY, 0.0);
   glTexCoord2f(0.0, texMaxY);
   glVertex3f(minX, maxY, 0.0);
   glEnd();

   glEnable(GL_LIGHTING);

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}

void fboBegin(int buffer) {
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
   glDrawBuffer(buffer);
}

void fboEnd() {
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void fboClear(int buffer) {
   fboBegin(buffer);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //glClear(GL_COLOR_BUFFER_BIT);
   //glClear(GL_DEPTH_BUFFER_BIT);
   fboEnd();
}

void clearAllBuffers() {
   /*
   fboClear(GLOW_BUFFER);
   fboClear(BLUR_BUFFER);
   fboClear(BLOOM_BUFFER);
   fboClear(DEPTH_BUFFER);
   //fboClear(GEOM_BUFFER);
   fboClear(NORMAL_BUFFER);
   fboClear(OVERLAY_BUFFER);
   */
}

int nextPowerOfTwo(int num) {
   int result = 1;
   while (result < num) {
      result *= 2;
   }
   printf("%d -> %d\n", num, result);
   //return result;
   return num;
}

void toggleFullScreen() {
   gameSettings->toggleFullScreen();
   setupVideo();
}

void toggleGrabMode() {
   // Toggle grab with the ~ key.
   if (SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON) {
      SDL_WM_GrabInput(SDL_GRAB_OFF);
   } else {
      SDL_WM_GrabInput(SDL_GRAB_ON);
   }
}

/**
 * Find the index which has the maximum value in an array of doubles.
 * length is the length of arr.
 */
int maxValuedIndexInArray(double arr[], int length) {
   double maxValue = arr[0];
   int maxIndex = 0;

   for(int i=1; i<length; i++) {
      if (arr[i] > maxValue) {
         maxValue = arr[i];
         maxIndex = i;
      }
   }

   return maxIndex;
}
