


#include "Graphics/Particle.h"
#include "Utility/GameState.h"
#include <math.h>
#include <iostream>

#define MAX_PARTICLES 7000
#define PARTICLE_SIZE 0.01f
#define PARTICLE_LIFE 0.002f
#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif
#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif 


using namespace std;

list<Particle*> Particle::particles;
GLuint Particle::texture; 

Particle::Particle(Point3D* _position, Vector3D* _velocity, float _life, float _fade, float _r, float _g, float _b)
{
   position = _position;
   velocity = _velocity;
   slowdown = 2.0f;
   shouldRemove = false;
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
      if ((*particle)->step(timeDifference)) {
         particle = Particle::particles.erase(particle);
      }
   }
}

bool Particle::step(double timeDifference)
{
   // Move On The X Axis By X Speed 
   position->x += velocity->xMag * timeDifference;
   // Move On The Y Axis By Y Speed 
   position->y += velocity->yMag * timeDifference;
   // Move On The Z Axis By Z Speed 
   position->z += velocity->zMag * timeDifference;
   
   //std::cout << "timeDifference= " << timeDifference << ": (" << position->x << "," << position->y << "," << position->z << ")" << std::endl;

   // Reduce Particles Life By 'Fade'
   life -= fade * timeDifference;
   //std::cout << "timeDifference = " << timeDifference << std::endl;
   //std::cout << "life = " << life << std::endl;
   //std::cout << "fade = " << fade << std::endl;
   return life < 0;
}

void Particle::Add(Point3D* pos, Vector3D* vec)
{
   if (particles.size() >= MAX_PARTICLES) {
      std::cout << "max particles reached!" << std::endl;
      return;
   }
   float _fade = ( float )( rand( ) %10 ) / 10000.0f + 0.001f;
   float _r = (( float )( rand( ) %100 ) ) / 100.0f ;
   float _g = (( float )( rand( ) %100 ) ) / 100.0f ;
   float _b = (( float )( rand( ) %100 ) ) / 100.0f ;
   
   
   float randHigh = (( float )( rand( ) %10 ) + 90.0f) / 100.0f ;
   float randMid = (( float )( rand( ) %10 ) + 50.0f) / 100.0f ;
   float randLow = (( float )( rand( ) %10 ) + 0.0f) / 100.0f ;
   
   int chooser = (rand() %6);
   switch ( chooser ) {
      case 0: 
         _r = randHigh;
         _g = randMid;
         _b = randLow;
         break;
      case 1: 
         _r = randHigh;
         _g = randLow;
         _b = randMid;
         break;
      case 2: 
         _r = randMid;
         _g = randHigh;
         _b = randLow;
         break;
      case 3: 
         _r = randMid;
         _g = randLow;
         _b = randHigh;
         break;
      case 4: 
         _r = randLow;
         _g = randMid;
         _b = randHigh;
         break;
      case 5: 
         _r = randLow;
         _g = randHigh;
         _b = randMid;
         break;
   }
   
   particles.push_back(new Particle(pos, vec, PARTICLE_LIFE, _fade, _r, _g, _b));
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

   
   //materials(Orange);
   //gluSphere(quadric, 0.05, 20, 20);

   glBindTexture( GL_TEXTURE_2D, Particle::texture );
   glColor4f( r,g,b, life );
   
   glDisable(GL_LIGHTING);
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_CULL_FACE);
   glBegin(GL_QUADS);
   
   // Top Left
   glTexCoord2d( 0, 1 );
   glVertex3f( -PARTICLE_SIZE, PARTICLE_SIZE, 0.0f );
   // Top Right
   glTexCoord2d( 1, 1 );
   glVertex3f(PARTICLE_SIZE, PARTICLE_SIZE, 0.0f);
   // Bottom Right
   glTexCoord2d( 1, 0 );
   glVertex3f( PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f );
   // Bottom Left
   glTexCoord2d( 0, 0 );
   glVertex3f( -PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f );
   glEnd( );
   
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_CULL_FACE);
   glEnable(GL_LIGHTING);
   //std::cout << "drew: (" << position->x << "," << position->y << "," << position->z << ")" << std::endl;
   glPopMatrix();
}

void Particle::drawParticles()
{
   glDisable(GL_COLOR_MATERIAL);
   //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   
   extern GameState* gameState;
   
   Point3D eyePoint = (gameState->getCamera()->getEyePoint());
   /* Modify each of the particles */
   list<Particle*>::iterator particle = Particle::particles.begin();
   for (; particle != Particle::particles.end(); particle++) 
   {
      (*particle)->draw(&eyePoint);
   }
}

