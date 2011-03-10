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
}

void Particle::update(double timeDifference)
{
   list<Particle*>::iterator particle = Particle::particles.begin();
   for (; particle != Particle::particles.end(); ++particle) 
   {
      if ((*particle)->step(timeDifference)) {
         particle = Particle::particles.erase(particle);
      }
   }
}

/**
 * Return true to delete the particle.
 */
bool Particle::step(double timeDifference)
{
   // Move On The X Axis By X Speed 
   position->x += velocity->xMag * timeDifference;
   // Move On The Y Axis By Y Speed 
   position->y += velocity->yMag * timeDifference;
   // Move On The Z Axis By Z Speed 
   position->z += velocity->zMag * timeDifference;
   
   return doubleTime() > startTime + life;
}

void Particle::draw(Point3D* eyePoint)
{
   glPushMatrix();
   /* Draw The Particle Using Our RGB Values,
   * Fade The Particle Based On It's Life
   */
   
   Vector3D forward(0, 0, 1);
   Vector3D toCamera(*position, *eyePoint);
   Vector3D cross = forward.cross(toCamera);

   double angle = forward.getAngleInDegrees(toCamera);
   
   position->glTranslate();
   glRotatef(angle, cross.xMag, cross.yMag, cross.zMag);

   
   setMaterial(WhiteSolid);
   
   float alpha = ((startTime + life) - doubleTime()); 
   alpha = clamp(alpha, 0, 1);

   glBindTexture( GL_TEXTURE_2D, Texture::getTexture("Particle") );
   glColor4f( r,g,b, alpha);

   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_LIGHTING);
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_CULL_FACE);
   //glDisable(GL_DEPTH_TEST);
   glBegin(GL_QUADS);
   
   // Top Left
   glTexCoord2d( 0, 1 );
   glVertex3f( -size, size, 0.0f );
   // Top Right
   glTexCoord2d( 1, 1 );
   glVertex3f(size, size, 0.0f);
   // Bottom Right
   glTexCoord2d( 1, 0 );
   glVertex3f( size, -size, 0.0f );
   // Bottom Left
   glTexCoord2d( 0, 0 );
   glVertex3f( -size, -size, 0.0f );
   glEnd( );
   
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_CULL_FACE);
   glEnable(GL_LIGHTING);
   //glEnable(GL_DEPTH_TEST);
   //std::cout << "drew: (" << position->x << "," << position->y << "," << position->z << ")" << std::endl;
   glPopMatrix();
}

void Particle::drawParticles()
{
   extern GameState* gameState;
   
   Point3D eyePoint = (gameState->getCamera()->getEyePoint());
   /* Modify each of the particles */
   list<Particle*>::iterator particle = Particle::particles.begin();
   for (; particle != Particle::particles.end(); ++particle) 
   {
      (*particle)->draw(&eyePoint);
   }
}

