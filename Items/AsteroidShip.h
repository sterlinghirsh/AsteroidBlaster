/** * AsteroidShip.h
 * Sterling Hirsh
 * This is the ship that a player plays from or can see.
 */

#ifndef __ASTEROIDSHIP_H__
#define __ASTEROIDSHIP_H__

//#include "Utility/Quaternion.h"
#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"
#include "Utility/Radar.h"
#include "Utility/GlobalUtility.h"
#include "Utility/Timer.h"

#include "Shots/Shot.h"


#include "Items/Object3D.h"
#include "Items/Ball.h"

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
#include "Weapons/HomingMissile.h"

#include <list>

#define DAMAGER_INDEX_ASTEROID -1

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
class ClientCommand;

class AsteroidShip : public Object3D {
   //public variables------------------------------
   public:
      int score;
      int kills;
      int deaths;
      int life;

      // Banking
      int nShards; // Total number of shards the ship has.
      int bankedShards; // Shards that were collected this round.
      // Shards that were collected in previous rounds.
      // These will be banked over time.
      int unbankedShards; 
      int totalBankedShards;
      double bankPeriod; // Time it takes for a single shard to bank.
      Timer bankTimer; // This is set if there are any shards unbanked

      // The current level of the engine.
      int engineUpgrade;
      // The maximum upgrade level of the engine.
      int engineMax;
      // The price increment per upgrade.
      int enginePrice;
      double health;
      double healthMax;
      int healthUpgradePrice;
      int healthPrice;
      int healthAmount;
      int healthUpgradeAmount;
      int regenHealthLevel;
      int regenHealthUpgradePrice;
      int regenHealthLevelMax;
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
      double invulnerableTime;
      double timeLeftToRespawn;
      bool spawnInvulnerable;
      bool drawSpawn;
      CollisionSphere* collisionSphere;

      Object3D* lastDamager;
      int lastDamagerWeapon;
      
      float color1;
      float color2;

      // A ship has one FlyingAI, and one ShootingAI.
      ShootingAI* shooter;
      FlyingAI* flyingAI;
      
      bool drawHit;
      Point3D shotOrigin; // If we shoot a shot, where will it originate from?
      Vector3D shotDirection; // If we shoot a shot, where will it go?
      
      std::vector<Weapon*> weapons; // List of weapons this ship owns
      
      // Maybe these shouldn't be here?
      bool isFiring;
      int currentWeapon;
      float shakeAmount;
      
      double isBarrelRollingLeft;
      double isBarrelRollingRight;
      
      
   //private variables------------------------------
   private:
   
   
   //public functions------------------------------
   public:
      //constructor
      AsteroidShip(const GameState* _gameState);
      //destructor
      virtual ~AsteroidShip();
      
      // Virtual functions from Object3D and Drawable class
      virtual void draw();
      virtual void update(double timeDiff);
      virtual void drawGlow() { draw(); };
      virtual void drawInMinimap();
      virtual void debug();
      virtual std::string serialize();
      
      bool isRespawning();
      
      double getHealth();
      int getScore();
      int getShards();
      double getAimX();
      double getAimY();
      void updatePlayerAcceleration();
      void rotate();
      
      // We'll have to update this later so the AI can use it.
      void updateShotDirection(double xOffset, double yOffset);
      void updateShotDirection(Vector3D& dir);
      void keepFiring();
      // Change this to something reasonable once we have a radar.
      Radar *getRadar();

      float getShakeAmount();
      void setShakeAmount(float shakeIn);
      double getAliveTime();

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
      virtual void setYawSpeed(double yawAmountIn);
      virtual void setRollSpeed(double rollAmount);
      virtual void setPitchSpeed(double pitchAmount);
      void fire(bool startFiring);
      void selectWeapon(int weaponType);
      
      // Camera and view related functions
      void setView(int _view);
      int getCurrentView();
      void setCameraToShotOrigin();
      void setZoomSpeed(float speed);
      void setOrientation(double forwardX, double forwardY, double forwardZ, double upX, double upY, double upZ);
      void setOrientation(Vector3D& _forward, Vector3D& _up);
      void lookAt(double lookAtX, double lookAtY, double lookAtZ, double upX, double upY, double upZ);
      
      
      // Weapon related functions
      void nextWeapon();
      void prevWeapon();
      int getNextWeaponID();
      int getPrevWeaponID();
      // Return a reference to the list of weapons that the ship has.
      // Get the number of types of weapons the ship has. They're indexed 0 - (n-1)
      int getNumWeapons();
      Weapon* getCurrentWeapon();
      Weapon* getNextWeapon(); //Just tell us what the next weapon up would be, but don't switch to it.
      Weapon* getPreviousWeapon(); //Just tell us what the previous weapon would be, but don't switch to it.
      float getCurrentWeaponCoolDown();
      Vector3D* getShotDirection();
      Weapon* getWeapon(int wep);
      std::vector<Weapon*> getWeapons(){ return weapons; }

      void readCommand(ClientCommand& command);

      void atLevelEnd();
      
   //private functions------------------------------
   private:
   
   //protected variables------------------------------
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
      bool isFirstSpawn;

      float zoomFactor;
      float zoomSpeed;

      Vector3D *cameraOffset;

      int currentView; // Defined by view names above.
      
      double aimX, aimY;
      
      double accelerationStartTime;
      int particlesEmitted;

      Timer respawnTimer;
      Timer aliveTimer;
      
      
   //protected functions------------------------------
   protected:
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
      void draw_spawn();
      void stopSounds();

      void createEngineParticles(double timeDiff);
      void createLowHealthParticles(double timeDiff);
      void addNewLowHealthParticle(Point3D& emitter, Vector3D& initialOffset, Vector3D& offsetDirectionX,
       Vector3D& offsetDirectionY, double color = 1.0);
      void addNewParticle(Point3D& emitter, Vector3D& initialOffset, Vector3D& offsetDirectionX,
       Vector3D& offsetDirectionY, double color = 1.0);
      Shard* makeShard();

};

#endif
