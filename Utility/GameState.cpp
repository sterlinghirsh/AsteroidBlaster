/**
 * GameState - Stores the state of the game.
 * Lists of objects, etc.
 * @author Sterling Hirsh
 * @date 1-23-11
 * CPE 476
 */

#include <math.h>
#include <stdio.h>
#include "Utility/GameState.h"
#include "Utility/Matrix4.h"
#include "Utility/Music.h"
#include "Utility/SoundEffect.h"
#include "Particles/Particle.h"

extern double minimapSizeFactor;

std::ostringstream GameState :: sstream2;

GameState::GameState(double worldSizeIn) {
   godMode = false;
   bloom = false;
   bloom1 = false;
   reg = true;
   gameIsRunning = true;
   
   /* A view frustum culled list of objects to be used for drawing and by
      the shooting AI.
      */
   viewFrustumObjects = NULL;

   worldSize = worldSizeIn;
   skybox = new Skybox();
   ship = new AsteroidShip();
   minimap = new Minimap(ship);
   camera = new Camera(ship);
   cube = new BoundingSpace(worldSize / 2, 0, 0, 0);
   //sphere = new BoundingSphere(worldSize, 0, 0, 0);
   // Set up our text objects to be displayed on screen.
   curFPS = 0;

   // Init Text Objects
   SDL_Rect position = {0,0};
   std::string fontName = "Font/Slider.ttf";
   int fontSize = 18;
   
   FPSText = new Text("FPS: ", curFPS, "",  fontName, position, fontSize);
   numAsteroidsText = new Text("Asteroids Remaining: ", custodian.asteroidCount, "",  fontName, position, fontSize);
   numShardsText = new Text("Shards Remaining: ", custodian.shardCount, "",  fontName, position, fontSize);
   scoreText = new Text("Score: ", ship->getScore(), "",  fontName, position, fontSize);
   shardText = new Text("Shards: ", ship->getShards(), "",  fontName, position, fontSize);
   healthText = new Text("Health: ", ship->getHealth(), "",  fontName, position, fontSize);
   weaponText = new Text("Current Weapon: ", ship->getCurrentWeapon()->getName(), "",  fontName, position, fontSize);
   
   // Get the ammo into a stream to turn it into a string.
   sstream2 << ship->getCurrentWeapon()->curAmmo;
   ammoText = new Text("Ammo: ", sstream2.str(), "",  fontName, position, fontSize);
   // Clear the sstream2
   sstream2.str("");
   
   curLevelText = new Text("Level: ", curLevel, "",  fontName, position, fontSize);
   
   
   SDL_Rect centerRec = {GW/2, GH/2};
   gameOverText = new Text("GAME OVER", fontName, centerRec, 32);
   winText = new Text("YOU WIN!", fontName, centerRec, 32);


   // Improve the positioning code.
   weaponReadyBar = new ProgressBar(0.5, 0.1, -1.2, -0.3);
   healthBar = new ProgressBar(0.75, 0.05, -1, -0.3);

   // Set up objects.
   custodian.add(ship);
   curLevel = 1;
   numAsteroidsToSpawn = curLevel;
   initAsteroids();
   doYaw = 0;
   mouseX = 0;
   mouseY = 0;

   scoreToWin = 15000;
   godMode = false;

   // TODO: comment this or rename it.
   isW = isA = isS = isD = false;


   // Bloom
   xSize = ySize = 512;
   //colorBits = new char[ xSize * ySize * 3 ];
   //texture creation..
   glGenTextures(1, &hTexture);
   glBindTexture(GL_TEXTURE_2D, hTexture);

   glTexImage2D(GL_TEXTURE_2D, 0, 3, xSize,
         ySize, 0, GL_RGB,
         GL_UNSIGNED_BYTE, NULL);

   //you can set other texture parameters if you want
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glGenTextures(1, &vTexture);
   glBindTexture(GL_TEXTURE_2D, vTexture);

   glTexImage2D(GL_TEXTURE_2D, 0, 3, xSize,
         ySize, 0, GL_RGB,
         GL_UNSIGNED_BYTE, NULL);

   //you can set other texture parameters if you want

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glGenTextures(1, &blurTex);
   glBindTexture(GL_TEXTURE_2D, blurTex);

   glTexImage2D(GL_TEXTURE_2D, 0, 3, xSize,
         ySize, 0,GL_RGB,
         GL_UNSIGNED_BYTE, NULL);

   //you can set other texture parameters if you want

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


   //clean up
   //delete[] colorBits;
   //glBindTexture(GL_TEXTURE_2D, 0);

}

/**
 * Deconstructor: clean up all of our objects.
 */
GameState::~GameState() {
   delete skybox;
   delete ship;
   delete camera;
   delete cube;
   delete weaponReadyBar;
}

/**
 * This is the step function.
 */
void GameState::update(double timeDiff) {
   //check if it should go to the next level
   if(custodian.asteroidCount == 0) {
      nextLevel();
      return;
   }

   std::vector<Drawable*>* objects = custodian.getListOfObjects();
   std::set<Drawable*, compareByDistance>* collisions;
   std::set<Drawable*, compareByDistance>::iterator otherObject;

   // Determine whether or not the game should continue running
   if (ship->getHealth() <= 0) {
      gameIsRunning = false;
   } 
   /*
   else if (ship->getScore() >= scoreToWin) {
      gameIsRunning = false;
   }
   */


   cube->constrain(ship);
   for (item = objects->begin(); item != objects->end(); ++item) {
      if (*item == NULL)
         continue;
      (*item)->update(timeDiff);
      cube->constrain(*item);
   }
   // Update the values of all of the text objects.
   ship->keepFiring();

   custodian.update();
   // Get updated list.
   objects = custodian.getListOfObjects();

   Particle :: updateParticles(timeDiff);

   for (item = objects->begin(); item != objects->end(); ++item) {
      collisions = custodian.findCollisions(*item, false);
      for (otherObject = collisions->begin(); otherObject != collisions->end(); ++otherObject) {
         (*item)->handleCollision(*otherObject);
         (*otherObject)->handleCollision(*item);
      }
      delete collisions;
   }
   // Update all of the text seen on screen.
   updateText();
   // If the player lost, draw the game over text
   if (!gameIsRunning && ship->getHealth() <= 0) {
      SoundEffect::playSoundEffect("GameOver.wav");
   }
   weaponReadyBar->setAmount(ship->getCurrentWeaponCoolDown());
   healthBar->setAmount(ship->getHealth() / 100.0);
   cube->update(timeDiff);
   minimap->update(timeDiff);
}

/**
 * Draw objects in the minimap.
 */
void GameState::drawMinimap() {
   minimap->draw();
}

/**
 * Draw the main view.
 */
void GameState::draw() {
   /*
   if (thirdPerson) {
      Vector3D newOffset(ship->forward->scalarMultiply(-3));
      newOffset.addUpdate(ship->up->scalarMultiply(0.5));
      camera->setOffset(ship->getOffset);
   } else {
      camera->setOffset(0, 0, 0);
   }
   */
   camera->setViewVector(ship->getViewVector());
   camera->setOffset(*ship->getCameraOffset());

   //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   camera->setCamera(true);
   camera->shake(ship->getShakeAmount());
   skybox->draw(camera);
   cube->draw();

   // Get a list of all of the objects after culling them down to the view frustum.
   //std::list<Object3D*>* objects = ship->getRadar()->getViewFrustumReading();
   viewFrustumObjects = ship->getRadar()->getViewFrustumReading();
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
   ship->drawCrosshair();

   for (listIter = viewFrustumObjects->begin(); listIter != viewFrustumObjects->end(); ++listIter) {
      if (*listIter == NULL) {
         continue;
      } else if (*listIter == ship && 
       ship->getCurrentView() != VIEW_FIRSTPERSON_SHIP &&
       ship->getCurrentView() != VIEW_FIRSTPERSON_GUN) {
         // Don't draw the ship in first Person mode.
         (*listIter)->draw();
      } else {      
         (*listIter)->draw();
      }
   }
}

/**
 * Draw the glowing objects.
 */
void GameState::drawGlow() {
   if (ship->getCurrentView() == VIEW_THIRDPERSON_SHIP ||
    ship->getCurrentView() == VIEW_THIRDPERSON_GUN) {
      Vector3D newOffset(ship->forward->scalarMultiply(-3));
      newOffset.addUpdate(ship->up->scalarMultiply(0.5));
      camera->setOffset(newOffset);
   } else {
      camera->setOffset(0, 0, 0);
   }
   camera->setViewVector(ship->getViewVector());
   camera->setOffset(*ship->getCameraOffset());

   //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   camera->setCamera(true);
   //skybox->draw(camera);
   cube->draw();

   // Get a list of all of the objects after culling them down to the view frustum.
   //std::list<Object3D*>* objects = ship->getRadar()->getViewFrustumReading();
   viewFrustumObjects = ship->getRadar()->getViewFrustumReading();
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   /*
      if (thirdPerson) {
      ship->drawShotDirectionIndicators();
      } else {
      ship->drawCrosshair();
      }
      */

   for (listIter = viewFrustumObjects->begin(); listIter != viewFrustumObjects->end(); ++listIter) {
      if (*listIter == NULL || *listIter == ship)
         continue;
      (*listIter)->drawGlow();
   }

   // Don't draw the ship in first Person mode.
   if (ship->getCurrentView() == VIEW_THIRDPERSON_SHIP ||
    ship->getCurrentView() == VIEW_THIRDPERSON_GUN)
      ship->draw();
}

void GameState::hBlur() {
   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClear(GL_COLOR_BUFFER_BIT);
   useOrtho();
   glPushMatrix();
   //glBlendFunc(GL_ONE, GL_ONE);
   glDisable(GL_LIGHTING);
   float minY = -1.0;
   float maxY = 1.0;
   float minX = -1.0 * aspect;
   float maxX = 1.0 * aspect;
   int tex = hTexture;
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, tex);
   GLint texLoc = glGetUniformLocation(hBlurShader, "RTscene");
   glUseProgram(hBlurShader);
   glUniform1i(texLoc, tex);

   glColor4f(1.0, 1.0, 1.0, 0.5);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(minX, minY, 0.0);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(maxX, minY, 0.0);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(maxX, maxY, 0.0);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(minX, maxY, 0.0);
   glEnd();
   glUseProgram(0);
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);

   glBindTexture(GL_TEXTURE_2D, gameState->vTexture);

   glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
         0,0, GW, GH, 0);

   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPopMatrix();
   glBindTexture(GL_TEXTURE_2D, 0);
   //printf("h\n");
}

void GameState::vBlur() {
   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClear(GL_COLOR_BUFFER_BIT);
   useOrtho();
   glPushMatrix();
   //glBlendFunc(GL_ONE, GL_ONE);
   glDisable(GL_LIGHTING);
   float minY = -1.0;
   float maxY = 1.0;
   float minX = -1.0 * aspect;
   float maxX = 1.0 * aspect;
   int tex = vTexture;
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, vTexture);
   GLint texLoc = glGetUniformLocation(vBlurShader, "blurSampler");
   glUseProgram(vBlurShader);
   glUniform1i(texLoc, tex);

   glColor4f(1.0, 1.0, 1.0, 0.5);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(minX, minY, 0.0);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(maxX, minY, 0.0);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(maxX, maxY, 0.0);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(minX, maxY, 0.0);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);
   glPopMatrix();
   glUseProgram(0);
   
   glBindTexture(GL_TEXTURE_2D, gameState->blurTex);

   glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
         0,0, GW, GH, 0);

   glBindTexture(GL_TEXTURE_2D, 0);
   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //printf("v\n");
}

void GameState::drawBloom() {
   useOrtho();
   glPushMatrix();
   glBlendFunc(GL_ONE, GL_ONE);
   glDisable(GL_LIGHTING);
   float minY = -1.0;
   float maxY = 1.0;
   float minX = -1.0 * aspect;
   float maxX = 1.0 * aspect;
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, blurTex);

   glColor4f(1.0, 1.0, 1.0, 0.5);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0);
   glVertex3f(minX, minY, 0.0);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(maxX, minY, 0.0);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(maxX, maxY, 0.0);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(minX, maxY, 0.0);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);
   glPopMatrix();
   
   glBindTexture(GL_TEXTURE_2D, 0);
}

void GameState::drawHud() {
   useOrtho();

   /* Set the camera using the location of your eye,
    * the location where you're looking at, and the up vector.
    * The camera is set to be just 0.25 behind where you're looking at.
    */
   gluLookAt(0, 0, 0.25, 0, 0, 0, 0, 1, 0);

   /* Use orthonormal view so the text stays perpendicular
    * to the camera at all times.
    */

   /* We need to disable the lighting temporarily
    * in order to set the color properly.
    */
   glDisable(GL_LIGHTING);
   glDisable(GL_CULL_FACE);
   drawAllText();
   weaponReadyBar->draw();
   healthBar->draw();
   glEnable(GL_LIGHTING);
   //glEnable(GL_DEPTH_TEST);
   usePerspective();
   glPopMatrix();
   glEnable(GL_CULL_FACE);
}

/**
 * Draw all of the text in the allTexts list to the screen.
 * This function should be called once per display loop.
 * We do all of the looking at, lighting changes, etc.
 * one time here to improve efficiency.
 */
void GameState::drawAllText() {
   
   SDL_Rect position;
   position.x = 10;
   position.y = 30;

   /* Don't draw stuff in front of the text. */
   //glDisable(GL_DEPTH_TEST);

   // If the player lost, draw the game over text
   if (!gameIsRunning && ship->getHealth() <= 0) {
      gameOverText->draw();
   } else if (!gameIsRunning && ship->getHealth() > 0) {
      // If the player won, draw the win text
      winText->draw();
   }

   // Draw all of the Text objects.
   FPSText->setPosition(position);
   FPSText->draw();
   
   position.y += 15;
   numAsteroidsText->setPosition(position);
   numAsteroidsText->draw();

   position.y += 15;
   numShardsText->setPosition(position);
   numShardsText->draw();
   
   position.y += 15;
   scoreText->setPosition(position);
   scoreText->draw();
   
   position.y += 15;
   shardText->setPosition(position);
   shardText->draw();
   
   position.y += 15;
   healthText->setPosition(position);
   healthText->draw();
   
   position.y += 15;
   weaponText->setPosition(position);
   weaponText->draw();
   
   position.y += 15;
   ammoText->setPosition(position);
   ammoText->draw();
   
   position.y += 15;
   curLevelText->setPosition(position);
   curLevelText->draw();

}

/**
 * Update the values contained in all of the texts.
 */
void GameState::updateText() {
   FPSText->updateBody(curFPS);
   numAsteroidsText->updateBody(custodian.asteroidCount);
   numShardsText->updateBody(custodian.shardCount);
   scoreText->updateBody(ship->getScore());
   shardText->updateBody(ship->getShards());
   healthText->updateBody(ship->getHealth());
   weaponText->updateBody(ship->getCurrentWeapon()->getName());
   // If the gun has infinite ammo, say so.
   if(ship->getCurrentWeapon()->curAmmo == -1)
      ammoText->updateBody("Inf");
   else {
      sstream2 << ship->getCurrentWeapon()->curAmmo;
      ammoText->updateBody(sstream2.str());
      sstream2.str("");
   }
}


void GameState::initAsteroids() {
   Asteroid3D* tempAsteroid;
   std::set<Drawable*, compareByDistance>* collisions;

   /* We want this spaceHolder because we don't want to spawn asteroids
    * too close to the ship.
    */
   Object3D* spaceHolder = new Object3D(0, 0, 0, 0);
   spaceHolder->minX = spaceHolder->minY = spaceHolder->minZ = -10;
   spaceHolder->maxX = spaceHolder->maxY = spaceHolder->maxZ = 10;
   custodian.add(spaceHolder);

   int numCollisions = 0;
   // Spawn the initial asteroids for the game.
   for (int i = 0; i < numAsteroidsToSpawn; ++i) {
      tempAsteroid = new Asteroid3D(5 + 5 * randdouble(), worldSize);
      // Add each asteroid to the custodian so we know of its existence.
      custodian.add(tempAsteroid);
      do {
         custodian.update();
         collisions = custodian.findCollisions(tempAsteroid, true);
         numCollisions = collisions->size();
         if (numCollisions > 0) {
            tempAsteroid->newRandomPosition();
         }
         delete collisions;
      } while (numCollisions > 0);
   }
   custodian.remove(spaceHolder);
   custodian.update();
}

void GameState::setCurFPS(double fpsIn) {
   curFPS = fpsIn;
}

/**
 * Tells whether or not the game is currently running (not game over or won)
 */
bool GameState::isGameRunning() {
   return gameIsRunning;
}

/**
 * Reset everything in the game to play again
 */
void GameState::reset() {
   //delete ship;
   delete camera;
   delete cube;
   delete ship;
   delete minimap;
   
   custodian.clear();
   Particle::particles.clear();
   
   cube = new BoundingSpace(worldSize / 2, 0, 0, 0);
   ship = new AsteroidShip();
   cube->constrain(ship);
   minimap = new Minimap(ship);
   camera = new Camera(ship);
   gameIsRunning = true;

   custodian.add(ship);
   initAsteroids();
}

void GameState::nextLevel() {
   SoundEffect::stopAllSoundEffect();
   Music::stopMusic();
   Music::playMusic("Asteroids2.ogg");
   storeMenu->menuActive = true;

   minimap = new Minimap(ship);
   gameIsRunning = true;
   curLevel++;
   curLevelText->updateBody(curLevel);
   numAsteroidsToSpawn = curLevel;
   printf("Level'd up to %d!\n",curLevel);
   initAsteroids();
}


/**
 * Handles the player pressing down a key
 */
void GameState::keyDown(int key) {
   switch(key) {

   case SDLK_w:
      isW = true;
      if(!ship->flyingAI->isEnabled()) {
         if (isS) {
            ship->accelerateForward(0);
         } else {
            ship->accelerateForward(1);
         }
      }
      break;

   case SDLK_s:
      isS = true;
      if(!ship->flyingAI->isEnabled()) {
         if (isW) {
            ship->accelerateForward(0);
         } else {
            ship->accelerateForward(-1);
         }
      }
      break;

   case SDLK_a:
      isA = true;
      if(!ship->flyingAI->isEnabled()) {
         if (isD) {
            //printf("should be not turning left\n");
            ship->setYawSpeed(0.0);
         } else {
            //printf("should be turning left\n");
            ship->setYawSpeed(1.0);
         }
      }
      break;

   case SDLK_d:
      isD = true;
      if(!ship->flyingAI->isEnabled()) {
         if (isA) {
            //printf("should be not turning right\n");
            ship->setYawSpeed(0.0);
         } else {
            //printf("should be turning right\n");
            ship->setYawSpeed(-1.0);
         }
      }
      break;

   case SDLK_q:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateRight(-1);
      break;

   case SDLK_e:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateRight(1);
      break;

   case SDLK_9:
      camera->zoom();
      break;

   case SDLK_SPACE:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateUp(1);
      break;

   case SDLK_LCTRL:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateUp(-1);
      break;

      //case SDLK_LSHIFT:
   case SDLK_RSHIFT:
      doYaw = !doYaw;
      ship->setRollSpeed(0);
      break;

   case SDLK_LSHIFT:
      if(!ship->flyingAI->isEnabled())
         ship->setBoost(true);
      break;

   case SDLK_b:
      ship->setBrake(true);
      break;

   case SDLK_ESCAPE:
      exit(0);
      break;

   case SDLK_t:
      ship->nextView();
      break;

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
         ship->accelerateForward(0);
         ship->accelerateRight(0);
         ship->accelerateUp(0);
         ship->setYawSpeed(0.0);
         ship->setPitchSpeed(0.0);
         ship->setRollSpeed(0.0);
      } else {
         // Stop accelerating the ship when turning on the flyingAI as well.
         ship->accelerateForward(0);
         ship->accelerateRight(0);
         ship->accelerateUp(0);
         ship->setYawSpeed(0.0);
         ship->setPitchSpeed(0.0);
         ship->setRollSpeed(0.0);
         ship->flyingAI->enable();
      }
      break;

   case SDLK_v:
      // If we're in godMode, ignore whether or not a weapon is purchased
      if(gameState->godMode)
         ship->prevWeapon();
      else {
         // Keep scrolling through weapons as long as they're not purchased.
         do {
            ship->nextWeapon();
         } while (!ship->getCurrentWeapon()->purchased);
      }
      break;
   case SDLK_F10:
      gameState->ship->nShards += 1;
      break;
   case SDLK_z:
      // If we're in godMode, ignore whether or not a weapon is purchased
      if(gameState->godMode)
         ship->prevWeapon();
      else {
         // Keep scrolling through weapons as long as they're not purchased.
         do {
            ship->prevWeapon();
         } while (!ship->getCurrentWeapon()->purchased);
      }
      break;
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
      minimap->adjustZoomDirection = 1;
      break;
   case SDLK_4:
      minimap->adjustZoomDirection = -1;
      break;

   case SDLK_KP_ENTER:
      if (Mix_PausedMusic()) {
         Music::resumeMusic();
      } else {
         Music::pauseMusic();
      }
      break;
      // If the user presses F11, give them all of the weapons.
   case SDLK_F11:
      // Make all of the weapons be purchased.
      for (int i = 0; i < ship->getNumWeapons(); i++) {
         ship->getWeapon(i)->purchased = true;
         ship->getWeapon(i)->curAmmo += 500;
      }
      break;
   case SDLK_F2:
      bloom = !bloom;
      break;
   case SDLK_F3:
      reg = !reg;
      break;
   case SDLK_F4:
      bloom1 = !bloom1;
      break;
   case SDLK_F12:
      godMode = !godMode;
      printf("Zoe mode: %d\n", godMode);
      break;
   case SDLK_BACKQUOTE:
      toggleGrabMode();
      break;
   }
}

/**
 * Handles the player letting go of a key
 */
void GameState::keyUp(int key) {
   switch(key) {

   case SDLK_r:
      reset();
      break;

   case SDLK_s:
      isS = false;
      if(!ship->flyingAI->isEnabled()) {
         if (isW) {
            ship->accelerateForward(1);
         } else {
            ship->accelerateForward(0);
         }
      }
      break;

   case SDLK_w:
      isW = false;
      if(!ship->flyingAI->isEnabled()) {
         if (isS) {
            ship->accelerateForward(-1);
         } else {
            ship->accelerateForward(0);
         }
      }
      break;

   case SDLK_a:
      isA = false;
      if(!ship->flyingAI->isEnabled()) {
         if (isD) {
            ship->setYawSpeed(-1.0);
         } else {
            ship->setYawSpeed(0.0);
         }
      }
      break;

   case SDLK_d:
      isD = false;
      if(!ship->flyingAI->isEnabled()) {
         if (isA) {
            ship->setYawSpeed(1.0);
         } else {
            ship->setYawSpeed(0.0);
         }
      }
      //if(!ship->flyingAI->isEnabled())
      //ship->setYawSpeed(0);
      break;

   case SDLK_q:
   case SDLK_e:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateRight(0);
      break;

   case SDLK_SPACE:
   case SDLK_LCTRL:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateUp(0);
      break;

   case SDLK_LSHIFT:
      if(!ship->flyingAI->isEnabled())
         ship->setBoost(false);
      break;

   case SDLK_b:
      ship->setBrake(false);
      break;
   // Minimap Size
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
   }
}

/**
 * Handles the player clicking the mouse
 */
void GameState::mouseDown(int button) {
   switch(button) {
      // Left mouse down
   case 1:
      //ship->selectWeapon(0);
      ship->fire(true);
      break;

      // Right mouse down
   case 3:
      doYaw = !doYaw;
      ship->setRollSpeed(0);
      //printf("right mouse down\n");
      break;

   case 4:
      // If we're in godMode, ignore whether or not a weapon is purchased
      if(gameState->godMode)
         ship->prevWeapon();
      else {
         // Keep scrolling through weapons as long as they're not purchased.
         do {
            ship->prevWeapon();
         } while (!ship->getCurrentWeapon()->purchased);
      }
      break;
   case 5:
      // If we're in godMode, ignore whether or not a weapon is purchased
      if(gameState->godMode)
         ship->nextWeapon();
      else {
         // Keep scrolling through weapons as long as they're not purchased.
         do {
            ship->nextWeapon();
         } while (!ship->getCurrentWeapon()->purchased);
      }
      break;
   }
}

/**
 * Handles the player letting go of a mouse click
 */
void GameState::mouseUp(int button) {
   switch (button) {
      // Left mouse up
   case 1:
      ship->fire(false);
      break;
      // Right mouse up
   case 3:
      doYaw = !doYaw;
      ship->setRollSpeed(0);
      break;
   }
}

void GameState::mouseMove(int dx, int dy, int x, int y) {
   double worldX = p2wx(x);
   double worldY = p2wy(y);

   mouseX = worldX;
   mouseY = worldY;

   ship->updateShotDirection(worldX, worldY);

   worldX = clamp(worldX * fabs(worldX), -1, 1);
   worldY = clamp(-worldY * fabs(worldY), -1, 1);

   if(!ship->flyingAI->isEnabled()) {
      if (doYaw) {
         ship->setYawSpeed(-worldX);
      }
      else {
         ship->setRollSpeed(worldX);
      }

      ship->setPitchSpeed(worldY);
   }

}

double GameState::getWallxMin() { return cube->xMin; }
double GameState::getWallxMax() { return cube->xMax; }
double GameState::getWallyMin() { return cube->yMin; }
double GameState::getWallyMax() { return cube->yMax; }
double GameState::getWallzMin() { return cube->zMin; }
double GameState::getWallzMax() { return cube->zMax; }

double GameState::getMouseX() { return mouseX; }
double GameState::getMouseY() { return mouseY; }
Camera* GameState::getCamera() { return camera; }

