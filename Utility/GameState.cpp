/**
 * GameState - Stores the state of the game.
 * Lists of objects, etc.
 * @author Sterling Hirsh
 * @date 1-23-11
 * CPE 476
 */

#include "Utility/GameState.h"

#include "Utility/Matrix4.h"
#include "Utility/SoundEffect.h"
#include "Utility/Collision.h"
#include "Utility/GlobalUtility.h"

#include "Graphics/Texture.h"
#include "Graphics/Skybox.h"
#include "Graphics/Sprite.h"
#include "Graphics/Camera.h"

#include "Particles/Particle.h"

#include "Text/Text.h"
#include "Text/Input.h"
#include "Text/GameMessage.h"

#include "Items/AsteroidShip.h"
#include "Items/BoundingSpace.h"
#include "Items/Asteroid3D.h"
#include "Items/Spring.h"

#include "Menus/MainMenu.h"
#include "Menus/StoreMenu.h"

#include "HUD/ProgressBar.h"
#include "HUD/WeaponDisplay.h"
#include "HUD/Minimap.h"
#include "HUD/Screen.h"

#include "Network/gamestate.pb.h"
#include "Network/ServerSide.h"
#include "Network/ClientSide.h"
#include "Network/ClientInfo.h"

#include <utility>

#include <math.h>
#include <stdio.h>
#include <fstream>
#include <sstream>

#define SAVEFILENAME "AsteroidBlaster.sav"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

// DEBUG
extern std::ofstream debugoutput;


extern double minimapSizeFactor;

std::ostringstream sstream2; // TODO: Better name plz.
ast::ClientCommand localClientCommand;

GameState::GameState(GameStateMode _gsm) :
   custodian(this),
   clientCommand(localClientCommand),
   gsm(_gsm),
   levelTimer(this),
   gameOverTimer(this) {
      initialize();
   }

/**
 * Deconstructor: clean up all of our objects.
 */
GameState::~GameState() {
   destruct();
}

void GameState::destruct() {
   delete skybox;
   delete ship;
   delete shipCamera;
   delete spectatorCamera;
   delete spring;
   delete cube;
   delete shardBankBar;

   if (serverSide != NULL)
      delete serverSide;

   if (clientSide != NULL)
      delete clientSide;
   // Somehow makes it segfault when quitting so commented out
   /*if (io != NULL) {
     io->stop();
     delete io;
     }*/
}

void GameState::initialize() {
      curGameStateId = 0;
      lastReceivedGameStateId = 0;
      ship = NULL;

      resetClientCommand(true);
      clientCommand.set_curweapon(BLASTER_WEAPON_INDEX);
      serverSide = NULL;
      clientSide = NULL;

      shipCamera = NULL;
      minimap = NULL;

      if (gsm == SingleMode) {
      } else if (gsm == MenuMode) {
      } else if (gsm == ClientMode) {
         clientSide = new ClientSide(this);
      } else if (gsm == ServerMode) {
         serverSide = new ServerSide(this);
      }

      gameTime = 0;
      levelStartTime = 0;

      godMode = false;
      gameIsRunning = true;
      levelOver = false;
      if (gsm == ServerMode || gsm == MenuMode) {
         usingShipCamera = false;
      } else {
         usingShipCamera = true;
      }

      /* A view frustum culled list of objects to be used for drawing and by
         the shooting AI.
         */
      viewFrustumObjects = NULL;
      targetableViewFrustumObjects = NULL;

      worldSize = WORLD_SIZE;
      skybox = new Skybox();

      spring = new Spring(this);
      if (gsm == SingleMode)
         setShip(new AsteroidShip(this));

      spectatorCamera = new Camera(false);
      //make it look at the center of the map for spectator mode
      spectatorCamera->lookAt(0.0, 0.0, 0.0);
      spectatorSpeed = 0.2;
      spectatorRadius = 120.0;


      cube = new BoundingSpace(worldSize / 2, 0, 0, 0, this);
      //sphere = new BoundingSphere(worldSize, 0, 0, 0);
      // Set up our text objects to be displayed on screen.
      curFPS = 0;

      // Init Text Objects
      SDL_Rect position = {0,0};
      std::string fontName = DEFAULT_FONT;

      //new all the text class we will be using
      FPSText = new Text("FPS: ", curFPS, "",  hudFont, position);
      scoreText = new Text("Score: ", 0, "",  hudFont, position);
      bankedShardText = new Text("Banked Shards: ", 0, "",  hudFont, position);
      unbankedShardText = new Text("Unbanked Shards: ", 0, "",  hudFont, position);
      timerText = new Text("", sstream2.str(), "", hudFont, position);
      timerText->alignment = RIGHT_ALIGN;
      curLevelText = new Text("Level: ", curLevel, "",  hudFont, position);
      curLevelText->alignment = RIGHT_ALIGN;
      lifeText = new Text("Lives: ", 3, "",  hudFont, position);
      lifeText->alignment = RIGHT_ALIGN;

      // Clear the sstream2
      sstream2.str("");

      // Improve the positioning code.

      shardBankBar = new ProgressBar(0.06f, 0.4f, p2wx(10), p2wy(10));
      shardBankBar->setHorizontal(true);

      float healthSpan = (float)gameSettings->GW / (float)gameSettings->GH;

      const float healthHeight = 0.10f;
      const float weaponBarHeight = 0.3f;
      healthBar = new ProgressBar(healthHeight, healthSpan, 0.0f, 0.95f - (healthHeight * 0.5f));
      healthBar->setHorizontal(true);
      healthBar->setSkew(0.0, 0.05f);
      healthBar->setCentered(true);
      
      weaponBar = new WeaponDisplay(weaponBarHeight, weaponBarHeight, 0.0f, -0.9f + (weaponBarHeight * 0.5f), this);

      // Start off at level 1.
      curLevel = 1;
     
      // Spawn one more asteroid for each level, to a point..
      numAsteroidsToSpawn = decideNumAsteroidsToSpawn();

      if (gsm != SingleMode) {
         custodian.update();
      } else {
         // For single.
         // Set up objects.
         custodian.add(ship);
         // It crashes without this. :/
         initAsteroids();
      }


      doYaw = false;
      mouseX = 0;
      mouseY = 0;

      // Make a good formula here for how many seconds a level should last.
      levelDuration = 120;

      godMode = false;

      // TODO: comment this or rename it.
      isW = isA = isS = isD = false;
      isI = isJ = isK = isL = false;

      drawGraphics = true;
   
}

/**
 * The game should run for levelDuration # of seconds.
 */
void GameState::setLevelTimer() {
   levelTimer.setCountDown(levelDuration);
   // levelTimer is implicltly now 'active'.
}

/**
 * Pause the levelTimer. It should remain at its current time left when it
 * is resumed again in the future.
 */
void GameState::pauseLevelTimer() {
   levelTimer.pause();
}

/**
 * Resume the levelTimer.
 */
void GameState::resumeLevelTimer() {
   levelTimer.resume();
}

void GameState::addAIPlayer() {
   static int nextName = 0;

   int numNames = 7;
   std::string names[] = {
      "CPU1",
      "CPU2",
      "CPU3",
      "CPU4",
      "CPU5",
      "CPU6",
      "CPU7"
   };

   AsteroidShip* otherShip = new AsteroidShip(this);
   double randX = (randdouble())*(worldSize / 2);
   double randY = (randdouble())*(worldSize / 2);
   double randZ = (randdouble())*(worldSize / 2);
   otherShip->position->update(randX, randY, randZ);
   otherShip->flyingAI->enable();
   otherShip->shooter->enable();
   otherShip->lives = 1;
   otherShip->name = names[nextName++ % numNames];
   custodian.add(otherShip);
}

unsigned GameState::addNetworkPlayer(unsigned clientID) {
   AsteroidShip* otherShip = new AsteroidShip(this);
   otherShip->randomizePosition();
   custodian.add(otherShip);
   custodian.shipsByClientID.insert(std::pair<unsigned, AsteroidShip*>(clientID, otherShip));
   return otherShip->id;
}

/**
 * Initialize the displays once the textures have been loaded.
 */
void GameState::addScreens() {
   printf("screens added\n");
   screens.push_back(Screen(0, 0, Texture::getTexture("fboTex")));
   screens.push_back(Screen(0, 1, Texture::getTexture("hblurTex")));
   screens.push_back(Screen(0, 2, Texture::getTexture("bloomTex")));
   screens.push_back(Screen(1, 0, Texture::getTexture("albedoTex")));
   screens.push_back(Screen(1, 1, Texture::getTexture("normalTex")));
   screens.push_back(Screen(1, 2, Texture::getTexture("noLightTex")));
   screens.push_back(Screen(2, 0, Texture::getTexture("overlayTex")));
   screens.push_back(Screen(2, 1, Texture::getTexture("hudTex")));
   screens.push_back(Screen(2, 2, Texture::getTexture("depthTex")));
}

/**
 * This is the step function.
 */
void GameState::update(double timeDiff) {
   //double oldGameTime = gameTime;
   double newLocalGameTime = gameTime + timeDiff;
   std::set< std::pair<unsigned, unsigned> >* recordedCollisions = NULL;
   if (gsm == ServerMode) {
      recordedCollisions = new std::set< std::pair<unsigned, unsigned> >();
   }

   setMusic();

   //updateGameTime(timeDiff); // Sets gameTime to newLocalTime.
   curGameStateId++;
   if (gsm != ServerMode) {
      // Cleanup here to make sure we're not depending on anything (hopefully.)
      // Don't cleanup if on server side.
      custodian.cleanup();
   }

   if (gsm == ClientMode) {
      clientSide->receive();
      clientSide->send(clientCommand, false);
   }
   
   custodian.update();
   if (ship != NULL &&
    (gsm == ClientMode || gsm == SingleMode) && 
    !ship->flyingAI->isEnabled() && !ship->shooter->isEnabled()) {
      ship->readCommand(clientCommand);
   }
   
   if (!justLoadedFirstFrame) {
      const double timeCorrectionAmount = 0.001; // seconds
      double networkGameTime = gameTime;

      if (newLocalGameTime > networkGameTime + timeCorrectionAmount * 3) {
         newLocalGameTime -= timeCorrectionAmount;
      } else if (newLocalGameTime < networkGameTime - timeCorrectionAmount * 3) {
         newLocalGameTime += timeCorrectionAmount * 2;
      }

      // This advances the stuff.
      debugoutput << "diff: " << newLocalGameTime - gameTime << std::endl;
      advancePhysics(gameTime, newLocalGameTime, recordedCollisions);
   }
   

   // if the game over timer is set, and is over
   if (gameOverTimer.isRunning && gameOverTimer.getTimeLeft() < 0) {
      mainMenu->firstTime = true;
      mainMenu->activate();
      gameOverTimer.reset();
      custodian.clear();
      Particle::Clear();
      MeshFace::Clear();
      custodian.update();
      GameMessage::Clear();
   }

   // if it's not in MenuMode
   if (gsm != MenuMode) {
      // If the level should be over, let's go to the store menu.
      if(!gameOverTimer.isRunning && ((levelTimer.isRunning && levelTimer.getTimeLeft() <= 0) ||
            (gameIsRunning &&
             (gsm == SingleMode) &&
             custodian.asteroidCount == 0 &&
             custodian.shardCount == 0)) ) {
         nextLevel();
         return;
      }

      // the last 6 seconds of the stage
      if (levelTimer.getTimeLeft() <= 6 && levelTimer.getTimeLeft() > 0 && !gameOverTimer.isRunning) {
         // Check if it is done waiting until going to the next level
         std::ostringstream gameMsg;
         if (gsm == SingleMode) {
            gameMsg << "Entering store in " << (int)levelTimer.getTimeLeft() << "...";
         } else {
            gameMsg << "Next level in " << (int)levelTimer.getTimeLeft() << "...";
         }
         GameMessage::Add(gameMsg.str(), 30, 0, this);
      }
   }


   // Update before proceeding.
   // TODO: Do we need to do this here?
   custodian.update();
   // Used to cleanup here, but that broke collisions.
   
   Particle::updateParticles(timeDiff);
   //Sprite::updateSprites(timeDiff);
   MeshFace::updateIndependentFaces(timeDiff);
   GameMessage::updateAllMessages(timeDiff);
   
   ast::CollisionMessage* curFrameCollisions = NULL;
   if (gsm == ServerMode) {
      curFrameCollisions = new ast::CollisionMessage();
      curFrameCollisions->set_gamestateid(curGameStateId);
      savedCollisionMessages[curGameStateId] = curFrameCollisions;
      std::set< std::pair<unsigned, unsigned> >::iterator collisionIter;
            
      for (collisionIter = recordedCollisions->begin(); collisionIter !=
       recordedCollisions->end(); collisionIter++) {
         // Every 2 is a collision.
         curFrameCollisions->add_collider((*collisionIter).first);
         curFrameCollisions->add_collider((*collisionIter).second);
      }
   }

   // Check every object for collisions.
   if (gsm == ClientMode) {
      // Read collisions and handle them.
      std::map<unsigned, ast::CollisionMessage*>::iterator collisionIter;
      collisionIter = savedCollisionMessages.begin();
      CollisionBase* collision;
      unsigned aId, bId;
      for (; collisionIter != savedCollisionMessages.end(); ++collisionIter) {
         // Go by 2s.
         for (int i = 0; i < collisionIter->second->collider_size(); i += 2) {
            aId = collisionIter->second->collider(i);
            bId = collisionIter->second->collider(i + 1);
            // DEBUG
            // printf("aid: %d, bid: %d\n", aId, bId);
            collision = custodian.getCollision(custodian[aId], custodian[bId]);
            if (collision != NULL) {
               collision->handleCollision();
               delete collision;
            } else {
               // DEBUG
               printf("Null collision.\n");
            }
         }
         savedCollisionMessages.erase(collisionIter);
      }
   }

   // TODO: Add code to handle sound on client side, when ship is null.
   if (ship != NULL && shipCamera != NULL) {
      SoundEffect::updatePositions(shipCamera->position,
       ship->velocity, shipCamera->forward, shipCamera->up);

      shardBankBar->setAmount(ship->bankTimer.isRunning ? ship->bankTimer.getAmountComplete() : 0);
      healthBar->setAmount(((float) ship->health / (float) ship->healthMax));

      if (gameSettings->enableMinimap) {
         minimap->update(timeDiff);
      }
   } else {
      Vector3D up(0, 1, 0);
      Vector3D center(0, 0, 0);
      Vector3D fwd(spectatorCamera->position, &center);
      fwd.normalize();
      Vector3D velocity(up.cross(fwd).scalarMultiply(20));
      SoundEffect::updatePositions(spectatorCamera->position, &velocity,
       &fwd, &up);
   }

   if (gsm == ServerMode) {
      // Send the gamestate to everyone.
      unsigned gameStateId = storeFullGameState();
      std::map<unsigned, ClientInfo*>& clients = serverSide->getClients();
      std::map<unsigned, ClientInfo*>::iterator clientIter = clients.begin();

      ast::Frame frame;
      bool sendReliable;

      for (; clientIter != clients.end(); clientIter++) {
         sendReliable = false;
         if (clientIter->second == NULL) {
            clients.erase(clientIter);
         } else if (!clientIter->second->connected) {
            delete clientIter->second;
            clients.erase(clientIter);
         } else {
            ClientInfo* client = clientIter->second;

            if (client->ackGameState == 0) {
               // Send the initial update.
               sendReliable = true;
               // No diff.
               frame.set_shipid(client->shipid);
               frame.mutable_gamestate()->CopyFrom(*(savedGameStates[gameStateId]));
            } else {
               saveDiff(*(savedGameStates[client->ackGameState]), frame.mutable_gamestate());
               
               unsigned i;
               for (i = client->ackGameState + 1; i <= gameStateId; ++i) {
                  if (savedCollisionMessages[i] != NULL)
                     frame.add_collision_message()->CopyFrom(*(savedCollisionMessages[i]));
               }
            }

            serverSide->send(client->client, frame, sendReliable);
            
            frame.Clear();
         }
      }

      serverSide->receive();
      clearOldSavedGameStates();
   }

   updateText();
   cube->update(timeDiff);
   spectatorCameraUpdate(timeDiff);
   ++curFrame;
   justLoadedFirstFrame = false;
}

void GameState::networkUpdate(double timeDiff) {
   
}

void GameState::spectatorCameraUpdate(double timeDiff) {
   double pieCounter = fmod(M_PI * 0.75 + (getGameTime() * spectatorSpeed), 2 * M_PI);
   spectatorCamera->position->x = sin(pieCounter)*spectatorRadius;
   spectatorCamera->position->y = sin(pieCounter)*(spectatorRadius/3);
   spectatorCamera->position->z = -cos(pieCounter)*spectatorRadius;
   spectatorCamera->lookAt(0.0,0.0,0.0);
}

/**
 * Draw objects in the minimap.
 */
void GameState::drawMinimap() {
   if ((gsm != MenuMode) && !ship->isRespawning())
      minimap->draw();
}

/**
 * Draws all Displays.
 */
void GameState::drawScreens() {
   if (!showBloomScreen) { return; }
   for (unsigned i = 0; i < screens.size(); i++) {
      screens[i].draw();
   }
}

/**
 * Returns whether the minimap is on.
 */
bool GameState::minimapOn() {
   return minimap->isEnabled();
}

/**
 * Toggles the minimap on or off.
 */
void GameState::toggleMinimap() {
   if (minimap != NULL) {
      minimap->toggle();
   }
}

/**
 * Draw everything in gamestate
 */
void  GameState::draw() {
   if (!drawGraphics) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      SDL_GL_SwapBuffers();
      return;
   }
   // the time difference since last update call
   double timeDiff = doubleTime() - lastDrawTime;

   if (stereo_eye_left || !drawStereo_enabled) {
      setCurFPS(1 / timeDiff);
      lastDrawTime = doubleTime();

      // Clear the screen
      glClear(GL_COLOR_BUFFER_BIT);
   }
   
   glClear(GL_DEPTH_BUFFER_BIT);

   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   usePerspective();

   glMatrixMode(GL_MODELVIEW);


   // Draw glowing objects to a texture for the bloom effect.
   aspect = (float)gameSettings->GW/(float)gameSettings->GH;

   glPushMatrix();

   if (gsm == MenuMode) {
      gluLookAt(0, 0, 20, // Eye at origin + 20.
            0, 0, -1, // X goes right, Z goes away.
            0, 1, 0); // Y goes up.
   }

   drawObjects();

   glPopMatrix();
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);


   if (gameSettings->bloom) {
      hBlur();
      vBlur();
   }

   usePerspective();

   if (!gameSettings->drawDeferred) {
      glPushMatrix();
      // Draw the hud
      if (gsm != MenuMode) {
         drawHud();
      }
      glPopMatrix();
   } else {
      glPushMatrix();
      if (gsm != MenuMode) {
         drawHud();
      }
      glPopMatrix();

      glPushMatrix();
      useOrtho();
      render(this);
      usePerspective();
      glPopMatrix();
   }

   if (gameSettings->useOverlay) {
      drawOverlay();
   }

   drawScreens();

   // TODO: Make this clearer. Why not swap when lookAt is true?
   if (gsm != MenuMode) {
      if ((!stereo_eye_left) || !drawStereo_enabled) {
         SDL_GL_SwapBuffers();
      }
   }
}

Camera* GameState::getCurrentCamera() {
   if (usingShipCamera && ship != NULL && shipCamera != NULL) {
      currentCamera = shipCamera;
      shipCamera->setViewVector(ship->getViewVector());
      shipCamera->setOffset(*ship->getCameraOffset());
   } else {
      currentCamera = spectatorCamera;
   }
   return currentCamera;
}

/**
 * Draw objects
 */
void GameState::drawObjects() {
   if (!drawGraphics) {
      return;
   }

   // This decides where the camera is, either ship or spectator camera.
   getCurrentCamera(); // Also sets currentCamera.

   // If this gameState is not mainGamestate menu, set the camera and shake.
   if (gsm != MenuMode) {
      currentCamera->setCamera(true);
   }

   if (ship != NULL && shipCamera != NULL && currentCamera == shipCamera) {
      shipCamera->shake(ship->getShakeAmount());
   }

   if (gameSettings->drawDeferred) {
      fboBegin();
      GLenum buffers[] = {NOLIGHT_BUFFER, GLOW_BUFFER};
      // Draw the skybox if this gameState is not mainmenu and is glow shading
      if ((gsm != MenuMode)) {
         glDrawBuffers(1, buffers);
         glDrawBuffer(NOLIGHT_BUFFER);
         skybox->draw(currentCamera);
      }

      glDrawBuffers(2, buffers);
      // Draw the outer cube
      cube->draw();
      //fboEnd();
   } else {
      // Draw the skybox if this gameState is not mainmenu and is glow shading
      if ((gsm != MenuMode)) {
         skybox->draw(currentCamera);
      }
      // Draw the outer cube
      cube->draw();
   }

   glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
   if ((gsm != MenuMode) && usingShipCamera && ship != NULL) {
      if (gameSettings->drawDeferred) {
         GLenum buffers[] = {GLOW_BUFFER, GL_NONE, NOLIGHT_BUFFER,
            ALBEDO_BUFFER};
         glDrawBuffers(4, buffers);
         glUseProgram(bonerShader);
      }

      ship->drawCrosshair();
      if (gameSettings->drawDeferred) {
         glUseProgram(0);
      }
      //fboEnd();
   }

   // Get a list of all of the objects after culling them down to the view frustum.
   if (ship != NULL) {
      viewFrustumObjects = ship->getRadar()->getViewFrustumReading();
   } else {
      viewFrustumObjects = getGenericViewFrustumReading(&custodian, spectatorCamera);
   }

   for (listIter = viewFrustumObjects->begin(); listIter != viewFrustumObjects->end(); ++listIter) {
      if (*listIter == NULL) {
         continue;
      } else if (*listIter == ship && (gsm == MenuMode)) {
         // Don't draw the ship in first Person mode.
      } else {
         (*listIter)->draw();
      }
   }
   if (gameSettings->drawDeferred) {
      fboEnd();
   }

   delete viewFrustumObjects;
}


void GameState::hBlur() {
   int tex = Texture::getTexture("fboTex");
   GLint texLoc = glGetUniformLocation(hBlurShader, "RTscene");
   glUseProgram(hBlurShader);
   glUniform1i(texLoc, tex);

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
   glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
   glClear(GL_COLOR_BUFFER_BIT);

   drawScreenQuad(tex);

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void GameState::vBlur() {
   int tex = Texture::getTexture("hblurTex");
   GLint texLoc = glGetUniformLocation(vBlurShader, "blurSampler");
   glUseProgram(vBlurShader);
   glUniform1i(texLoc, tex);

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
   glDrawBuffer(GL_COLOR_ATTACHMENT2_EXT);
   glClear(GL_COLOR_BUFFER_BIT);

   drawScreenQuad(tex);

   glUseProgram(0);

   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void GameState::drawBloom() {
   drawScreenQuad(Texture::getTexture("bloomTex"));
}

// Draw the Hud, which includes, minimap, weapon/health bar, text overlay...
void GameState::drawHud() {
   glPushMatrix();
   useOrtho();

   /* Set the shipCamera using the location of your eye,
    * the location where you're looking at, and the up vector.
    * The shipCamera is set to be just 0.25 behind where you're looking at.
    */
   gluLookAt(0, 0, 0.25, 0, 0, 0, 0, 1, 0);

   //glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);


   /* We need to disable the lighting temporarily
    * in order to set the color properly.
    */
   glDisable(GL_LIGHTING);
   glDisable(GL_CULL_FACE);
   drawAllText();
   if(usingShipCamera && ship != NULL && !ship->isRespawning()){
      if (gameSettings->drawDeferred) {
         fboBegin();
         glDrawBuffer(HUD_BUFFER);
      }

      float width = gameSettings->GW / (float) gameSettings->GH;
      float stereoOffset = 0; // px
      
      if (gsm == SingleMode) {
         if (drawStereo_enabled) {
            stereoOffset = p2wx((gameSettings->GW / 4) + 3);
            if (!stereo_eye_left) {
               stereoOffset *= -1;
            }
         }

         shardBankBar->x = p2wx(10) + stereoOffset;
         shardBankBar->y = p2wy(100);

         shardBankBar->draw();
      }
      
      if (drawStereo_enabled) {
         width /= 2;
         stereoOffset = p2wx((gameSettings->GW / 4) + 5);
         if (!stereo_eye_left) {
            stereoOffset *= -1;
         }
      }


      healthBar->width = width;
      healthBar->x += stereoOffset;
      healthBar->draw();
      healthBar->x -= stereoOffset;
      weaponBar->draw();

      if (gameSettings->enableMinimap) {
         drawMinimap();
      }
      if (gameSettings->drawDeferred) {
         fboEnd();
      }
   }
   usePerspective();

   glEnable(GL_CULL_FACE);

   glPopMatrix();
}

void GameState::drawOverlay() {
   glPushMatrix();
   glClear(GL_DEPTH_BUFFER_BIT);
   useOrtho();
   glDisable(GL_LIGHTING);
   float minY = -1.0;
   float maxY = 1.0;
   float minX = -1.0f * aspect;
   float maxX = 1.0f * aspect;

   if (!gameIsRunning) {
      glColor4f(0.0, 1.0, 0.0, 0.7f);
      float scale = 1.0;
      glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
      glBegin(GL_QUADS);
      glVertex3f(minX * scale, minY * scale, 0.0);
      glVertex3f(maxX * scale, minY * scale, 0.0);
      glVertex3f(maxX * scale, maxY * scale, 0.0);
      glVertex3f(minX * scale, maxY * scale, 0.0);
      glEnd();
   }

   int tex = Texture::getTexture("overlayTex");
   drawScreenQuad(tex);
   usePerspective();
   glPopMatrix();
}

/**
 * Draw all of the text in the allTexts list to the screen.
 * This function should be called once per display loop.
 * We do all of the looking at, lighting changes, etc.
 * one time here to improve efficiency.
 */
void GameState::drawAllText() {
   int rightEdge, leftEdge;
   if (drawStereo_enabled) {
      rightEdge = gameSettings->GW / 2;
      rightEdge += stereo_eye_left ? 3 : -3;

      leftEdge = 10 + (stereo_eye_left ? 3 : -3);

   } else {
      rightEdge = gameSettings->GW;
      leftEdge = 10;
   }

   SDL_Rect position;
   const Sint16 positionDifferenceY = 15;

   // Position all the text on left side
   position.x = leftEdge;
   position.y = 30;
   FPSText->setPosition(position);

   position.y = (Sint16) (position.y + positionDifferenceY);
   scoreText->setPosition(position);

   position.y = (Sint16) (position.y + positionDifferenceY);
   bankedShardText->setPosition(position);

   position.y = (Sint16) (position.y + positionDifferenceY);
   unbankedShardText->setPosition(position);

   position.y = (Sint16) (position.y + positionDifferenceY);

   shardBankBar->y = p2wy(position.y);

   // Position all the text on right side
   position.x = (Sint16)(rightEdge - 30);
   position.y = 30;
   curLevelText->setPosition(position);

   position.y = (Sint16) (position.y + positionDifferenceY);
   timerText->setPosition(position);

   position.y = (Sint16) (position.y + positionDifferenceY);
   lifeText->setPosition(position);

   // Draw all the text on left side

   FPSText->draw();

   if (ship != NULL) {
      scoreText->draw();
      if (gsm == SingleMode) {
         bankedShardText->draw();
         unbankedShardText->draw();
      }
      lifeText->draw();
   }

   // Draw all the text on right side
   timerText->draw();
   curLevelText->draw();

   // Draw all messages
   GameMessage::drawAllMessages();
}

/**
 * Update the values contained in all of the texts.
 */
void GameState::updateText() {
   FPSText->updateBody((int)curFPS);
   if (ship != NULL) {
      scoreText->updateBody(ship->getScore());
      bankedShardText->updateBody(ship->bankedShards);
      unbankedShardText->updateBody(ship->unbankedShards);
      lifeText->updateBody(ship->lives);
   }

   // Update the timer on screen to show the minutes & seconds remaining.
   std::string minutes;
   sstream2 << minutesRemaining(levelTimer.getTimeLeft());
   //sstream2 << minutesRemaining(levelDuration - (doubleTime() - timeStarted));
   minutes = sstream2.str();
   sstream2.str("");
   std::string seconds;
   int secsRemaining = secondsRemainder(levelTimer.getTimeLeft());
   sstream2 << secondsRemainder(levelTimer.getTimeLeft());

   // Throw a 0 in front, so that it comes out 2:09 instead of 2:9.
   if (secsRemaining < 10) {
      seconds = "0" + sstream2.str();
   }
   else {
      seconds = sstream2.str();
   }

   sstream2.str("");
   timerText->updateBody(minutes + ":" + seconds);
}

void GameState::initAsteroids() {
   /**
    * Don't init asteroids in the menu.
    */
   if (gsm == MenuMode) {
      return;
   }

   Asteroid3D* tempAsteroid;

   /*
   std::set<CollisionBase*, compareByDistance>* collisions;
   std::set<CollisionBase*, compareByDistance>::iterator curCollision;
   */

   /* We want this spaceHolder because we don't want to spawn asteroids
    * too close to the ship.
    */
   /* TODO: One of these should go around each ship. */
   /*
   Object3D* spaceHolder = new Object3D(this);
   spaceHolder->minX = spaceHolder->minY = spaceHolder->minZ = -10;
   spaceHolder->maxX = spaceHolder->maxY = spaceHolder->maxZ = 10;
   custodian.add(spaceHolder);

   int numCollisions = 0;
   // Spawn the initial asteroids for the game.
   */
   double asteroidSize;
   if (curLevel == 1)
      asteroidSize = 5;
   else
      asteroidSize = 8;

   for (int i = 0; i < numAsteroidsToSpawn; ++i) {
      tempAsteroid = new Asteroid3D(asteroidSize, worldSize, this, true);
      // Add each asteroid to the custodian so we know of its existence.
      custodian.add(tempAsteroid);
      /*
      do {
         custodian.update();
         collisions = custodian.findCollisions(tempAsteroid, true);
         numCollisions = (int) collisions->size();
         if (numCollisions > 0) {
            tempAsteroid->newRandomPosition();
         }
         // TODO: Fix memory leak.
         delete collisions;
      } while (numCollisions > 0);
      */
   }
   /*
   spaceHolder->shouldRemove = true;
   */
   custodian.update();
}

void GameState::setCurFPS(double fpsIn) {
   curFPS = fpsIn;
}

void GameState::resetClientCommand(bool shouldClearMultiplayerInfo) {
   // Do stuff.
   clientCommand.clear_forwardacceleration();
   clientCommand.clear_rightacceleration();
   clientCommand.clear_upacceleration();
   clientCommand.clear_brake();
   
   clientCommand.clear_yawspeed();
   clientCommand.clear_pitchspeed();
   clientCommand.clear_rollspeed();

   clientCommand.clear_fire();

   clientCommand.set_curweapon(BLASTER_WEAPON_INDEX);

   clientCommand.set_mousex(0);
   clientCommand.set_mousey(0);

   clientCommand.set_name(gameSettings->name);

   // Do we need to clear shipid and lastreceivedgamestateid?
   if (shouldClearMultiplayerInfo) {
      clientCommand.clear_shipid();
      clientCommand.clear_lastreceivedgamestateid();
   }
}

/**
 * Reset everything in the game to play again
 */
void GameState::reset(bool shouldLoad) {
   delete shipCamera;
   delete spectatorCamera;
   delete cube;
   delete ship;
   delete minimap;
   shipCamera = NULL;
   spectatorCamera = NULL;
   cube = NULL;
   ship = NULL;
   minimap = NULL;

   custodian.clear();
   Particle::Clear();
   MeshFace::Clear();
   custodian.update();
   custodian.cleanup();
   resetClientCommand();

   setLevelTimer();

   gameIsRunning = true;
   // The level is not over when we're starting a new game.
   levelOver = false;

   curLevel = 1;
   cube = new BoundingSpace(worldSize / 2, 0, 0, 0, this);


   // Client Side network diff reset
   if (gsm == ClientMode) {
      justLoadedFirstFrame = false;
   }

   //temp
   if(shouldLoad) {
      loadFromDisk();
      custodian.update();
   } else {
      numAsteroidsToSpawn = decideNumAsteroidsToSpawn();
      
      std::cout << "Resetting." << std::endl;

      if (gsm == SingleMode) {
         ship = new AsteroidShip(this);
      }


      if (gsm == MenuMode || gsm == SingleMode) {
         custodian.add(ship);
         // Crashes without this.
         initAsteroids();
      } else if (gsm == ServerMode) {
         initAsteroids();
      } else if (gsm == ClientMode) {
         custodian.update();
      }
   }
   
   if (gsm == SingleMode || ship != NULL) {
      setShip(ship);
   }
   
   if (ship == NULL) {
      usingShipCamera = false;
   } else {
      usingShipCamera = true;
   }

   spectatorCamera = new Camera(false);
   curLevelText->updateBody(curLevel);
  
   storeMenu->menuActive = false;
   
   GameMessage::Clear();
   addLevelMessage();

   if (levelTimer.getIsPaused()) {
      printf("LT was paused.\n");
      levelTimer.resume();
      if (levelTimer.isRunning) {
         printf("LT now running with %f seconds left.\n",
               levelTimer.getTimeLeft());
      }
   }
}

/**
 * Add a message at the beginning of each level telling the player
 * which level it is.
 */
void GameState::addLevelMessage() {
   //add level message
   std::ostringstream levelMessage;
   levelMessage << "Level " << curLevel;
   GameMessage::Add(levelMessage.str(), 30, 5, this);
}

/**
 * Add a warning message telling the player that there's enemies
 * at the beginning of each level.
 */
void GameState::addWarningMessage() {
   //Add warning mesage
   if (gsm == SingleMode && (curLevel > 1)) {
      std::ostringstream gameMsg;
      int numAIShips = (custodian.ships.size()-1);
      if (numAIShips == 1) {
         gameMsg << "WARNING: 1 Enemy Ship Detected";
      } else {
         gameMsg << "WARNING: " << numAIShips << " Enemy Ships Detected";
      }
      
      GameMessage::Add(gameMsg.str(), 30, 5, this);
   }
}

/**
 * Add a message telling the player that a new weapon has been unlocked.
 */
void GameState::addWeaponUnlockMessage(Weapon* unlockedWeapon) {
   std::ostringstream gameMsg;
   gameMsg << unlockedWeapon->getName() << " level " << unlockedWeapon->level << " unlocked!";
   GameMessage::Add(gameMsg.str(), 30, 3, this);
}

/**
 * Spawn one more asteroid per level, up to 8, where it's capped.
 * Levels 8+ will all spawn only 8 asteroids.
 */
int GameState::decideNumAsteroidsToSpawn() {
   return std::min(curLevel, 8);
}

void GameState::nextLevel() {
   SoundEffect::stopAllSoundEffect();

   std::set<AsteroidShip*>::iterator shipIter;

   for (shipIter = custodian.ships.begin(); shipIter != custodian.ships.end(); ++shipIter) {
      (*shipIter)->atLevelEnd();
   }

   std::set<Asteroid3D*>::iterator asteroidIter;

   for (asteroidIter = custodian.asteroids.begin(); asteroidIter != custodian.asteroids.end(); ++asteroidIter) {
      (*asteroidIter)->shouldRemove = true;
   }

   std::set<Shard*>::iterator shardIter;

   for (shardIter = custodian.shards.begin(); shardIter != custodian.shards.end(); ++shardIter) {
      (*shardIter)->shouldRemove = true;
   }

   std::set<Shot*>::iterator shotIter;

   for (shotIter = custodian.shots.begin(); shotIter != custodian.shots.end(); ++shotIter) {
      (*shotIter)->shouldRemove = true;
   }

   if (gsm == SingleMode && 
       ship != NULL && 
       !ship->shooter->isEnabled() && !ship->flyingAI->isEnabled()) {
      storeMenu->menuActive = true;
      setMusic();
   }

   setLevelTimer();
   
   gameIsRunning = true;
   // The current level is over when we're advancing to the next level.
   levelOver = true;
   curLevel++;
   curLevelText->updateBody(curLevel);
   numAsteroidsToSpawn = decideNumAsteroidsToSpawn();
   printf("Level'd up to %d!\n", curLevel);

   // Add AI players
   if (gsm == SingleMode && (curLevel > 1)) {
      /* Spawn 1 on level 1, 1 on level 2, 2 on level 3, 2 on level 4.. etc.
       * Never spawn more than 4 new AI's on any level though, but old AI's
       * can carry over from previous levels.
       */
      for(int tmp = 0; tmp < std::min((int)floor(curLevel/2), 4); tmp++) {
         addAIPlayer();
      }
   }

   // Add Asteroids
   if (gsm == SingleMode || gsm == ServerMode) {
      initAsteroids();
   }


   custodian.update();
   GameMessage::Clear();

   addLevelMessage();
   if (gsm == SingleMode) {
      addWarningMessage();
   }

   // Reset the ship's controls so that the ship isn't stuck moving in a direction
   resetClientCommand();
}

/**
 * Handles the player pressing down a key
 */
void GameState::keyDown(int key, int unicode) {
   //If game is not running, do not take input anymore
   if (!gameIsRunning || chat->active || ipInput->active) {
      return;
   }

   switch(key) {
      //movement keys, not valid if flying AI is enabled

      if (ship != NULL && !ship->flyingAI->isEnabled()) {
      case SDLK_w:
         isW = true;
         if (isS) {
            clientCommand.set_forwardacceleration(0);
         } else {
            clientCommand.set_forwardacceleration(1);
         }
         break;

      case SDLK_s:
         isS = true;
         if (isW) {
            clientCommand.set_forwardacceleration(0);
         } else {
            clientCommand.set_forwardacceleration(-1);
         }
         break;

      case SDLK_a:
         isA = true;
         if (!doYaw) {
            if (isD) {
               clientCommand.set_yawspeed(0);
            } else {
               clientCommand.set_yawspeed(1.0);
            }
         } else {
            if (isD) {
               clientCommand.set_rollspeed(0);
            } else {
               clientCommand.set_rollspeed(-1.0);
            }
         }
         break;

      case SDLK_d:
         isD = true;
         if (!doYaw) {
            if (isA) {
               clientCommand.set_yawspeed(0);
            } else {
               clientCommand.set_yawspeed(-1.0);
            }
         } else {
            if (isA) {
               clientCommand.set_rollspeed(0);
            } else {
               clientCommand.set_rollspeed(1.0);
            }
         }
         break;

      case SDLK_i:
         clientCommand.set_mousex(0);
         clientCommand.set_mousey(0);
         isI = true;
         if (isK) {
            clientCommand.set_pitchspeed(0);
         } else {
            clientCommand.set_pitchspeed(1.0);
         }
         break;
      
      case SDLK_k:
         clientCommand.set_mousex(0);
         clientCommand.set_mousey(0);
         isK = true;
         if (isI) {
            clientCommand.set_pitchspeed(0);
         } else {
            clientCommand.set_pitchspeed(-1.0);
         }
         break;
      
      case SDLK_j:
         clientCommand.set_mousex(0);
         clientCommand.set_mousey(0);
         isJ = true;
         if (doYaw) {
            if (isL) {
               clientCommand.set_yawspeed(0);
            } else {
               clientCommand.set_yawspeed(1.0);
            }
         } else {
            if (isL) {
               clientCommand.set_rollspeed(0);
            } else {
               clientCommand.set_rollspeed(-1.0);
            }
         }
         break;
      
      case SDLK_l:
         clientCommand.set_mousex(0);
         clientCommand.set_mousey(0);
         isL = true;
         if (doYaw) {
            if (isJ) {
               clientCommand.set_yawspeed(0);
            } else {
               clientCommand.set_yawspeed(-1.0);
            }
         } else {
            if (isJ) {
               clientCommand.set_rollspeed(0);
            } else {
               clientCommand.set_rollspeed(1.0);
            }
         }
         break;

      case SDLK_q:
         clientCommand.set_rightacceleration(-1);
         break;

      case SDLK_e:
         clientCommand.set_rightacceleration(1);
         break;

         /*
      case SDLK_SPACE:
         clientCommand.set_upacceleration(1);
         break;
         */
      case SDLK_SPACE:
         if (ship != NULL && !ship->shooter->isEnabled()) {
            clientCommand.set_fire(true);
         }
         break;

      /*
      case SDLK_LCTRL:
         clientCommand.set_upacceleration(-1);
         break;
         */

      case SDLK_RSHIFT:
         doYaw = !doYaw;
         clientCommand.set_rollspeed(0);
         break;


      case SDLK_b:
         clientCommand.set_brake(true);
         break;
      }


      //Camera controls
   if (ship != NULL) {
      case SDLK_8:
            ship->setZoomSpeed(-1);
         break;
      case SDLK_9:
            ship->setZoomSpeed(1);
         break;

      case SDLK_t:
            ship->nextView();
         break;
      case SDLK_y:
            usingShipCamera = !usingShipCamera;
         break;

         /*
      case SDLK_c:
            shipCamera->stereoOffsetAmount += 0.05;
            printf("stereoOffset: %f\n", shipCamera->stereoOffsetAmount);
            break;
      case SDLK_x:
            shipCamera->stereoOffsetAmount -= 0.05;
            printf("stereoOffset: %f\n", shipCamera->stereoOffsetAmount);
            break;
            */
   }

   case SDLK_KP_PLUS:
      spectatorSpeed += 0.1;
      break;
   case SDLK_KP_MINUS:
      spectatorSpeed -= 0.1;
      break;
   case SDLK_KP_MULTIPLY:
      spectatorRadius += 10.0;
      break;
   case SDLK_KP_DIVIDE:
      spectatorRadius -= 10.0;
      break;


      // AI controls
   if (ship != NULL) {
         /*
      case SDLK_g:
         //TODO: Based on how many shooting AIs this ship has, activate the correct one.
         if(ship->shooter->isEnabled())
            ship->shooter->disable();
         else
            ship->shooter->enable();
         break;

      case SDLK_f:
         // When you disable the flyingAI, stop accelerating the ship.
         if(ship->flyingAI->isEnabled()) {
            ship->flyingAI->disable();
            resetClientCommand();
         } else {
            // Stop accelerating the ship when turning on the flyingAI as well.
            resetClientCommand();
            ship->flyingAI->enable();
         }
         break;
         */


         //switch weapons
      case SDLK_v:
      case SDLK_u:
         // Keep scrolling through weapons as long as they're not purchased.
         clientCommand.set_curweapon(ship->getPrevWeaponID());
         break;

      case SDLK_z:
      case SDLK_o:
         // Keep scrolling through weapons as long as they're not purchased.
         clientCommand.set_curweapon(ship->getNextWeaponID());
         break;

         // Enable chat.
         /*
      case SDLK_j:
         chat->activate(true);
         break;
         */

         // Minimap Display Size
      case SDLK_1:
         minimap->adjustDisplaySizeDirection = 1;
         break;
      case SDLK_2:
         minimap->adjustDisplaySizeDirection = -1;
         break;
      case SDLK_0:
         minimap->toggle();
         break;
         // Minimap Zoom
      case SDLK_3:
         minimap->disableAutoZoom();
         minimap->adjustZoomDirection = 1;
         break;
      case SDLK_4:
         minimap->disableAutoZoom();
         minimap->adjustZoomDirection = -1;
         break;
      case SDLK_5:
         minimap->toggleAutoZoom();
         break;
   }


      // Audio and Video settings
   case SDLK_KP_ENTER:
      if (SoundEffect::musicPlaying()) {
         SoundEffect::pauseMusic();
      } else {
         SoundEffect::resumeMusic();
      }
      break;


      // DEBUG KEYS
   case SDLK_F2:
      gameSettings->bloom = !gameSettings->bloom && gameSettings->goodBuffers;
      break;
   case SDLK_F3:
      showBloomScreen = !showBloomScreen;
      break;
   case SDLK_F4:
      nextLevel();
      break;

   case SDLK_F5:
      gameSettings->drawDeferred = !gameSettings->drawDeferred && gameSettings->goodBuffers;
      break;

   case SDLK_F6:
      testFunction();
      break;

   case SDLK_F7:
      drawGraphics = !drawGraphics;
      break;

   case SDLK_F8:
      debugPosition();
      break;
   case SDLK_F9:
      addAIPlayer();
      break;

   if (ship != NULL) {
      case SDLK_F10:
         ship->unbankedShards += 10;
         break;
   }
      // If the user presses F11, give them all of the weapons.
   case SDLK_F11:
      // Make all of the weapons be purchased.
      if (ship != NULL) {
         for (int i = 0; i < ship->getNumWeapons(); i++) {
            ship->getWeapon(i)->purchased = true;
         }
      } else {
         std::set<AsteroidShip*>::iterator tmpShip;
         tmpShip = custodian.ships.begin();
         for (; tmpShip != custodian.ships.end(); ++tmpShip) {
            for (int i = 0; i < (*tmpShip)->getNumWeapons(); i++) {
               (*tmpShip)->getWeapon(i)->purchased = true;
            }
         }
      }
      break;
      // Enables God Mode
   case SDLK_F12:
      godMode = !godMode;
      printf("Zoe mode: %d\n", godMode);
      break;
   }
}

/**
 * Handles the player letting go of a key
 */
void GameState::keyUp(int key) {
   //If game is not running, do not take input anymore
   if(!gameIsRunning || chat->active){ return;}

   switch(key) {
   case SDLK_s:
      isS = false;
      if(ship != NULL && !ship->flyingAI->isEnabled()) {
         if (isW) {
            clientCommand.set_forwardacceleration(1);
         } else {
            clientCommand.set_forwardacceleration(0);
         }
      }
      break;

   case SDLK_w:
      isW = false;
      if(ship != NULL && !ship->flyingAI->isEnabled()) {
         if (isS) {
            clientCommand.set_forwardacceleration(-1);
         } else {
            clientCommand.set_forwardacceleration(0);
         }
      }
      break;

   case SDLK_a:
      isA = false;
      if(ship != NULL && !ship->flyingAI->isEnabled()) {
         if (!doYaw) {
            if (isD) {
               clientCommand.set_yawspeed(-1.0);
            } else {
               clientCommand.set_yawspeed(0);
            }
         } else {
            if (isD) {
               clientCommand.set_rollspeed(1.0);
            } else {
               clientCommand.set_rollspeed(0);
            }
         }
      }
      break;

   case SDLK_d:
      isD = false;
      if(ship != NULL && !ship->flyingAI->isEnabled()) {
         if (!doYaw) {
            if (isA) {
               clientCommand.set_yawspeed(1.0);
            } else {
               clientCommand.set_yawspeed(0);
            }
         } else {
            if (isA) {
               clientCommand.set_rollspeed(-1.0);
            } else {
               clientCommand.set_rollspeed(0);
            }
         }
      }
      break;


   if (ship != NULL && !ship->flyingAI->isEnabled()) {
      case SDLK_i:
         clientCommand.set_mousex(0);
         clientCommand.set_mousey(0);
         isI = false;
         if (isK) {
            clientCommand.set_pitchspeed(-1.0);
         } else {
            clientCommand.set_pitchspeed(0);
         }
         break;
      
      case SDLK_k:
         clientCommand.set_mousex(0);
         clientCommand.set_mousey(0);
         isK = false;
         if (isI) {
            clientCommand.set_pitchspeed(1.0);
         } else {
            clientCommand.set_pitchspeed(0);
         }
         break;
      
      case SDLK_j:
         clientCommand.set_mousex(0);
         clientCommand.set_mousey(0);
         isJ = false;
         if (doYaw) {
            if (isL) {
               clientCommand.set_yawspeed(-1.0);
            } else {
               clientCommand.set_yawspeed(0);
            }
         } else {
            if (isL) {
               clientCommand.set_rollspeed(1.0);
            } else {
               clientCommand.set_rollspeed(0);
            }
         }
         break;
      
      case SDLK_l:
         clientCommand.set_mousex(0);
         clientCommand.set_mousey(0);
         isL = false;
         if (doYaw) {
            if (isJ) {
               clientCommand.set_yawspeed(1.0);
            } else {
               clientCommand.set_yawspeed(0);
            }
         } else {
            if (isJ) {
               clientCommand.set_rollspeed(-1.0);
            } else {
               clientCommand.set_rollspeed(0);
            }
         }
         break;
   }


   case SDLK_q:
   case SDLK_e:
      if(ship != NULL && !ship->flyingAI->isEnabled())
         clientCommand.set_rightacceleration(0);
      break;

      /*
   case SDLK_SPACE:
   case SDLK_LCTRL:
      if(ship != NULL && !ship->flyingAI->isEnabled())
         clientCommand.set_upacceleration(0);
      break;
      */
   case SDLK_SPACE:
      if (ship != NULL && !ship->shooter->isEnabled()) {
         clientCommand.set_fire(false);
      }
      break;


   case SDLK_b:
      clientCommand.set_brake(false);
      break;
      // Minimap Size
   if (ship != NULL) {
      case SDLK_1:
         minimap->adjustDisplaySizeDirection = 0;
         break;
      case SDLK_2:
         minimap->adjustDisplaySizeDirection = 0;
         break;
         // Minimap Zoom
      case SDLK_3:
         minimap->adjustZoomDirection = 0;
         break;
      case SDLK_4:
         minimap->adjustZoomDirection = 0;
         break;

         // Camera
      case SDLK_8:
      case SDLK_9:
         ship->setZoomSpeed(0);
         break;
      }
   }
}

/**
 * Handles the player clicking the mouse
 */
void GameState::mouseDown(int button) {
   //If game is not running, do not take input anymore
   if(!gameIsRunning){ return;}

   switch(button) {
      // Left mouse down
   case 1:
      clientCommand.set_fire(true);
      break;

      // Right mouse down
   case 3:
      doYaw = !doYaw;
      clientCommand.set_rollspeed(0);
      clientCommand.set_yawspeed(0);
      break;

      // Scroll
   case 4:
      clientCommand.set_curweapon(ship->getPrevWeaponID());
      break;
   case 5:
      clientCommand.set_curweapon(ship->getNextWeaponID());
      break;
   }
}

/**
 * Handles the player letting go of a mouse click
 */
void GameState::mouseUp(int button) {
   //If game is not running, do not take input anymore
   if(!gameIsRunning){ return;}

   switch (button) {
      // Left mouse up
   case 1:
      clientCommand.set_fire(false);
      break;
      // Right mouse up
   case 3:
      doYaw = !doYaw;
      clientCommand.set_rollspeed(0);
      clientCommand.set_yawspeed(0);
      break;
   }
}

void GameState::mouseMove(int dx, int dy, int x, int y) {
   //If game is not running, do not take input anymore
   if(!gameIsRunning)
      return;

   bool drawStereo_enabled_tmp = drawStereo_enabled;
   drawStereo_enabled = false;
   shipControlX = p2wx(x);
   shipControlY = p2wy(y);

   mouseX = shipControlX;
   mouseY = shipControlY;

   clientCommand.set_mousex(mouseX);
   clientCommand.set_mousey(mouseY);

   shipControlX = shipControlX / p2wx(gameSettings->GW);
   shipControlY = shipControlY / p2wy(0);
   
   drawStereo_enabled = drawStereo_enabled_tmp;

   shipControlX = clamp(shipControlX * fabs(shipControlX * shipControlX), -1, 1);
   shipControlY = clamp(-shipControlY * fabs(shipControlY * shipControlY), -1, 1);

   if (ship != NULL) {
      if(!ship->flyingAI->isEnabled()) {
         if (doYaw) {
            clientCommand.set_yawspeed(-shipControlX);
         } else {
            clientCommand.set_rollspeed(shipControlX);
         }

         clientCommand.set_pitchspeed(shipControlY);
      }
   }
}

double GameState::getWallxMin() const { return cube->xMin; }
double GameState::getWallxMax() const { return cube->xMax; }
double GameState::getWallyMin() const { return cube->yMin; }
double GameState::getWallyMax() const { return cube->yMax; }
double GameState::getWallzMin() const { return cube->zMin; }
double GameState::getWallzMax() const { return cube->zMax; }

double GameState::getMouseX() { return mouseX; }
double GameState::getMouseY() { return mouseY; }


Camera* GameState::getCurrentCamera() const {
   if (usingShipCamera) {
      return shipCamera;
   } else {
      return spectatorCamera;
   }
}

void GameState::debugPosition() {
   // First reset.
   reset();
   // Find the asteroid. Set its position.
   Asteroid3D* asteroid = NULL;
   std::vector<Object3D*>* objects = custodian.getListOfObjects();
   std::vector<Object3D*>::iterator iter = objects->begin();

   for (; iter != objects->end(); ++iter) {
      asteroid = dynamic_cast<Asteroid3D*>(*iter);
      if (asteroid != NULL)
         break;
   }

   ship->position->update(0, 0, 0);
   ship->forward->updateMagnitude(0, 1, 0);
   ship->up->updateMagnitude(0, 0, 1);
   ship->right->updateMagnitude(1, 0, 0);

   asteroid->position->update(0, 20, 0);
   asteroid->velocity->updateMagnitude(0, 0, 0);

   asteroid->update(0.01);
   asteroid->debug();

   // Make all of the weapons be purchased.
   for (int i = 0; i < ship->getNumWeapons(); i++) {
      ship->getWeapon(i)->purchased = true;
      if(ship->getWeapon(i)->curAmmo != -1) {
         ship->getWeapon(i)->curAmmo += 500;
      }
   }

}

/**
 * This is going to be a lot like the save() function, but we pass in
 * a complete gameState and get back a diffed gamestate.
 * The load() function will handle a diff no problem, as long as it already has
 * an existing gamestate in memory.
 * We're going to overwrite the gamestate we were passed in.
 */
void GameState::saveDiff(const ast::GameState& oldState, ast::GameState* newState) {
   if (gsm == SingleMode) {
      // We probably won't use ship->id when running a server.
      if (oldState.playership() != ship->id)
         newState->set_playership(ship->id);
   }

   newState->set_gametime(getGameTime()); // Always changes.

   if (oldState.curlevel() != curLevel)
      newState->set_curlevel(curLevel);
   
   newState->set_id(curGameStateId);
   
   // This means if the leveltimer hasn't changed, clear it.
   // Sucks that we have to allocate then deallocate, but I can't think of a better way.
   if (!levelTimer.saveDiff(oldState.leveltimer(), newState->mutable_leveltimer())) {
      newState->clear_leveltimer();
   }

   const std::map<unsigned, Object3D*>& objects = custodian.getObjectsByID();
   std::map<unsigned, Object3D*>::const_iterator iter = objects.begin();
   const ast::Entity* oldEnt;
   ast::Entity* newEnt;
   Object3D* obj = NULL;

   int curOldId = 0;

   // We have to iterate through all current objects and all objects in oldState
   // at the same time to update the right ones.
   // We take advantage of the fact that both lists are sorted by id.
   for (; iter != objects.end(); ++iter) {
      oldEnt = NULL;
      newEnt = newState->add_entity();
      obj = (*iter).second;

      while (curOldId < oldState.entity_size()) {
         oldEnt = &oldState.entity(curOldId);

         if (oldEnt->id() < obj->id) {
            // We must have removed this object.
            ++curOldId;
            // Reset pointer just in case.
            oldEnt = NULL;
            continue;
         } else if (oldEnt->id() == obj->id) {
            // This is the object we care about.
            ++curOldId;
            break;
         } else if (oldEnt->id() > obj->id) {
            if (obj->shouldRemove) {
               // If we get here, we want to just not send this entity.
               // The entity has been previously removed, so don't send more updates.
               oldEnt = NULL;
               break;
            } else {
               // We've gone too far!
               // We we should be able to add an object that we haven't already seen.
               // Kind of a weird error message, but it's unique.
               std::cerr << "Found serialized object with id > next real object." << std::endl;
               exit(EXIT_FAILURE);
            }
         }
      }

      if (oldEnt == NULL) {
         if (!obj->shouldRemove) {
            obj->save(newEnt);
         }
      } else if (!oldEnt->shouldremove()) {
         // Don't send old stuff.
         if (!obj->saveDiff(*oldEnt, newEnt)) {
            newState->mutable_entity()->RemoveLast();
         } else {
            if (obj->shouldRemove) {
               debugoutput << "Sending changed type " << obj->type << " id " << obj->id << "\n";
                     
            }
            assert(oldEnt->id() == newEnt->id());
         }
      }
   }
}

void GameState::saveToDisk() {
   ast::GameState gs;
   std::ofstream ofs(SAVEFILENAME);
   save(&gs);

   if (!gs.SerializeToOstream(&ofs)) {
      std::cerr << "Failed to write gameState." << std::endl;
   }

   std::cout << "Saved GameState" << std::endl;
}

void GameState::save(ast::GameState* gs) {
   if (gsm == SingleMode) {
      gs->set_playership(ship->id);
   }
   
   gs->set_gametime(getGameTime());
   gs->set_curlevel(curLevel);
   gs->set_id(curGameStateId);
   levelTimer.save(gs->mutable_leveltimer());
   
   const std::map<unsigned, Object3D*>& objects = custodian.getObjectsByID();
   std::map<unsigned, Object3D*>::const_iterator iter = objects.begin();

   ast::Entity* ent;
   Object3D* obj = NULL;

   for (; iter != objects.end(); ++iter) {
      obj = (*iter).second;
      if (!obj->shouldRemove) {
         ent = gs->add_entity();
         obj->save(ent);
      }
      //std::cout << "ID: " << ent->id() << " Type: " << ent->type() << std::endl;
   }
}

void GameState::loadFromDisk() {
   ast::GameState gs; // The incoming protobuf gamestate.
   std::ifstream ifs(SAVEFILENAME);

   // if (!ifs) No file. Oh well :/

   if (!gs.ParseFromIstream(&ifs)) {
      std::cerr << "Failed to read gameState." << std::endl;
   }

   load(gs);
   
   ifs.close();
   std::cout << "Loaded GameState" << std::endl;
}

void GameState::load(const ast::GameState& gs) {
   gameTime = gs.gametime();

   if (gs.has_leveltimer()) {
      levelTimer.load(gs.leveltimer());
   }

   if (gs.has_curlevel()) {
      curLevel = gs.curlevel();
   }

   for (int i = 0; i < gs.entity_size(); ++i) {
      const ast::Entity& ent = gs.entity(i);
      custodian.updateObjectFromEntity(ent);
      //std::cout << "ID: " << ent.id() << " Type: " << ent.type() << std::endl;
   }

   if (gs.has_playership()) {
      std::cout << "Playership id: " << gs.playership() << std::endl;
      AsteroidShip* tmpShip = dynamic_cast<AsteroidShip*>(custodian[gs.playership()]);
      if (tmpShip == NULL) {
         std::cerr << "ERROR no ship found!" << std::endl;
         exit(EXIT_FAILURE);
      } else {
         ship = tmpShip;
      }
   }
}

void GameState::testFunction() {
   /*NetTimer test;
   test.fromObject(levelTimer);

   std::ostringstream oss;
   boost::archive::text_oarchive oa(oss);
   int i = NET_OBJ_ASTEROID;
   oa << i << test;

   std::cout << "NetTimer=" << oss.str() << std::endl;*/
   
   if (ship != NULL) {
      ship->health = 1;
   }

}

void GameState::gameOver() {
   // Check if it is done waiting until going to the next level
   if (!gameOverTimer.isRunning) {
      GameMessage::Clear();
      std::ostringstream gameMsg;
      gameMsg << "Game Over ";
      GameMessage::Add(gameMsg.str(), 30, 5, this);
      gameOverTimer.setCountDown(5);
   }
}

double GameState::getGameTime() {
   return gameTime;
}

void GameState::updateGameTime(double timeDiff) {
   gameTime += timeDiff;
}

void GameState::connect(char* addr) {
   if (gsm != ClientMode) {
      std::cerr << "GameState::connect() called when not in client mode!" << std::endl;
      exit(EXIT_FAILURE);
   }

   reset();

   clientSide->connect(addr);
}

void GameState::disconnect() {
   if (gsm == ClientMode) {
      printf("Disconnecting.\n");
      clientSide->disconnect();
   }
}

/**
 * This is executed server side when a new command comes in.
 */
void GameState::handleCommand(const ast::ClientCommand& command) {
   if (!command.has_shipid() || command.shipid() == 0) {
      std::cerr << "Shipid not set. Ignoring." << std::endl;
      return;
   }

   AsteroidShip* curShip = dynamic_cast<AsteroidShip*>(custodian[command.shipid()]);
   if (curShip == NULL) {
      std::cerr << "Command refers to a nonexistent ship." << std::endl;
   } else {
      curShip->readCommand(command);
   }
}

/**
 * This is executed client side when a new frame comes in.
 */
void GameState::handleFrame(ast::Frame* frame) {

   if (frame->has_gamestate()) {
      ast::CollisionMessage* curCollisionMessage = NULL;
      
      if (frame->gamestate().id() > lastReceivedGameStateId ||
       lastReceivedGameStateId == 0) {
         while (frame->collision_message_size() > 0) {

            curCollisionMessage = frame->mutable_collision_message()->ReleaseLast();
            if (curCollisionMessage->gamestateid() > lastReceivedGameStateId) {
               savedCollisionMessages[curCollisionMessage->gamestateid()] = curCollisionMessage;
            } else {
               delete curCollisionMessage;
            }
         }

         //double localGameTime = gameTime;
         double newGameTime = frame->gamestate().gametime();

         // This sets the time to the new time and moves everything to where it should be.
         if (lastReceivedGameStateId != 0) {
            // Don't save collision data.
            advancePhysics(gameTime, newGameTime, NULL);
         }

         // This loads the new things for the current time.
         load(frame->gamestate());


         // If this is below 0, the update is taking us back in time.
         //double frameTimeDiff = gameTime - localGameTime;
         // frametimediff HEREEEEEEEE

         justLoadedFirstFrame = true;
         
         //double timeChangeAmount = 0.001; // Seconds

         custodian.update();
         lastReceivedGameStateId = frame->gamestate().id();
         clientCommand.set_lastreceivedgamestateid(lastReceivedGameStateId);
      }
      
      // TODO: Come up with a way to do this that doesn't involve resetting the ship every time.
      
      if (frame->has_shipid()) {
         std::cout << "Found shipid. Setting it to " << frame->shipid() << "." << std::endl;
         unsigned shipid = frame->shipid();
         
         AsteroidShip* tmpShip = dynamic_cast<AsteroidShip*>(custodian[shipid]);
         if (tmpShip == NULL) {
            std::cerr << "ERROR no ship found in handleFrame!" << std::endl;
            exit(EXIT_FAILURE);
         } else {
            setShip(tmpShip);
         }
      }
   }
}

unsigned GameState::storeFullGameState() {
   ast::GameState* gs = new ast::GameState();
   // Increment first.
   savedGameStates[curGameStateId] = gs;

   save(gs);

   return curGameStateId;
}

ast::GameState* GameState::getLastSavedGameState() {
   std::map<unsigned, ast::GameState*>::reverse_iterator iter;
   iter = savedGameStates.rbegin();
   if (iter == savedGameStates.rend())
      return NULL;
   else
      return iter->second;
}

ast::GameState* GameState::getSavedGameState(unsigned id) {
   std::map<unsigned, ast::GameState*>::iterator iter;
   iter = savedGameStates.find(id);
   if (iter == savedGameStates.end())
      return NULL;
   else
      return iter->second;
}

void GameState::setShip(AsteroidShip* newShip) {
   ship = newShip;
   ship->name = gameSettings->name;
   clientCommand.set_shipid(ship->id);

   if (minimap != NULL) {
      delete minimap;
   }

   minimap = new Minimap(ship);

   if (shipCamera != NULL) {
      delete shipCamera;
   }

   shipCamera = new Camera(ship);

   spring->attach(ship, shipCamera);
   usingShipCamera = true;
}

void GameState::removeNetworkPlayer(unsigned shipid) {
   Object3D* ship = custodian[shipid];
   if (ship != NULL) {
      ship->shouldRemove = true;
   }
}

void GameState::advancePhysics(double startTime, double endTime, std::set< std::pair<unsigned, unsigned> >* recordedCollisions) {
   double totalTimeDiff = endTime - startTime;
   // Ensure at least 50 fps in physics.
   const double maxTimeDiff = 0.02; // seconds;
   int numSteps = (int) ceil(fabs(totalTimeDiff) / maxTimeDiff);

   // Just in case.
   if (numSteps <= 0)
      numSteps = 1;

   double stepTimeDiff = totalTimeDiff / numSteps;

   // Set the gameTime to startTime, then increment time until we get to endTime.
   gameTime = startTime;
   
   std::vector<Object3D*>* objects = custodian.getListOfObjects();

   // Update each item.
   for (int i = 0; i < numSteps; ++i) {
      gameTime += stepTimeDiff;
      for (item = objects->begin(); item != objects->end(); ++item) {
         if (*item == NULL)
            continue;
         (*item)->update(stepTimeDiff);
         cube->constrain(*item);
         if (ship == *item && shipCamera != NULL)
            spring->update(stepTimeDiff);
      }

      // Add items that should be added, remove items that should be removed, update
      // positions in the sorted lists.
      custodian.update();

      if (stepTimeDiff >= 0 && gsm != ClientMode)
         testCollisions(recordedCollisions);
   }
}

void GameState::testCollisions(std::set< std::pair<unsigned, unsigned> >* recordedCollisions) {
   std::vector<Object3D*>* objects = custodian.getListOfObjects();
   std::set<CollisionBase*, compareByDistance>* collisions;
   std::set<CollisionBase*, compareByDistance>::iterator curCollision;
      
   for (item = objects->begin(); item != objects->end(); ++item) {
      collisions = custodian.findCollisions(*item, false);
      for (curCollision = collisions->begin(); curCollision != collisions->end();
       ++curCollision) {
         (*curCollision)->handleCollision();
         if (recordedCollisions != NULL) {
            recordedCollisions->insert(std::make_pair((*curCollision)->aId, (*curCollision)->bId));
            /*
            // Every 2 is a collision.
            curFrameCollisions->add_collider((*curCollision)->aId);
            curFrameCollisions->add_collider((*curCollision)->bId);
            */
         }
      }
      delete collisions;
      // TODO: fix memory leak.
   }

   
}

void GameState::setServerMode() {
   destruct();
   new (this) GameState(ServerMode); // Reconstruct this gamestate
   reset();
}

void GameState::setClientMode(char* serverAddr) {
   destruct();
   new (this) GameState(ClientMode); // Reconstruct this gameState
   connect(serverAddr);
   reset();
}

void GameState::setSingleMode() {
   destruct();
   new (this) GameState(SingleMode); // Reconstruct this gameState
   reset();
}

void GameState::setMusic() {
   if (gsm == MenuMode || !gameIsRunning)
      return;

   if (storeMenu->menuActive) {
      SoundEffect::playMusic("8-bit3.ogg");
   } else {
      const int numSongs = 3;
      const int curSong = (curLevel - 1) % numSongs;
      switch (curSong) {
         case 0: SoundEffect::playMusic("Asteroids2.ogg"); break;
         case 1: SoundEffect::playMusic("AsteroidsLevel2.ogg"); break;
         case 2: SoundEffect::playMusic("dingding"); break;
      }
   }
}

/**
 * This clears all the old saved game states in savedGameStates[].
 * This should only be called in server mode.
 */
void GameState::clearOldSavedGameStates() {
   if (gsm != ServerMode || curGameStateId <= 2) {
      return;
   }
   
   // This is the oldest gamestate to keep.
   unsigned oldestGameState = curGameStateId - 2;

   std::map<unsigned, ClientInfo*>& clients = serverSide->getClients();
   std::map<unsigned, ClientInfo*>::iterator clientIter = clients.begin();

   for (; clientIter != clients.end(); clientIter++) {
      ClientInfo* client = clientIter->second;
      if (client->ackGameState != 0 && client->ackGameState - 2 < oldestGameState) {
         oldestGameState = client->ackGameState - 2;
      }
   }

   std::map<unsigned, ast::GameState*>::iterator curState, endState;
   curState = savedGameStates.begin();
   endState = savedGameStates.upper_bound(oldestGameState);
   
   for (; curState != endState; ++curState) {
      delete curState->second;
   }

   savedGameStates.erase(savedGameStates.begin(), endState);
}
