/**
 * Particles that come off the blaster shots.
 * @author Sterling Hirsh
 * @date 2/17/11
 */

#include "Graphics/BlasterShotParticle.h"

BlasterShotParticle::BlasterShotParticle(Point3D* _position, 
 Vector3D* _velocity, float _life, float _fade, float _r, float _g, float _b) :
 Particle(_position, _velocity, _life, _fade, _r, _g, _b) {
   // Do nothing new.
   size = 0.1;
}

bool BlasterShotParticle::step(double timeDifference) {
   const float velocityScalePerSecond = 0.1;
   velocity->scalarMultiplyUpdate(velocityScalePerSecond * timeDifference);
   Particle::step(timeDifference);
}

void BlasterShotParticle::Add(Point3D* pos, Vector3D* vec) {
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
   
   particles.push_back(new BlasterShotParticle(pos, vec, PARTICLE_LIFE, _fade, _r, _g, _b));

}
