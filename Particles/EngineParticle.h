#ifndef __ENGINEPARTICLE_H__
#define __ENGINEPARTICLE_H__

#include "Particles/Particle.h"

class EngineParticle : public Particle {
   public:
      EngineParticle(Point3D* p, Vector3D* v, float life, float r, float g, float b, const GameState* _gameState);
      static void Add(Point3D* pos, Vector3D* vec, int color, const GameState*);
      virtual void update(double timeDifference);

};

#endif
