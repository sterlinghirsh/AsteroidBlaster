/**
 * Class for drawing a skybox
 * Sterling Hirsh
 * 471
 * 11/7/2010
 */

#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "Graphics/TextureImporter.h"
#include "Graphics/Camera.h"
#include <string>

class Skybox {
   public:
      materialStruct curMaterial;
      TextureImporter* tex;

      Skybox(std::string filename);
      void draw(Camera* camera);

   private:
      void drawcube();
};

#endif
