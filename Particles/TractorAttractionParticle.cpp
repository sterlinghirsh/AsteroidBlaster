/**
 * Particles that come off shards during attraction.
 * @author Sterling Hirsh
 * @date 3/9/11
 */

#include "Particles/TractorAttractionParticle.h"
#include "math.h"

const float startingSize = 0.3f;
const float minLife = 0.5f; // Seconds
TractorAttractionParticle::TractorAttractionParticle(Point3D* _position,
      Vector3D* _velocity, float _life, float _r, float _g, float _b, Point3D* _target) :
   Particle(_position, _velocity, _life, _r, _g, _b) {
      target = _target;
      size = startingSize;
   }

void TractorAttractionParticle::update(double timeDifference) {
   // Make these always fly at the target.
   velocity->updateMagnitude(*position, *target);
   if (velocity->getComparisonLength() < 4) {
      // If the particle is within 2 units of the ship, have it die.
      life = 0;
   } else {
      velocity->setLength(30); // Constant speed.
   }
   // Do the parent's update.
   Particle::update(timeDifference);
}

void TractorAttractionParticle::Add(Point3D* pos, Vector3D* vec, Point3D* _target) {
   float _fade = (float) ((0.5 * randdouble()) + minLife);
   float _r = (float) (0.9 + (randdouble() * 0.1));
   float _g = (float) (0.9 + (randdouble() * 0.1));
   float _b = 0;

   Particle::Add(new TractorAttractionParticle(pos, vec, _fade, _r, _g, _b, _target));
}
