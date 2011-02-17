/**
 * Particles that come off the blaster shots.
 * @author Sterling Hirsh
 * @date 2/17/11
 */

#ifndef __BLASTERSHOTPARTICLE_H__
#define __BLASTERSHOTPARTICLE_H__

#include "Graphics/Particle.h"

class BlasterShotParticle : public Particle  {
   public :
      BlasterShotParticle(Point3D* p, Vector3D* v, float life, float fade, float r, float g, float b);
      bool step(double timeDifference);
      static void Add(Point3D* pos, Vector3D* vec);
};

#endif

