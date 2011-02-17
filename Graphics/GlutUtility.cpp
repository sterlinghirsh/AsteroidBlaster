/**
 * Sterling Hirsh
 * GlutUtility.c
 * Functions that aid with glut.
 */

#include <math.h>
#include <algorithm>

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <cmath>
#endif

#include "GlutUtility.h"

#ifndef WIN32
#include <sys/time.h>
#endif

int GW, GH, mouseX, mouseY;
unsigned long curFrame;
bool drawPerspective = true;
GLUquadricObj *quadric;
RGB myimage[64][64];
RGB* pixel;
GLuint shader1;

using namespace std;

int flopY(int yIn) {
   return (GH - 1) - yIn;
}

double p2wx(int xp) { 
   double d = ((GW - 1.0) / 2.0);
   double c = ((1 - GW) * GH) / (-2.0 * GW);
   return (xp - d) / c;
}

double p2wy(int yp) {
   double e = ((GH - 1) / 2.0);
   double f = e;
   return (flopY(yp) - f) / e;
}

double p2ix(int xp) {
   return p2wx(xp) * GH / (double) GW;
}

double p2iy(int yp) {
   return p2wy(yp);
}

int w2px(double xw) {
   int answer = 0;
   double d = ((GW - 1.0) / 2.0);
   double c = ((1 - GW) * GH) / (-2.0 * GW);
#ifdef WIN32
   answer = floor((xw * c) + d+0.5);
#else 
   answer = round((xw * c) + d);
#endif

   return answer;
}

int w2py(double yw) {
   int answer = 0;
   double e = ((GH - 1) / 2.0);
   double f = e;

#ifdef WIN32
   answer = flopY(floor((yw * e) + f)+0.5);
#else 
   answer = flopY(round((yw * e) + f));
#endif

   return answer;
}

/* Maintain Aspect Ratio */
void reshape (GLsizei w, GLsizei h) {
   GW = w;
   GH = h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (drawPerspective) {
      gluPerspective(VERT_FOV, (double)w/h, 1, 200);
   } else {
      glOrtho((double)w/-h, (double) w/h, -1, 1, -0.5, 0.5); 
   }
   glMatrixMode(GL_MODELVIEW);
   glViewport(0, 0, w, h);
}

void usePerspective() {
   drawPerspective = true;
   reshape(GW, GH);
}

void useOrtho() {
   drawPerspective = false;
   reshape(GW, GH);
}

double distance3D(double x1, double y1, double z1, double x2,
 double y2, double z2) {
   return sqrt(
    (x2 - x1) * (x2 - x1) + 
    (y2 - y1) * (y2 - y1) + 
    (z2 - z1) * (z2 - z1));
}

double distance2D(double x1, double y1, double x2, double y2) {
   return sqrt(
    (x2 - x1) * (x2 - x1) +
    (y2 - y1) * (y2 - y1));
}

double clamp(double num, double minVal, double maxVal) {
   return max(minVal, min(num, maxVal));
}

double randdouble() {
   return (double)rand() / (double) RAND_MAX;
}

double doubleTime() {
   double answer = 0;
#ifdef WIN32
   SYSTEMTIME st;
   GetSystemTime(&st);
   answer = (double)(st.wSecond) + ((st.wMilliseconds) / 1000.0);
#else 
   static struct timeval tp;
   gettimeofday(&tp, NULL);
   answer = (double)(tp.tv_sec) + ((tp.tv_usec) / 1000000.0);
#endif

   return answer;

}

void drawCylinder(double radius, double length) {
   glPushMatrix();
   gluDisk(quadric, 0, radius, 32, 1);
   gluCylinder(quadric, radius, radius, length, 10, 2);
   glTranslatef(0, 0, length);
   gluDisk(quadric, 0, radius, 32, 1);
   glPopMatrix();
}

//sets up a specific material
void materials(materialStruct materials) {
  glMaterialfv(GL_FRONT, GL_AMBIENT, materials.ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materials.diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materials.specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, materials.shininess);
}

//initialization for texture mapping
void init_tex() {
  //glBindTexture(GL_TEXTURE_2D, 0);
  //glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
  {0.0}
};

materialStruct WhiteSolid = {
  {1.0, 1.0, 1.0, 1.0},
  {1.0, 1.0, 1.0, 1.0},
  {1.0, 1.0, 1.0, 1.0},
  {1.0}
};

materialStruct BlackSolid = {
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0}
};

materialStruct CyanSolid = {
  {0.0, 1.0, 1.0, 1.0},
  {0.0, 1.0, 1.0, 1.0},
  {0.2, 1.0, 1.0, 1.0},
  {0.0}
};

materialStruct OrangeSolid = {
  {1.0, .27, 0.0, 1.0},
  {1.0, .27, 0.0, 1.0},
  {1.0, .27, 0.0, 1.0},
  {0.0}
};

materialStruct GreenShiny = {
  {0.0, 0.3, 0.0, 1.0},
  {0.0, 0.9, 0.0, 1.0},
  {0.2, 1.0, 0.2, 1.0},
  {8.0}
};

materialStruct GreenTransparent = {
  {0.0, 0.3, 0.0, 0.3},
  {0.0, 0.9, 0.0, 0.3},
  {0.2, 1.0, 0.2, 0.3},
  {8.0}
};


materialStruct BlueShiny = {
  {0.0, 0.0, 0.3, 1.0},
  {0.0, 0.0, 0.9, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0}
};

materialStruct RedFlat = {
  {0.3, 0.0, 0.0, 1.0},
  {0.9, 0.0, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0}
};

materialStruct RedTransparent = {
  {0.3, 0.0, 0.0, 0.6},
  {0.9, 0.0, 0.0, 0.6},
  {0.0, 0.0, 0.0, 0.6},
  {0.0}
};


materialStruct YellowFlat = {
  {0.3, 0.3, 0.0, 1.0},
  {0.9, 0.9, 0.0, 1.0},
  {0.0, 0.0, 0.0, 1.0},
  {0.0}
};

materialStruct YellowTransparent = {
  {0.3, 0.3, 0.0, 0.4},
  {0.9, 0.9, 0.0, 0.4},
  {0.0, 0.0, 0.0, 0.4},
  {0.0}
};

materialStruct Rock = {
   {0.0, 0.0, 0.0, 1.0},
   {0.6, 0.6, 0.8, 1.0},
   {.2, 0.2, 0.2, 1.0},
   {2}
};




char *textFileRead(char *fn) {
	FILE *fp;
	char *content = NULL;

	int count=0;

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

