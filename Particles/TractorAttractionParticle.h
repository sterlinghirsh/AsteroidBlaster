/**
 * Particles that come off shards during attraction.
 * @author Sterling Hirsh
 * @date 3/9/11
 */

#ifndef __TRACTORATTRACTIONPARTICLE_H__
#define __TRACTORATTRACTIONPARTICLE_H__

#include "Particles/Particle.h"

class TractorAttractionParticle : public Particle  {
   public :
      TractorAttractionParticle(Point3D* p, Vector3D* v, float life, float r, float g, float b, Point3D* _target, const GameState* _gameState);
      static void Add(Point3D* pos, Vector3D* vec, Point3D* _target, const GameState* _gameState);
      virtual void update(double timeDifference);
      Point3D* target;
};

#endif


