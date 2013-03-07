
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <list>
#include <map>
#include <iostream>

#include "SDL_image.h"
#include "Utility/Point3D.h"

class Image {
   public:
      Image(Point3D _p, Point3D _s, unsigned int _texture);
      ~Image();
      
      static void Add(Point3D _p, Point3D _s, unsigned int _texture, std::string keyName);
      static Image* getImage(std::string keyName);
      void drawImage();
      
   private:
      static std::map<std::string, Image*> images;
      static int numOfImages;
      Point3D location;
      Point3D scale;
      unsigned int texture;
};

#endif
