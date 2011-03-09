
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <list>
#include <map>
#include <iostream>
#include "SDL_image.h"

class Image {
   public:
      Image();
      ~Image();
      
      static void Add(std::string file, std::string keyName);
      static SDL_Surface* getImage(std::string keyName);

   private:
      static std::map<std::string, SDL_Surface*> images;
      static int numOfImages;
};

#endif
