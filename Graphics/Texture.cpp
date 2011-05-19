

#include "Graphics/Texture.h"
#include <iostream>
#include <map>
#include <math.h>
#include <string>

#ifdef WIN32
#include <windows.h>
#include <GL/glew.h>
#elif __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#elif linux
#include <GL/gl.h>
#include <GL/glu.h>
#else
std::cout << "Not Windows, OSX or linux..." << std::endl;
exit(1);
#endif


#include "SDL_image.h"

std::map<std::string, unsigned int> Texture::textures;
int Texture::numOfTextures = 0;


Texture::Texture() {

}

Texture::~Texture() {
   //std::map<std::string, unsigned int>::iterator it = Texture::textures.begin();
   //for (; it != Texture::textures.end(); ++it){
   //   SDL_FreeSurface(it->second);
   //}
}

void Texture::Add(std::string file, std::string keyName) {
   std::map<std::string, unsigned int>::iterator iter = textures.find(keyName);

   if (iter != textures.end()) {
      std::cerr << "That texture keyName (" << keyName << ") is already in use!" << std::endl;
      exit(1);
   }


   SDL_Surface *image;
   unsigned int texture;

   image=IMG_Load(file.c_str());
   if(!image) {
      printf("IMG_Load: %s\n", IMG_GetError());
      SDL_Quit();
      exit(1);
   }


   /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,       
   as expected by OpenGL for textures */    
   Uint32 rmask, gmask, bmask, amask;    
   /* SDL interprets each pixel as a 32-bit number, so our masks must depend       
   on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN    
   rmask = 0xff000000;
   gmask = 0x00ff0000;
   bmask = 0x0000ff00;
   amask = 0x000000ff;
#else    
   rmask = 0x000000ff;  
   gmask = 0x0000ff00;
   bmask = 0x00ff0000;
   amask = 0xff000000;
#endif

   GLenum texture_format;
   GLint  nOfColors;
   // get the number of channels in the SDL surface
   nOfColors = image->format->BytesPerPixel;
   if (nOfColors == 4) {    // contains an alpha channel
      if (image->format->Rmask == 0x000000ff)
         texture_format = GL_RGBA;
      else
         texture_format = GL_BGRA;
   } else if (nOfColors == 3) {    // no alpha channel
      if (image->format->Rmask == 0x000000ff)
         texture_format = GL_RGB;
      else
         texture_format = GL_BGR;
   } else {
      printf("warning: the image is not truecolor..  this will probably break\n");
      // this error should not go unhandled
   }

   // create texture
   glGenTextures(1, &texture );
   glBindTexture(GL_TEXTURE_2D, texture );
   
   // turn bitmap into texture
   //glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, temp->w, temp->h, 0, texture_format, GL_UNSIGNED_BYTE, temp->pixels );
   gluBuild2DMipmaps(GL_TEXTURE_2D, nOfColors, image->w, image->h, 
    texture_format, GL_UNSIGNED_BYTE, image->pixels);

   // set texture filtering
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      // free image
   SDL_FreeSurface(image);

   textures.insert(std::pair<std::string, unsigned int>(keyName,texture));
     
   numOfTextures++;
}

// Should insert a new blank texture.
void Texture::Add(int width, int height, std::string keyName) {
   std::map<std::string, unsigned int>::iterator iter = textures.find(keyName);

   if (iter != textures.end()) {
      std::cerr << "That texture keyName (" << keyName << ") is already in use!" << std::endl;
      exit(1);
   }

   GLuint texture;

   // create texture
   glGenTextures(1, &texture );
   glBindTexture(GL_TEXTURE_2D, texture );

   // turn bitmap into texture
   glTexImage2D(GL_TEXTURE_2D, 0, 3, width,
         height, 0, GL_RGB,
         GL_UNSIGNED_BYTE, NULL);
   
   // set texture filtering
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   textures.insert(std::pair<std::string, unsigned int>(keyName,texture));
     
   numOfTextures++;
}

unsigned int Texture::getTexture(std::string keyName) {
   std::map<std::string, unsigned int>::iterator iter = textures.find(keyName);

   if (iter == textures.end()) {
      std::cerr << "failed to find: " << keyName << " in Texture::textures." << std::endl;
      exit(1);
   }
   
   return iter->second;
}




