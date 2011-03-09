/**
 * Particles from the engine of the ship.
 */

#include "Particles/EngineParticle.h"

EngineParticle::EngineParticle(Point3D* p, Vector3D* v, float life, float fade, float r, float g, float b) : Particle(p, v, life, fade, r, g, b) {
   // Nothing special :/
};

void EngineParticle::Add(Point3D* pos, Vector3D* vec) {
   if (particles.size() >= MAX_PARTICLES) {
      std::cout << "max particles reached!" << std::endl;
      delete pos;
      delete vec;
      return;
   }

   float _fade = ( float )( rand( ) %10 ) / 10000.0f + 0.001f;
   float _r = 0.9 + ((( float )( rand( ) %10 ) ) / 100.0f) ;
   float _g = 0;
   float _b = 0;

   particles.push_back(new EngineParticle(pos, vec, PARTICLE_LIFE, _fade, _r, _g, _b));
}
