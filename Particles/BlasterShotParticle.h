/**
 * Particles that come off the blaster shots.
 * @author Sterling Hirsh
 * @date 2/17/11
 */

#ifndef __BLASTERSHOTPARTICLE_H__
#define __BLASTERSHOTPARTICLE_H__

#include "Particles/Particle.h"

class BlasterShotParticle : public Particle  {
   public :
      BlasterShotParticle(Point3D* p, Vector3D* v, float life, float r, float g, float b, const GameState* _gameState);
      void update(double timeDifference);
      static void Add(Point3D* pos, Vector3D* vec, const GameState* _gameState);
      static void AddRainbow(Point3D* pos, Vector3D* vec, int particleNum, int particleCycle, const GameState* _gameState);
};

#endif

