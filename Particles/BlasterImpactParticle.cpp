/**
 * Particles that come off the blaster impacts.
 * @author Sterling Hirsh
 * @date 3/9/11
 */

#include "Particles/BlasterImpactParticle.h"
#include "math.h"

const float startingSize = 0.3f;
const float minLife = 0.5f; // Seconds

BlasterImpactParticle::BlasterImpactParticle(Point3D* _position,
      Vector3D* _velocity, float _life, float _r, float _g, float _b) :
   Particle(_position, _velocity, _life, _r, _g, _b) {
      size = startingSize;
   }

void BlasterImpactParticle::update(double timeDifference) {
   const float velocityScalePerSecond = 2;
   velocity->xMag -= velocity->xMag * velocityScalePerSecond * timeDifference;
   velocity->yMag -= velocity->yMag * velocityScalePerSecond * timeDifference;
   velocity->zMag -= velocity->zMag * velocityScalePerSecond * timeDifference;
   // Do the parent's update.
   Particle::update(timeDifference);
}

void BlasterImpactParticle::Add(Point3D* pos, Vector3D* vec) {

   float _fade = (float) ((0.5 * randdouble()) + minLife);
   float _r = (float) (0.9 + (randdouble() * 0.1));
   float _g = 0;
   float _b = 0;

   Particle::Add(new BlasterImpactParticle(pos, vec, _fade, _r, _g, _b));
}
