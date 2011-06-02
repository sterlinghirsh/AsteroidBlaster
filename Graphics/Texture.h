
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <list>
#include <map>
#include <iostream>

class Texture {
   public:
      Texture();
      ~Texture();
      
      static void Add(std::string file, std::string keyName);
      static void Add(int width, int height, std::string keyName,
            bool isDepth = false);
      static unsigned int getTexture(std::string keyName);

   private:
      static std::map<std::string, unsigned int> textures;
      static int numOfTextures;
};

#endif
