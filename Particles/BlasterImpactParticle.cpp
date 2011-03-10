/**
 * Particles that come off the blaster impacts.
 * @author Sterling Hirsh
 * @date 3/9/11
 */

#include "Particles/BlasterImpactParticle.h"
#include "math.h"

BlasterImpactParticle::BlasterImpactParticle(Point3D* _position,
      Vector3D* _velocity, float _life, float _r, float _g, float _b) :
   Particle(_position, _velocity, _life, _r, _g, _b) {
      size = 0.3;
   }

bool BlasterImpactParticle::step(double timeDifference) {
   const float velocityScalePerSecond = 2;
   velocity->xMag -= velocity->xMag * velocityScalePerSecond * timeDifference;
   velocity->yMag -= velocity->yMag * velocityScalePerSecond * timeDifference;
   velocity->zMag -= velocity->zMag * velocityScalePerSecond * timeDifference;
   return Particle::step(timeDifference);
}

void BlasterImpactParticle::Add(Point3D* pos, Vector3D* vec) {
   if (particles.size() >= MAX_PARTICLES) {
      std::cout << "max particles reached!" << std::endl;
      delete pos;
      delete vec;
      return;
   }
   const float minLife = 0.5; // Seconds

   float _fade = (0.5 * randdouble()) + minLife;
   float _r = 0.9 + (randdouble() * 0.1) ;
   float _g = 0;
   float _b = 0;

   particles.push_back(new BlasterImpactParticle(pos, vec, _fade, _r, _g, _b));
}
