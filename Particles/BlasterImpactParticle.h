/**
 * Particles that come off the blaster impacts.
 * @author Sterling Hirsh
 * @date 3/9/11
 */

#ifndef __BLASTERIMPACTPARTICLE_H__
#define __BLASTERIMPACTPARTICLE_H__

#include "Particles/Particle.h"

class BlasterImpactParticle : public Particle  {
   public :
      BlasterImpactParticle(Point3D* p, Vector3D* v, float life, float r, float g, float b);
      virtual bool step(double timeDifference);
      static void Add(Point3D* pos, Vector3D* vec);
};

#endif

