
#include "Graphics/Image.h"

#include <iostream>
#include <map>
#include <string>
#include "SDL_image.h"

std::map<std::string, Image*> Image::images;
int Image::numOfImages = 0;


Image::Image(Point3D _p, Point3D _s, unsigned int _texture){
   location.x = _p.x;
   location.y = _p.y;
   location.z = _p.z;
   
   scale.x = _s.x;
   scale.y = _s.y;
   scale.z = _s.z;
   
   texture = _texture;
}

Image::~Image() {
   std::map<std::string, Image*>::iterator it = Image::images.begin();
   for (; it != Image::images.end(); ++it ) {
      delete it->second;
   }
}

void Image::Add(Point3D _p, Point3D _s, unsigned int _texture, std::string keyName) {
   Image* tempImage = new Image(_p, _s, _texture);

   images.insert( std::pair<std::string, Image*>(keyName,tempImage) );
   numOfImages++;
}

void Image::drawImage(){
   glDisable(GL_LIGHTING);
   glEnable(GL_TEXTURE_2D);
   
   /* Enable smooth shading */
   glShadeModel( GL_SMOOTH );

   /* Depth buffer setup */
   glClearDepth( 1.0f );

   /* Enables Depth Testing */
   glEnable( GL_DEPTH_TEST );

   /* The Type Of Depth Test To Do */
   glDepthFunc( GL_LEQUAL );

   /* Really Nice Perspective Calculations */
   glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
   
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                     GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                     GL_CLAMP );
   
   glPushMatrix();
      glLoadIdentity( );
      glTranslatef( (float)location.x, (float)location.y, (float)location.z );
      glScaled( (float)scale.x, (float)scale.y, (float)scale.z );
      glColor3f(1.0, 1.0, 1.0);

      /* Select Our Texture */
      glBindTexture( GL_TEXTURE_2D, texture );

      /* NOTE:
      *   The x coordinates of the glTexCoord2f function need to inverted
      * for SDL because of the way SDL_LoadBmp loads the data. So where
      * in the tutorial it has glTexCoord2f( 1.0f, 0.0f ); it should
      * now read glTexCoord2f( 0.0f, 1.0f );
      */
      glBegin(GL_QUADS);
         /* Front Face */
         /* Bottom Left Of The Texture and Quad */
         glTexCoord2f( 0.0f, 1.0f ); glVertex2f( -1.0f, -1.0f);
         /* Bottom Right Of The Texture and Quad */
         glTexCoord2f( 1.0f, 1.0f ); glVertex2f(  1.0f, -1.0f);
         /* Top Right Of The Texture and Quad */
         glTexCoord2f( 1.0f, 0.0f ); glVertex2f(  1.0f,  1.0f);
         /* Top Left Of The Texture and Quad */
         glTexCoord2f( 0.0f, 0.0f ); glVertex2f( -1.0f,  1.0f);
      glEnd();
      glEnable(GL_CULL_FACE);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);
}

Image* Image::getImage(std::string keyName){
   std::map<std::string, Image*>::iterator iter = images.find(keyName);
   return iter->second;
}




