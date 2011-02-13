#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Graphics/TextureImporter.h"
#include "Utility/Vector3D.h"
#include "Utility/Point3D.h"
#include "Graphics/GlutUtility.h"
#include <string>
#include <list>

class Particle {
   public:
      Particle(Point3D* p, Vector3D* v, float life, float fade, float r, float g, float b);
      static bool LoadGLTextures();
      
      static std::list<Particle*> particles;
      static void drawParticles();
      static void update(double timeDifference);
      bool draw(Point3D* eyePoint);
      bool step(double timeDifference);
      
      static void Add(Point3D* pos, Vector3D* vec);
       
      Particle(std::string filename, int framesXIn, int framesYIn, double fpsIn, 
      Point3D posIn, double drawWidth, double drawHeight);
      static GLuint texture;
      
   private:
      Point3D* position;
      Vector3D* velocity;
      bool active;
      float life;
      float fade;
      float r,g,b;
      float slowdown;
      double startTime;
};

#endif
