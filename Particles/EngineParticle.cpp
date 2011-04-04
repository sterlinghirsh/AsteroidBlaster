/**
 * Particles from the engine of the ship.
 */

#include "Particles/EngineParticle.h"

const float startingSize = 0.4f;
const float minLife = 60; // Seconds

EngineParticle::EngineParticle(Point3D* p, Vector3D* v, float life, float r, float g, float b) : Particle(p, v, life, r, g, b) {
   // Nothing special :/
   size = startingSize;
};

/**
 * When color is 1, this makes red stuff. 2 is Green, 3 is Blue.
 */
void EngineParticle::Add(Point3D* pos, Vector3D* vec, int color) {
   float _fade = (float) randdouble() + minLife;
   float _r = 1.0f;
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

   Particle::Add(new EngineParticle(pos, vec, _fade, _r, _g, _b));
}

void EngineParticle::update(double timeDifference) {
   const float velocityScalePerSecond = 3;
   velocity->x -= velocity->x * velocityScalePerSecond * timeDifference;
   velocity->y -= velocity->y * velocityScalePerSecond * timeDifference;
   velocity->z -= velocity->z * velocityScalePerSecond * timeDifference;
   // Do the parent's update.
   Particle::update(timeDifference);
}
