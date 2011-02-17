#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Graphics/TextureImporter.h"
#include "Utility/Vector3D.h"
#include "Utility/Point3D.h"
#include "Utility/GlobalUtility.h"
#include <string>
#include <list>

#define MAX_PARTICLES 7000
#define PARTICLE_LIFE 0.002f

class Particle {
   public:
      Particle(Point3D* p, Vector3D* v, float life, float fade, float r, float g, float b);
      static bool LoadGLTextures();
      
      static std::list<Particle*> particles;
      static void drawParticles();
      static void update(double timeDifference);
      void draw(Point3D* eyePoint);
      bool step(double timeDifference);
      
      static void Add(Point3D* pos, Vector3D* vec);
       
      Particle(std::string filename, int framesXIn, int framesYIn, double fpsIn, 
      Point3D posIn, double drawWidth, double drawHeight);
      static GLuint texture;
      bool shouldRemove;
      
   protected:
      Point3D* position;
      Vector3D* velocity;
      
      float life;
      float fade;
      float r,g,b;
      float slowdown;
      double startTime;
      double size;
};

#endif
