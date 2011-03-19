/**
 * Particles that come off the elecricity impacts.
 * @author Sterling Hirsh
 * @date 3/9/11
 */

#include "Particles/ElectricityImpactParticle.h"
#include "math.h"

const float startingSize = 0.3f;
const float minLife = 0.8f; // Seconds

ElectricityImpactParticle::ElectricityImpactParticle(Point3D* _position,
      Vector3D* _velocity, float _life, float _r, float _g, float _b) :
   Particle(_position, _velocity, _life, _r, _g, _b) {
      size = startingSize;
   }

void ElectricityImpactParticle::update(double timeDifference) {
   /*
   const float velocityScalePerSecond = 0;
   velocity->xMag -= velocity->xMag * velocityScalePerSecond * timeDifference;
   velocity->yMag -= velocity->yMag * velocityScalePerSecond * timeDifference;
   velocity->zMag -= velocity->zMag * velocityScalePerSecond * timeDifference;
   */
   // Do the parent's update.
   Particle::update(timeDifference);
}

void ElectricityImpactParticle::Add(Point3D* pos, Vector3D* vec) {
   float _fade = (float) ((0.5 * randdouble()) + minLife);
   float _r = 1;
   float _g = 1;
   float _b = (float) (0.8 + (randdouble() * 0.2));

   Particle::Add(new ElectricityImpactParticle(pos, vec, _fade, _r, _g, _b));
}

