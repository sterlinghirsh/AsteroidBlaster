/**
 * Particles from the engine of the ship.
 */

#include "Particles/EngineParticle.h"

EngineParticle::EngineParticle(Point3D* p, Vector3D* v, float life, float fade, float r, float g, float b) : Particle(p, v, life, fade, r, g, b) {
   // Nothing special :/
   size = 0.1;
};

void EngineParticle::Add(Point3D* pos, Vector3D* vec, int color) {
   if (particles.size() >= MAX_PARTICLES) {
      std::cout << "max particles reached!" << std::endl;
      delete pos;
      delete vec;
      return;
   }

   // TODO: Make this number make sense.
   float _fade = randdouble() / 10000;
   float _r = 0.9 + ((( float )( rand( ) %10 ) ) / 100.0f) ;
   float _g = 0;
   float _b = 0;

   if (color == 2) {
      _g = _r;
      _r = 0;
   }

   if (color == 3) {
      _b = _r;
      _r = 0;
   }

   particles.push_back(new EngineParticle(pos, vec, PARTICLE_LIFE, _fade, _r, _g, _b));
}

bool EngineParticle::step(double timeDifference) {
   const float velocityScalePerSecond = 3;
   velocity->xMag -= velocity->xMag * velocityScalePerSecond * timeDifference;
   velocity->yMag -= velocity->yMag * velocityScalePerSecond * timeDifference;
   velocity->zMag -= velocity->zMag * velocityScalePerSecond * timeDifference;
   Particle::step(timeDifference);
}
