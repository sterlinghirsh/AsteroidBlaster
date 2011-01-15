/**
 * Sterling Hirsh
 * Texture Importer
 * Based on Zoe Wood's example code for 471
 * 11/17/2010
 */

#ifndef __TEXTUREIMPORTER_H__
#define __TEXTUREIMPORTER_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

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
      TextureImporter(std::string filename) {
         texID = TextureImporter::curTexID++;

         std::string fileNameString(filename);
         std::map<std::string, int>::iterator iter = texIDMap.find(filename);
         if (iter != texIDMap.end()) {
            texID = iter->second;
            return;
         }
         
         if (!ImageLoad(filename)) {
           exit(1);
         }  

         texIDMap.insert(std::pair<std::string, int>(filename, texID));
         
         /*  2d texture, level of detail 0 (normal), 3 components (red, green, blue),
          *  x size from image, y size from image,
          *  border 0 (normal), rgb color data, unsigned byte data, data  
          */ 
         glBindTexture(GL_TEXTURE_2D, texID);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizeX, sizeY,
            0, GL_RGBA, GL_UNSIGNED_BYTE, data);
         /*  cheap scaling when image smaller or bigger than texture */    
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      }
   private:
      unsigned long sizeX;
      unsigned long sizeY;

      /**
       * getint and getshort are help functions to load the bitmap byte by byte
       */
      static unsigned int getint(FILE *fp) {
        int c, c1, c2, c3;
        
        /*  get 4 bytes */ 
        c = getc(fp);  
        c1 = getc(fp);  
        c2 = getc(fp);  
        c3 = getc(fp);
        
        return ((unsigned int) c) +   
          (((unsigned int) c1) << 8) + 
          (((unsigned int) c2) << 16) +
          (((unsigned int) c3) << 24);
      }

      unsigned int getshort(FILE *fp){
        int c, c1;
        
        /* get 2 bytes*/
        c = getc(fp);  
        c1 = getc(fp);
        
        return ((unsigned int) c) + (((unsigned int) c1) << 8);
      }

      int ImageLoad(std::string filename) {
        const int bytesPerPixel = 4;
        FILE *file;
        unsigned long size;         /*  size of the image in bytes. */
        unsigned long i;            /*  standard counter. */
        unsigned short int planes;  /*  number of planes in image (must be 1)  */
        unsigned short int bpp;     /*  number of bits per pixel (must be 24) */
        char temp;                  /*  used to convert bgr to rgb color. */
        
        /*  make sure the file is there. */
        if ((file = fopen(filename.c_str(), "rb")) == NULL) {
          printf("File Not Found : %s\n",filename.c_str());
          return 0;
        }
        
        /*  seek through the bmp header, up to the width height: */
        fseek(file, 18, SEEK_CUR);
        
        /*  read the width and height. */    
        sizeX = getint (file);
        sizeY = getint (file);
        
        /*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
        size = sizeX * sizeY * bytesPerPixel;

        /*  read the planes */    
        planes = getshort(file);
        if (planes != 1) {
          printf("Planes from %s is not 1: %u\n", filename.c_str(), planes);
          return 0;
        }
        
        /*  read the bpp */    
        bpp = getshort(file);
        if (bpp != 24) {
          printf("Bpp from %s is not 24: %u\n", filename.c_str(), bpp);
          return 0;
        }
        
        /*  seek past the rest of the bitmap header. */
        fseek(file, 24, SEEK_CUR);
        
        /*  read the data.  */
        data = (char *) malloc(size + bytesPerPixel);
        if (data == NULL) {
          printf("Error allocating memory for color-corrected image data");
          return 0; 
        }

        for (i = 0; i < size / bytesPerPixel; i += 1) {
          if (fread(data + (i * bytesPerPixel), 3, 1, file) != 1) {
             printf("Error reading image data from %s.\n", filename.c_str());
             return 0;
          }

          temp = data[i * bytesPerPixel];
          data[i * bytesPerPixel] = data[(i * bytesPerPixel) + 2];
          data[(i * bytesPerPixel) + 2] = temp;
          data[(i * bytesPerPixel) + 3] = 0x00;
        }
        
        /*
        if ((i = fread(data, size, 1, file)) != 1) {
          printf("Error reading image data from %s.\n", filename.c_str());
          return 0;
        }
        */

         /*  reverse all of the colors. (bgr -> rgb) */
        /*
        for (i = 0; i < size; i += 3) {
          temp = data[i];
          data[i] = data[i+2];
          data[i+2] = temp;
        }
        */
        
        fclose(file); /* Close the file and release the filedes */
        return 1;
      }
};

#endif
