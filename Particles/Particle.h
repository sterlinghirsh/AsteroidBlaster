#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Items/Drawable.h"
#include "Utility/Texture.h"
#include "Utility/Vector3D.h"
#include "Utility/Point3D.h"
#include "Utility/GlobalUtility.h"
#include <string>
#include <list>

// Sensible limit on particles. Maybe should be even lower?
#define MAX_PARTICLES 5000
#define PARTICLE_LIFE 0.002f

class Particle : public Drawable {
   public:
      Particle(Point3D* p, Vector3D* v, float life, float r, float g, float b, const GameState* _gameState);
      virtual ~Particle();
      
      static std::list<Particle*> particles;
      static void updateParticles(double timeDifference);

      static void Add(Particle* newParticle);
      
      virtual void draw();
      virtual void update(double timeDifference);
       
      Particle(std::string filename, int framesXIn, int framesYIn, double fpsIn, 
      Point3D posIn, double drawWidth, double drawHeight);

      // Set to true by update() when the particle should be deleted
      bool shouldRemove;
      static int particleDisplayList;
      static void initDisplayList();
      
   protected:
      float life;
      float r,g,b;
      float slowdown;
      double startTime;
      double size;
};

#endif
