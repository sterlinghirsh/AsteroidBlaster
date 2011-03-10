/**
 * Particles that come off the blaster shots.
 * @author Sterling Hirsh
 * @date 2/17/11
 */

#ifndef __BLASTERIMPACTPARTICLE_H__
#define __BLASTERIMPACTPARTICLE_H__

#include "Particles/Particle.h"

class BlasterImpactParticle : public Particle  {
   public :
      BlasterImpactParticle(Point3D* p, Vector3D* v, float life, float r, float g, float b);
      bool step(double timeDifference);
      static void Add(Point3D* pos, Vector3D* vec);
      static void AddRainbow(Point3D* pos, Vector3D* vec, int particleNum, int particleCycle);
};

#endif

