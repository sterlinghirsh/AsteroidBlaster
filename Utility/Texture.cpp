
#include "Utility/Texture.h"
#include <iostream>
#include <map>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL_image.h"

std::map<std::string, unsigned int> Texture::textures;
int Texture::numOfTextures = 0;


Texture::Texture() {

}

Texture::~Texture() {
   //std::map<std::string, unsigned int>::iterator it = Texture::textures.begin();
   //for (; it != Texture::textures.end(); ++it){
  //    SDL_FreeSurface(it->second);
   //}
}

void Texture::Add(std::string file, std::string keyName) {
   std::map<std::string, unsigned int>::iterator iter = textures.find(keyName);

   if (iter != textures.end()) {
      std::cerr << "That texture keyName (" << keyName << ") is already in use!" << std::endl;
      exit(1);
   }


   SDL_Surface *image, *temp;
   int x = 0, y = 0, w, h;
   SDL_Rect area;
   unsigned int texture;

   image=IMG_Load(file.c_str());
   if(!image) {
      printf("IMG_Load: %s\n", IMG_GetError());
      SDL_Quit();
      exit(1);
   }

   // set color key
   SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(image->format, 255, 0, 128));

   // set width and height
   int width = image->w;
   int height = image->h;
   
   // get the closest w and h that's in the power of 2
   int tempLoop = 1;
   int loopVal = 1;
   while (loopVal < width) {
      loopVal = pow(tempLoop, 2);
      tempLoop++;
   }
   w = loopVal;
   
   tempLoop = 1;
   loopVal = 1;
   while (loopVal < height) {
      loopVal = pow(tempLoop, 2);
      tempLoop++;
   }
   h = loopVal;

   // blit from bitmap to a temporary surface with 32 bits per pixel
   temp = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 );

   if(temp == NULL){
      std::cerr << "Failed to create temporary surface..." << std::endl;
      exit (1);
   }

   // set up rectangle
   area.x = x * width;
   area.y = y * height;
   area.w = width;
   area.h = height;

   if(SDL_BlitSurface(image, &area, temp, NULL)== -1){
      std::cerr << "Failed to blit bitmap to temporary surface..." << std::endl;
      exit (1);
   }

   // create texture
   glGenTextures(1, &texture );
   glBindTexture(GL_TEXTURE_2D, texture );
   
   if(temp->pixels == NULL){
      std::cerr << "Failed to create texture..." << std::endl;
      exit (1);
   }

   // if temporary surface must be locked
   // lock temporary surface
   if(SDL_MUSTLOCK(temp)) {
      SDL_LockSurface(temp);
   }

   // turn bitmap into texture
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels );

   // if temporary surface had to be locked
   // unlock temporary surface again
   if(SDL_MUSTLOCK(temp)) {
      SDL_UnlockSurface(temp );
   }

   // set texture filtering
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   // free temporary surface
   SDL_FreeSurface(temp);

   // free image
   SDL_FreeSurface(image);


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




