/**
 * Particles that come off shards during attraction.
 * @author Sterling Hirsh
 * @date 3/9/11
 */

#include "Particles/TractorAttractionParticle.h"
#include "math.h"

TractorAttractionParticle::TractorAttractionParticle(Point3D* _position,
      Vector3D* _velocity, float _life, float _r, float _g, float _b, Point3D* _target) :
   Particle(_position, _velocity, _life, _r, _g, _b) {
      target = _target;
      size = 0.3;
   }

bool TractorAttractionParticle::step(double timeDifference) {
   const float velocityScalePerSecond = 2;
   Vector3D toTarget(*position, *target);
   toTarget.setLength(10);
   toTarget.scalarMultiplyUpdate(timeDifference);
   velocity->addUpdate(toTarget);
   return Particle::step(timeDifference);
}

void TractorAttractionParticle::Add(Point3D* pos, Vector3D* vec, Point3D* _target) {
   if (particles.size() >= MAX_PARTICLES) {
      std::cout << "max particles reached!" << std::endl;
      delete pos;
      delete vec;
      return;
   }
   const float minLife = 0.5; // Seconds

   float _fade = (0.5 * randdouble()) + minLife;
   float _r = 0.9 + (randdouble() * 0.1);
   float _g = 0.9 + (randdouble() * 0.1);
   float _b = 0;

   particles.push_back(new TractorAttractionParticle(pos, vec, _fade, _r, _g, _b, _target));
}
