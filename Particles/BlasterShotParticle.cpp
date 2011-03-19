/**
 * Particles that come off the blaster shots.
 * @author Sterling Hirsh
 * @date 2/17/11
 */

#include "Particles/BlasterShotParticle.h"
#include "math.h"

const float startingSize = 0.3f;
const float minLife = 1; // Seconds


BlasterShotParticle::BlasterShotParticle(Point3D* _position,
      Vector3D* _velocity, float _life, float _r, float _g, float _b) :
   Particle(_position, _velocity, _life, _r, _g, _b) {
      size = startingSize;
   }

void BlasterShotParticle::update(double timeDifference) {
   const float velocityScalePerSecond = 1;
   velocity->xMag -= velocity->xMag * velocityScalePerSecond * timeDifference;
   velocity->yMag -= velocity->yMag * velocityScalePerSecond * timeDifference;
   velocity->zMag -= velocity->zMag * velocityScalePerSecond * timeDifference;
   size -= timeDifference * startingSize / life;
   // Do the parent's update.
   Particle::update(timeDifference);
}

void BlasterShotParticle::Add(Point3D* pos, Vector3D* vec) {
   const float minLife = 0.5; // Seconds

   float _fade = minLife;
   float _r = 0.9f + ((float) randdouble() * 0.1f);
   float _g = 0;
   float _b = 0;

   Particle::Add(new BlasterShotParticle(pos, vec, _fade, _r, _g, _b));
}

void BlasterShotParticle::AddRainbow(Point3D* pos, Vector3D* vec, int particleNum, int particleCycle) {
   float _fade = minLife;
   float _r = 0;
   float _g = 0;
   float _b = 0;
   getBrightColor((float)(particleNum / particleCycle), _r, _g, _b);

   Particle::Add(new BlasterShotParticle(pos, vec, _fade, _r, _g, _b));

}
