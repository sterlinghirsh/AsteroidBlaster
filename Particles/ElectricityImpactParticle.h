/**
 * Particles that come off the electricity impacts.
 * @author Sterling Hirsh
 * @date 3/9/11
 */

#ifndef __ELECTRICITYIMPACTPARTICLE_H__
#define __ELECTRICITYIMPACTPARTICLE_H__

#include "Particles/Particle.h"

class ElectricityImpactParticle : public Particle  {
   public :
      ElectricityImpactParticle(Point3D* p, Vector3D* v, float life, float r, float g, float b);
      virtual void update(double timeDifference);
      static void Add(Point3D* pos, Vector3D* vec);
};

#endif


