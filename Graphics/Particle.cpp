#include "Graphics/Particle.h"
#include "Utility/GameState.h"
#include <math.h>
#include <iostream>

#define MAX_PARTICLES 1000

using namespace std;

list<Particle*> Particle::particles;
GLuint Particle::texture; 

Particle::Particle(Point3D* _position, Vector3D* _velocity, float _life, float _fade, float _r, float _g, float _b)
{
   position = _position;
   velocity = _velocity;
   slowdown = 2.0f;
   active = true;
   life = _life;
   fade = _fade;
   r = _r;
   g = _g;
   b = _b;
   startTime = doubleTime();
}

/* function to load in bitmap as a GL texture */
bool Particle::LoadGLTextures()
{
    /* Status indicator */
    bool Status = false;

    /* Create storage space for the texture */
    SDL_Surface *TextureImage[1]; 

    /* Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit */
    if ( ( TextureImage[0] = SDL_LoadBMP( "Images/particle.bmp" ) ) )
        {

       /* Set the status to true */
       Status = true;

       glBindTexture( GL_TEXTURE_2D, Particle::texture );
       
       /* Create The Texture */
       glGenTextures( 1, &texture );

       /* Typical Texture Generation Using Data From The Bitmap */
       glBindTexture( GL_TEXTURE_2D, texture );

       /* Generate The Texture */
       glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage[0]->w,
           TextureImage[0]->h, 0, GL_BGR,
           GL_UNSIGNED_BYTE, TextureImage[0]->pixels );

       /* Linear Filtering */
       glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
       glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        }

    /* Free up any memory we may have used */
    if ( TextureImage[0] )
       SDL_FreeSurface( TextureImage[0] );
       
    return Status;
}

void Particle::update(double timeDifference)
{
   list<Particle*>::iterator particle = Particle::particles.begin();
   for (; particle != Particle::particles.end(); particle++) 
   {
      if (!(*particle)->step(timeDifference)) {
         particle = Particle::particles.erase(particle);
         std::cout << "particle died!" << std::endl;
         continue;
      }
   }
}

bool Particle::step(double timeDifference)
{
   // Move On The X Axis By X Speed 
   position->x += velocity->xPos / ( slowdown * 1000 );
   // Move On The Y Axis By Y Speed 
   position->y += velocity->yPos / ( slowdown * 1000 );
   // Move On The Z Axis By Z Speed 
   position->z += velocity->zPos / ( slowdown * 1000 );
   
   std::cout << "changed: (" << position->x << "," << position->y << "," << position->z << ")" << std::endl;

   // Reduce Particles Life By 'Fade'
   life -= fade;
   std::cout << "life = " << life << std::endl;
   std::cout << "fade = " << fade << std::endl;
   return life > 0;
}

void Particle::Add(Point3D* pos, Vector3D* vec)
{
   float _fade = ( float )( rand( ) %100 ) / 1000.0f + 0.03f;
   float _r = 0.5f;
   float _g = 0.5f;
   float _b = 0.5f;
   
   particles.push_back(new Particle(pos, vec, 10.0f, _fade, _r, _g, _b));
}

bool Particle::draw(Point3D* eyePoint)
{

   /* Draw The Particle Using Our RGB Values,
   * Fade The Particle Based On It's Life
   */

   glColor4f( r,g,b,life );

   glTranslatef(position->x, position->y, position->z);
   //materials(Orange);
   gluSphere(quadric, 0.05, 20, 20);
   /*
   // Build Quad From A Triangle Strip
   glBegin( GL_TRIANGLE_STRIP );
   // Top Right
   glTexCoord2d( 1, 1 );
   glVertex3f( position->x + 0.5f, position->y + 0.5f, position->z );
   // Top Left
   glTexCoord2d( 0, 1 );
   glVertex3f( position->x - 0.5f, position->y + 0.5f, position->z );
   // Bottom Right
   glTexCoord2d( 1, 0 );
   glVertex3f( position->x + 0.5f, position->y - 0.5f, position->z );
   // Bottom Left
   glTexCoord2d( 0, 0 );
   glVertex3f( position->x - 0.5f, position->y - 0.5f, position->z );
   glEnd( );
   */

   //std::cout << "drew: (" << position->x << "," << position->y << "," << position->z << ")" << std::endl;

}

void Particle::drawParticles()
{
   glDisable(GL_COLOR_MATERIAL);
   //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glPushMatrix();
   extern GameState* gameState;
   
   /* Clear The Screen And The Depth Buffer */
   //

   //glLoadIdentity( );

   /* Modify each of the particles */
   list<Particle*>::iterator particle = Particle::particles.begin();
   for (; particle != Particle::particles.end(); particle++) 
   {
      (*particle)->draw(gameState->ship->position);
   }
   
   glPopMatrix();
   //glEnable(GL_COLOR_MATERIAL);
   /* Draw it to the screen */
   //SDL_GL_SwapBuffers( );
   
}

