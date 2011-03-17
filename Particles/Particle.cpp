/**
 * Particle base class.
 */


#include "Particles/Particle.h"
#include "Utility/GameState.h"
#include <math.h>
#include <iostream>

#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif
#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif 


using namespace std;

list<Particle*> Particle::particles;
int Particle::particleDisplayList;

Particle::Particle(Point3D* _position, Vector3D* _velocity, float _life, float _r, float _g, float _b) :
   Drawable(0,0,0,0)
{
   position = _position;
   velocity = _velocity;
   
   slowdown = 2.0f;
   shouldRemove = false;
   life = _life;
   r = _r;
   g = _g;
   b = _b;
   startTime = doubleTime();
   size = 0.01;
   cullRadius = 1;
   radius = 1;
}

Particle::~Particle() {
}

void Particle::updateParticles(double timeDifference)
{
   list<Particle*>::iterator particle = Particle::particles.begin();
   for (; particle != Particle::particles.end(); ++particle) 
   {
      if (*particle != NULL) {
         (*particle)->update(timeDifference);
         if ((*particle)->shouldRemove) {
            particle = Particle::particles.erase(particle);
         }
      }
   }
}

/**
 * Sets the particle's shouldRemove to true if the particle has outlived its lifespan.
 */
void Particle::update(double timeDifference)
{
   // Do the parent's update.
   Drawable::update(timeDifference);
   // Move On The X Axis By X Speed 
   position->x += velocity->xMag * timeDifference;
   // Move On The Y Axis By Y Speed 
   position->y += velocity->yMag * timeDifference;
   // Move On The Z Axis By Z Speed 
   position->z += velocity->zMag * timeDifference;
   
   // This will take care of marking shouldRemove as true or not
   if(doubleTime() > (startTime + life)) {
      shouldRemove = true;
   };
}

void Particle::draw()
{
   glPushMatrix();
   /* Draw The Particle Using Our RGB Values,
   * Fade The Particle Based On It's Life
   */
   
   position->glTranslate();
   
   float alpha = ((startTime + life) - doubleTime()); 

   // glColor4f clamps alpha to [0, 1].
   glColor4f( r,g,b, alpha);
   glUseProgram(billboardShader);
   GLint sizeLoc = glGetUniformLocation(billboardShader, "size");
   glUniform1f(sizeLoc, size);

   glCallList(particleDisplayList);
   glPopMatrix();
}

void Particle::initDisplayList() {
   particleDisplayList = glGenLists(1);
   glNewList(particleDisplayList, GL_COMPILE);
   
   glBindTexture( GL_TEXTURE_2D, Texture::getTexture("Particle") );
   // TODO: The problem with this  is that particles don't fade out. They snap out.
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_LIGHTING);
   glEnable(GL_TEXTURE_2D);
   glDepthFunc(GL_ALWAYS);
   //Disable(GL_DEPTH_TEST);
   glDisable(GL_CULL_FACE);
   glBegin(GL_TRIANGLE_FAN);
   
   // Top Left
   glTexCoord2d( 0, 1 );
   glVertex3f( -1, 1, 0.0f );
   // Top Right
   glTexCoord2d( 1, 1 );
   glVertex3f(1, 1, 0.0f);
   // Bottom Right
   glTexCoord2d( 1, 0 );
   glVertex3f( 1, -1, 0.0f );
   // Bottom Left
   glTexCoord2d( 0, 0 );
   glVertex3f( -1, -1, 0.0f );
   glEnd( );
   
   glDisable(GL_TEXTURE_2D);
   glDepthFunc(GL_LEQUAL);
   //glEnable(GL_DEPTH_TEST);
   /*
   glEnable(GL_CULL_FACE);
   glEnable(GL_LIGHTING);
   */

   glUseProgram(0);
   glEndList();
}

void Particle::Add(Particle* newParticle) {
   if (particles.size() >= MAX_PARTICLES) {
      // Erase the oldest particle silently.
      delete particles.front();
      particles.pop_front();
   }
   particles.push_back(newParticle);
}
