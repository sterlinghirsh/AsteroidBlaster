/**
 * Particles that come off shard orbiters.
 * @author Sterling Hirsh
 * @date 3/9/11
 */

#ifndef __SHARDPARTICLE_H__
#define __SHARDPARTICLE_H__

#include "Particles/Particle.h"

class ShardParticle : public Particle  {
   public :
      ShardParticle(Point3D* p, Vector3D* v, float life, float r, float g, float b, const GameState*);
      virtual void update(double timeDifference);
      static void Add(Point3D* pos, Vector3D* vec, const GameState*);
};

#endif


