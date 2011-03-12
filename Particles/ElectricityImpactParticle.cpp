/**
 * Particles that come off the elecricity impacts.
 * @author Sterling Hirsh
 * @date 3/9/11
 */

#include "Particles/ElectricityImpactParticle.h"
#include "math.h"

ElectricityImpactParticle::ElectricityImpactParticle(Point3D* _position,
      Vector3D* _velocity, float _life, float _r, float _g, float _b) :
   Particle(_position, _velocity, _life, _r, _g, _b) {
      size = 0.3;
   }

bool ElectricityImpactParticle::step(double timeDifference) {
   const float velocityScalePerSecond = 0;
   velocity->xMag -= velocity->xMag * velocityScalePerSecond * timeDifference;
   velocity->yMag -= velocity->yMag * velocityScalePerSecond * timeDifference;
   velocity->zMag -= velocity->zMag * velocityScalePerSecond * timeDifference;
   return Particle::step(timeDifference);
}

void ElectricityImpactParticle::Add(Point3D* pos, Vector3D* vec) {
   if (particles.size() >= MAX_PARTICLES) {
      std::cout << "max particles reached!" << std::endl;
      delete pos;
      delete vec;
      return;
   }
   const float minLife = 0.8; // Seconds

   float _fade = (0.5 * randdouble()) + minLife;
   float _r = 1;
   float _g = 1;
   float _b = 0.8 + (randdouble() * 0.2) ;

   particles.push_back(new ElectricityImpactParticle(pos, vec, _fade, _r, _g, _b));
}

