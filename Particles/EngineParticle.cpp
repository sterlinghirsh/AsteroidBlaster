/**
 * Particles from the engine of the ship.
 */

#include "Particles/EngineParticle.h"

const float startingSize = 0.6f;
const float minLife = 6; // Seconds

EngineParticle::EngineParticle(Point3D* p, Vector3D* v, float life, float r, float g, float b, const GameState* _gameState) : Particle(p, v, life, r, g, b, _gameState) {
   // Nothing special :/
   size = startingSize;
};

/**
 */
void EngineParticle::Add(Point3D* pos, Vector3D* vec, double color, const GameState* _gameState) {
   float _fade = (float) randdouble() + minLife;
   float _r = 1.0f;
   float _g = 0;
   float _b = 0;

   getBrightColor(color, _r, _g, _b);

   const float dimFactor = 10.0;
   _r = _r / dimFactor;
   _g = _g / dimFactor;
   _b = _b / dimFactor;

   Particle::Add(new EngineParticle(pos, vec, _fade, _r, _g, _b, _gameState));
}

void EngineParticle::update(double timeDifference) {
   double velocityReductionPerSecond = 20.0 * randdouble();
   double lengthSquared = velocity->getComparisonLength();

   velocity->scalarMultiplyUpdate(1 - timeDifference);

   // Do the parent's update.
   Particle::update(timeDifference);
}
