/** * AsteroidShip.h
 * Sterling Hirsh
 * This is the ship that a player plays from or can see.
 */

#ifndef __ASTEROIDSHIP_H__
#define __ASTEROIDSHIP_H__

//#include "Utility/Quaternion.h"
#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"
#include "Shots/Shot.h"
#include "Utility/GlobalUtility.h"
#include "Items/Object3D.h"
#include "Items/Ball.h"
#include "Utility/Radar.h"
#include "AI/ShootingAI.h"
#include "AI/FlyingAI.h"
#include "Weapons/Weapon.h"
#include "Weapons/Blaster.h"
#include "Weapons/RailGun.h"
#include "Weapons/TractorBeam.h"
#include "Weapons/Electricity.h"
#include "Weapons/LawnMower.h"
#include "Weapons/Ram.h"
#include "Weapons/AntiInertia.h"
#include "Weapons/TimedBomber.h"
#include "Weapons/Energy.h"
#include "Weapons/RemoteBomber.h"
#include <list>

// Later we'll put in a model for the ship.

extern GLfloat headlight_pos[4];
extern GLfloat headlight_amb[4];
extern GLfloat headlight_diff[4];
extern GLfloat headlight_spec[4];

// Incomplete declaration so we can use the pointer.
class Shot;
class Radar;
class ShootingAI;
class FlyingAI;

class AsteroidShip : public Object3D {
   public:
      CollisionSphere* collisionSphere;
      AsteroidShip(const GameState* _gameState);
      virtual ~AsteroidShip();
      double getHealth();
      int getScore();
      int getShards();
      double getAimX();
      double getAimY();
      void updatePlayerAcceleration();
      virtual void update(double timeDiff);
      void rotate();
      void nextWeapon();
      void prevWeapon();
      // Return a reference to the list of weapons that the ship has.
      std::vector<Weapon*> getWeaponsList();
      // Get the number of types of weapons the ship has. They're indexed 0 - (n-1)
      int getNumWeapons();

      // We'll have to update this later so the AI can use it.
      void updateShotDirection(double xOffset, double yOffset);
      void updateShotDirection(Vector3D dir);
      void updateShotDirection(Point3D dir);
      void keepFiring();
      virtual void draw();
      virtual void drawGlow() { draw(); };
      virtual void drawInMinimap();
      virtual void debug();

      float getShakeAmount();
      void setShakeAmount(float shakeIn);

      void drawShotDirectionIndicators();
      void drawCrosshair();
      Vector3D* getViewVector();
      Vector3D* getCameraOffset();
      void nextView();

      void reInitialize();
      
      int healthMaxUpgradePrice() { return ((int) healthMax/100)*healthUpgradePrice;}

      /**
       * These are going to be the functions that either the local player
       * or the AI player or the networked player will call.
       * Essentially, this is the ship's API.
       *
       * We give accelerateForward, Right, Up either -1, 0, or 1 to mean
       * backward, none, or forward. You can either fully accelerate or not.
       * Rotation is the opposite. You supply a value, this clamps it and sets it.
       * setBrake accepts a bool. True starts braking, false stops braking.
       * fire works the same as setBrake.
       * selectWeapon selects the weapon corresponding to the int weaponType.
       * We will need to prevent weapons that the player does not own from getting selected.
       */
      void accelerateForward(int direction);
      void accelerateRight(int direction);
      void accelerateUp(int direction);
      void setBrake(bool doBrake);
      void setBoost(bool doBoost);
      void setYawSpeed(double yawAmountIn);
      void setRollSpeed(double rollAmount);
      void setPitchSpeed(double pitchAmount);
      void fire(bool startFiring);
      void selectWeapon(int weaponType);

      // Change this to something reasonable once we have a radar.
      Radar *getRadar();
      // void getAllVisibleAsteroids();
      // void setShootingAI(bool ai);

      // A ship has a single flying AI
      // TODO: A ship has a list of shooting AIs rather than just one
      ShootingAI* shooter;
      FlyingAI* flyingAI;
      
      void setView(int _view);
      
      int score;
      int nShards;
      int engineUpgrade;
      int engineMax;
      int enginePrice;
      double health;
      double healthMax;
      int healthUpgradePrice;
      int healthPrice;
      int healthAmount;
      int healthUpgradeAmount;
      double shotOriginScale;
      double spin;
      double flashiness;
      double tracker;
      int rando;
      double x2Change;
      double y2Change;
      double z2Change;
      double xChange;
      double yChange;
      double zChange;
      double backChange;
      double lineMove;
      double zMove;
      double justGotHit;
      
      bool drawHit;
      
      Point3D shotOrigin;
      
      Vector3D shotDirection; // If we shoot a shot, where will it go?
      Weapon* getCurrentWeapon();
      /*
       * Just tell us what the next weapon up would be, but don't switch to it.
       */
      Weapon* getNextWeapon();
      /*
       * Just tell us what the previous weapon would be, but don't switch to it.
       */
      Weapon* getPreviousWeapon();
      float getCurrentWeaponCoolDown();
      Vector3D* getShotDirection();
      Weapon* getWeapon(int wep);
      
      std::vector<Weapon*> getWeapons(){ return weapons; }
      
      int getCurrentView();

      void setCameraDirectly();
      void setZoomSpeed(float speed);
      
      void setOrientation(double forwardX, double forwardY, double forwardZ,
 double upX, double upY, double upZ);
      void setOrientation(Vector3D& _forward, Vector3D& _up);
      void lookAt(double lookAtX, double lookAtY, double lookAtZ, double upX, double upY, double upZ);

      std::vector<Weapon*> weapons;

      // Maybe these shouldn't be here?
      bool isFiring;
      int currentWeapon;
      float shakeAmount;
   protected:
      int soundHandle;
      Radar* radar; // This is the ship's radar that it should query for information about its surroundings

      double brakeFactor;
      //NOT USED
      //double shipRadius; // Units, this is the distance from the center to assume that we're hitting something.

      // These could get changed with upgrades.
      double maxForwardAccel;
      double maxUpAccel;
      double maxRightAccel;
      double maxBoostForwardAccel;
      double maxBoostUpAccel;
      double maxBoostRightAccel;
      double maxYawSpeed;
      double maxPitchSpeed;
      double maxRollSpeed;

      double shotPhi; // Radians
      double shotTheta; // Radians

      double curForwardAccel; // Units per second^2
      double curRightAccel;
      double curUpAccel;

      double yawSpeed; // Rad/s
      double pitchSpeed;
      double rollSpeed;

      double targetYawSpeed;
      double targetPitchSpeed;
      double targetRollSpeed;

      double maxSpeed; // Units per second
      double maxBoostSpeed; // Units per second
      double frontX;
      double frontY;
      double frontZ;
      double cornerX;
      double cornerY;
      double cornerZ;
      double middleXY;
      double middleZ;
      double backX;
      double backY;
      double backZ;
      double skew;
      
      double hitX;
      double hitY;
      double hitZ;

      bool isBraking;
      bool isBoosting;

      float zoomFactor;
      float zoomSpeed;

      Vector3D *cameraOffset;

      int currentView; // Defined by view names above.

      void updateShotDirectionVector();
      void brake(double brakeFactorIn);
      void forwardAcceleration(double newAcc);
      void rightAcceleration(double newAcc);
      void upAcceleration(double newAcc);
      void draw_ship();
      void draw_frontpanels();
      void draw_backpanels();
      void draw_spaceboner();
      void draw_bonerlines();
      void draw_frontlines();
      void draw_backlines();
      void draw_hitEffect();

      double aimX, aimY;
      
      void createEngineParticles(double timeDiff);
      void addNewParticle(Point3D& emitter, Vector3D& initialOffset, Vector3D& offsetDirectionX,
       Vector3D& offsetDirectionY, int color = 1);
      double accelerationStartTime;
      int particlesEmitted;

      Timer respawnTimer;
};

#endif
