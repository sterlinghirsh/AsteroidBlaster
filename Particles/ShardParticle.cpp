/**
 * Particles that come off the shard orbiters.
 * @author Sterling Hirsh
 * @date 3/14/11
 */

#include "Particles/ShardParticle.h"
#include "math.h"

const float startingSize = 0.3f;
const float minLife = 0.5f; // Seconds
ShardParticle::ShardParticle(Point3D* _position,
      Vector3D* _velocity, float _life, float _r, float _g, float _b) :
   Particle(_position, _velocity, _life, _r, _g, _b) {
      size = startingSize;
   }

void ShardParticle::update(double timeDifference) {
   size -= timeDifference * startingSize / life;
   // Do the parent's update.
   Particle::update(timeDifference);
}

void ShardParticle::Add(Point3D* pos, Vector3D* vec) {
   float _fade = minLife;
   float _r = (float) (0.9 + (randdouble() * 0.1));
   float _g = (float) (0.9 + (randdouble() * 0.1));
   float _b = (float) (0.9 + (randdouble() * 0.1));

   particles.push_back(new ShardParticle(pos, vec, _fade, _r, _g, _b));
}

