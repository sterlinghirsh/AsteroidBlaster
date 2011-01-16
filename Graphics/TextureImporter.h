/**
 * Sterling Hirsh
 * Texture Importer
 * Based on Zoe Wood's example code for 471
 * 11/17/2010
 */

#ifndef __TEXTUREIMPORTER_H__
#define __TEXTUREIMPORTER_H__

#include <stdio.h>
#include <iostream>
#include <map>
#include <string>

class TextureImporter {
   public:
      int texID;
      char* data;
      static int curTexID;
      static std::map<std::string, int> texIDMap;

      TextureImporter(std::string filename);
   private:
      unsigned long sizeX;
      unsigned long sizeY;

      static unsigned int getint(FILE *fp);
      unsigned int getshort(FILE *fp);
      int ImageLoad(std::string filename);
};

#endif
