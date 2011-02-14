#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <map>
#include <string.h>
#include <GL/glut.h>
#include "Graphics/GlutUtility.h"

using namespace std;

#define FLT_MIN 1.1754E-38F
#define FLT_MAX 1.1754E+38F

int GW;
int GH;


typedef struct dvector{
	float x, y, z, c;
} dvector;

dvector rotstart, rotend, cprod;
float rotx, roty, angle;

GLfloat light_pos[4] = {1.0, 2.0, 1.0, 1.0};
//light color (ambiant, diffuse and specular)
GLfloat light_amb[4] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_diff[4] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_spec[4] = {0.8, 0.8, 0.8, 1.0};


GLfloat objectM[4][4] = {
  {1.0, 0.0, 0.0, 0.0},
  {0.0, 1.0, 0.0, 0.0},
  {0.0, 0.0, 1.0, 0.0},
  {0.0, 0.0, 0.0, 1.0}
};

GLfloat *trackballM = (GLfloat *)objectM;

void init_lighting() {
  //turn on light0
  glEnable(GL_LIGHT0);
  //set up the diffuse, ambient and specular components for the light
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
  glShadeModel(GL_FLAT);
  glEnable(GL_NORMALIZE);
}

void pos_light() {
  //set the light's position
  glMatrixMode(GL_MODELVIEW);
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}

void setUpMatrix(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, trackballM);
	glPopMatrix();
}



void reshape( GLsizei w, GLsizei h )
{
   GW = w;
   GH = h;
   glViewport(0, 0, w, h);
   glLoadIdentity();
   gluPerspective(60.0, (float)GW/GH, 1, 50);
   //gluOrtho2D( -(float)w/h, (float)w/h, -1, 1);
}
void draw_ship(){
	materials(black);
	glColor3f(0, 1, 0);
	glBegin(GL_TRIANGLES);

	glVertex3f(0, 0, 0);
	glVertex3f(.2, .2, 1.3);
	glVertex3f(.15, 0, 1);
	
	glVertex3f(0, 0, 0);
	glVertex3f(.2, .2, 1.3);
	glVertex3f(0, .15, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(-.2, .2, 1.3);
	glVertex3f(0, .15, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(-.2, .2, 1.3);
	glVertex3f(-.15, 0, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(-.2, -.2, 1.3);
	glVertex3f(-.15, 0, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(-.2, -.2, 1.3);
	glVertex3f(0, -.15, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(.2, -.2, 1.3);
	glVertex3f(0, -.15, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(.2, -.2, 1.3);
	glVertex3f(.15, 0, 1);
	glPopMatrix();

	/* Back of Ship */
	materials(black);

	glVertex3f(.15, 0, 1);
	glVertex3f(.2, .2, 1.3);
	glVertex3f(0, .15, 1);

	glVertex3f(.15, 0, 1);
	glVertex3f(.2, -.2, 1.3);
	glVertex3f(0, -.15, 1);

	glVertex3f(-.15, 0, 1);
	glVertex3f(-.2, -.2, 1.3);
	glVertex3f(0, -.15, 1);

	glVertex3f(-.15, 0, 1);
	glVertex3f(-.2, .2, 1.3);
	glVertex3f(0, .15, 1);

	
	materials(black);

	glVertex3f(.15, 0, 1);
	glVertex3f(0, 0, 1.6);
	glVertex3f(0, .15, 1);

	glVertex3f(.15, 0, 1);
	glVertex3f(0, 0, 1.6);
	glVertex3f(0, -.15, 1);

	glVertex3f(-.15, 0, 1);
	glVertex3f(0, 0, 1.6);
	glVertex3f(0, -.15, 1);

	glVertex3f(-.15, 0, 1);
	glVertex3f(-0, 0, 1.6);
	glVertex3f(0, .15, 1);
	glEnd();

	/* Outline of Ship */
	glLineWidth(5.0);
	glBegin(GL_LINE_LOOP);
	materials(Orange);
	glVertex3f(.15, 0, 1);
	glVertex3f(.2, .2, 1.3);
	glVertex3f(0, .15, 1);

	glVertex3f(.15, 0, 1);
	glVertex3f(0, 0, 1.6);
	glVertex3f(0, .15, 1);

	glVertex3f(.15, 0, 1);
	glVertex3f(.2, -.2, 1.3);
	glVertex3f(0, -.15, 1);

	glVertex3f(.15, 0, 1);
	glVertex3f(0, 0, 1.6);
	glVertex3f(0, -.15, 1);

	glVertex3f(-.15, 0, 1);
	glVertex3f(-.2, -.2, 1.3);
	glVertex3f(0, -.15, 1);

	glVertex3f(-.15, 0, 1);
	glVertex3f(0, 0, 1.6);
	glVertex3f(0, -.15, 1);

	glVertex3f(-.15, 0, 1);
	glVertex3f(-.2, .2, 1.3);
	glVertex3f(0, .15, 1);

	glVertex3f(-.15, 0, 1);
	glVertex3f(-0, 0, 1.6);
	glVertex3f(0, .15, 1);

	glEnd();

	glLineWidth(5.0);
	glBegin(GL_LINE_LOOP);
	materials(Orange);
	glVertex3f(.15, 0, 1.01);
	glVertex3f(0, -.15, 1.01);
	glVertex3f(-.15, 0, 1.01);
	glVertex3f(0, .15, 1.01);
	glEnd();


	glLineWidth(4.0);
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	materials(Cyan);
	glVertex3f(0, 0, 0);
	glVertex3f(.2, .2, 1.3);
	glVertex3f(.15, 0, 1);
	
	materials(Cyan);
	glVertex3f(0, 0, 0);
	glVertex3f(.2, .2, 1.3);
	glVertex3f(0, .15, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(-.2, .2, 1.3);
	glVertex3f(0, .15, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(-.2, .2, 1.3);
	glVertex3f(-.15, 0, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(-.2, -.2, 1.3);
	glVertex3f(-.15, 0, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(-.2, -.2, 1.3);
	glVertex3f(0, -.15, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(.2, -.2, 1.3);
	glVertex3f(0, -.15, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(.2, -.2, 1.3);
	glVertex3f(.15, 0, 1);

	glEnd();

}

void draw_vectors(){
	materials(white);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,.5,0);

	glVertex3f(0,0,0);
	glVertex3f(0,0,-1);

	glVertex3f(.5,0,0);
	glVertex3f(-.5,0,0);

	glEnd();


}

//the display call back - all drawing should be done in this function
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  gluLookAt(0, 0, 3.0, 0, 0, 0, 0, 1, 0);
  glLoadIdentity();
  glPushMatrix();
  //glTranslatef(2 * mlastx, 2 * mlasty, 0);
  pos_light();
  glPopMatrix();

  glPushMatrix();

  glMultMatrixf(trackballM);
  materials(GreenShiny);
  glRotatef(-30, 1, 1, 0);
  glTranslatef(0, 0, -.4);
  glScalef(1.5, .5, .8);
  draw_ship();
    glPushMatrix();
    glTranslatef(0, 0, .5);
    draw_vectors();
    glPopMatrix();
  glPopMatrix();

  glutSwapBuffers();
}


dvector get3d(float x, float y){
	dvector vector;
	if(sqrt((x*x) + (y*y)) > 1){
		vector.x = x / (sqrt((x*x) + (y*y)));
		vector.y = y / (sqrt((x*x) + (y*y)));
		vector.z = 0;
	}
	else {
		vector.x = x;
		vector.y = y;
		vector.z = sqrt(1 - (x*x) - (y*y));
	}
	vector.c = sqrt((vector.x*vector.x) + (vector.y*vector.y) + (vector.z*vector.z));
	return vector;
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) { /* if the left button is clicked */
      printf("mouse clicked at %d %d\n", x, y);
	rotx = p2wx(x);
	roty = p2wy(y);
	rotstart = get3d(rotx, roty);
      glutPostRedisplay();
    }
  }
}


void rotate(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(angle, cprod.x, cprod.y, cprod.z);
	glMultMatrixf(trackballM);
	glGetFloatv(GL_MODELVIEW_MATRIX, trackballM);
	glPopMatrix();
}

void cproduct(){
	cprod.x = (rotstart.y * rotend.z) - (rotstart.z * rotend.y);
	cprod.y = (rotstart.z * rotend.x) - (rotstart.x * rotend.z);
	cprod.z = (rotstart.x * rotend.y) - (rotstart.y * rotend.x);
}

void getAngle(){
	angle = acos(((rotstart.x * rotend.x) + (rotstart.y * rotend.y) + (rotstart.z * rotend.z)) / (rotstart.c * rotend.c)) * 57.2958;
}


//the mouse move callback
void mouseMove(int x, int y) {
  printf("mouse moved at %d %d\n", x, y);
	rotend = get3d(p2wx(x), p2wy(y));
	
	cproduct();
	getAngle();
	rotstart = rotend;
	
	rotate();
	glutPostRedisplay();
}

//the keyboard callback
void keyboard(unsigned char key, int x, int y ){
  switch( key ) {
    case 'q': case 'Q' :
      exit( EXIT_SUCCESS );
      break;
    case 'h' : case 'H' :
      printf("hello!\n");
      break;
  }
}



int main( int argc, char** argv ){

  glutInit( &argc, argv );
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  //intializations
  glutInitWindowSize(800, 800);
  glutInitWindowPosition(100, 100);
  glutCreateWindow( "My Second Window" );
  glClearColor(0.0, 0.0, 0.0, 1.0);
  
  glEnable(GL_DEPTH_TEST);

  //global variable intialization
  GW = GH = 500;
  //register the callback functions
  glutDisplayFunc( display );
  glutMouseFunc( mouse );
  glutMotionFunc( mouseMove );
  glutKeyboardFunc( keyboard );
  glutReshapeFunc( reshape );

  glEnable(GL_LIGHTING);
  init_lighting();
  glutMainLoop();
}
