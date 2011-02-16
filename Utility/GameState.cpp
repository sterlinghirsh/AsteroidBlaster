/**
 * GameState - Stores the state of the game.
 * Lists of objects, etc.
 * @author Sterling Hirsh
 * @date 1-23-11
 * CPE 476
 */

#include "Utility/GameState.h"
#include "Graphics/GlutUtility.h"
#include <math.h>
#include "Utility/Matrix4.h"

extern double minimapSizeFactor;

GameState::GameState(double worldSizeIn) {
   gameIsRunning = true;
   menuMode = true;
   worldSize = worldSizeIn;
   skybox = new Skybox("Images/stars.bmp");
   ship = new AsteroidShip();
   camera = new Camera(ship);
   cube = new BoundingSpace(worldSize / 2, 0, 0, 0);
   // Set up our text objects to be displayed on screen.
   curFPS = 0;

   // Init Text Objects
   FPSText = new BitmapTextDisplay("FPS: ", curFPS, "", 10, 20);
   numAsteroidsText = new BitmapTextDisplay("Asteroids Remaining: ", custodian.asteroidCount, "", 10, 40);
   scoreText = new BitmapTextDisplay("Score: ", ship->getScore(), "", 10, 60);
   shardText = new BitmapTextDisplay("Shards: ", ship->getScore(), "", 10, 80);
   healthText = new BitmapTextDisplay("Health: ", ship->getHealth(), "", 10, 100);
   gameOverText = new BitmapTextDisplay("GAME OVER", GW/2, GH/2);
   winText = new BitmapTextDisplay("YOU WIN!", GW/2, GH/2);
   weaponText = new BitmapTextDisplay("Current Weapon: ", ship->getCurrentWeapon()->getName(),
         "", 10, 120);

   // Set up objects.
   custodian.add(ship);
   numAsteroidsToSpawn = 10;
   initAsteroids();
   doYaw = 0;
   mouseX = 0;
   mouseY = 0;

   scoreToWin = 15000;
   thirdPerson = false;

}

/**
 * Deconstructor: clean up all of our objects.
 */
GameState::~GameState() {
   delete skybox;
   delete ship;
   delete camera;
   delete cube;
}

void GameState::update(double timeDiff) {
   std::vector<Object3D*>* objects = custodian.getListOfObjects();
   std::set<Object3D*>* collisions;
   std::set<Object3D*>::iterator otherObject;

   // Determine whether or not the game should continue running
   if (ship->getHealth() <= 0) {
      gameIsRunning = false;
   } else if (ship->getScore() >= scoreToWin) {
      gameIsRunning = false;
   }


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

   Particle::update(timeDiff);

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
}

/**
 * Draw objects in the minimap.
 */
void GameState::drawInMinimap() {
   // Get a reading of all the nearby Object3Ds from the Radar
   std::list<Object3D*>* objects = ship->getRadar()->getNearbyReading();
   static Matrix4 modelViewMatrix;
   static Point3D objectPosition;
   static Vector3D positionVector;
   const float scaleFactor = 0.05;
   double radius2D; // Radius when an object is projected onto the forward-right plane of the ship.
   double radius3D; // Radius from the ship in 3D space.

   positionVector.updateMagnitude(*ship->position);

   glPushMatrix();
   ship->glRotate(false);
   Vector3D oppositeOfPosition(*(ship->position));

   // Translate everything so that the ship is at 0, 0 and everything is centered there.
   oppositeOfPosition.updateMagnitude(oppositeOfPosition);
   glScalef(scaleFactor, scaleFactor, scaleFactor);
   //glRotatef(180, 0, 1, 0);
   oppositeOfPosition.glTranslate(-1);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // For each item that needs to be drawn in the minimap
   for (listIter = objects->begin(); listIter != objects->end(); ++listIter) {
      // Make sure it's not null, & then draw it in the minimap
      if (*listIter != NULL)
         (*listIter)->drawInMinimap();
   }
   glPopMatrix();
   // Now draw the lines.
   // Load just the rotation matrix.
   glPushMatrix();
   glLoadIdentity(); // We don't want the camera transform.
   ship->glRotate(true);
   modelViewMatrix.loadModelviewMatrix();
   glPopMatrix();
   glPushMatrix();
   glScalef(scaleFactor, scaleFactor, scaleFactor);
   materials(WhiteSolid);
   for (listIter = objects->begin(); listIter != objects->end(); ++listIter) {
      // Make sure it's not null, & then draw it in the minimap
      if (*listIter != NULL && (*listIter)->shouldDrawInMinimap && *listIter != ship) {
         objectPosition = *(*listIter)->position; // Get the position.
         positionVector.movePoint(objectPosition, -1); // Center on the ship
         objectPosition = modelViewMatrix * (objectPosition); // Rotate about the ship
         radius2D = distance2D(objectPosition.x, objectPosition.z);
         radius3D = distance3D(objectPosition.x, objectPosition.y, objectPosition.z);
         glDisable(GL_COLOR_MATERIAL);
         if (radius3D < 20) {
            glColor3f(1, 0, 0);
            materials(RedTransparent);
         } else if (radius3D < 40) {
            glColor3f(1, 1, 0);
            materials(YellowTransparent);
         } else {
            glColor3f(0, 1, 0);
            materials(GreenTransparent);
         }

         // Draw a disc.
         // This is the way that one game did it with the circles on a plane.
         glPushMatrix();
         glEnable(GL_LIGHTING);
         glRotatef(-90.0,1.0f,0.0f,0.0f);   // Rotate By 0 On The X-Axis
         gluDisk(quadric, radius2D - 1, radius2D + 1 ,16,2);
         glDisable(GL_LIGHTING);
         glPopMatrix();
         //glDisable(GL_LIGHTING);
         glBegin(GL_LINES);
         // Draw a point at the object's position.
         glVertex3f(objectPosition.x, objectPosition.y, objectPosition.z);
         // Draw the other point at the object's position without the Y component.
         glVertex3f(objectPosition.x, 0, objectPosition.z);
         glEnd();
      }
   }
   glPopMatrix();
}

/**
 * Draw the main view.
 */
void GameState::draw() {
   // Draw all of the text objects to the screen.
   drawAllText();

   if (thirdPerson) {
      Vector3D newOffset(ship->forward->scalarMultiply(-3));
      newOffset.addUpdate(ship->up->scalarMultiply(0.5));
      camera->setOffset(newOffset);
   } else {
      camera->setOffset(0, 0, 0);
   }

   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   camera->setCamera(true);
   skybox->draw(camera);
   cube->draw();

   // Get a list of all of the objects after culling them down to the view frustum.
   std::list<Object3D*>* objects = ship->getRadar()->getFullReading();
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   for (listIter = objects->begin(); listIter != objects->end(); ++listIter) {
      if (*listIter == NULL)
         continue;
      // Don't draw the ship in thirdPerson mode.
      if (thirdPerson || (*listIter != ship)) {
         (*listIter)->draw();
      }
   }

   if (thirdPerson) {
      ship->drawShotDirectionIndicators();
   } else {
      ship->drawCrosshair();
   }
}

/**
 * Draw all of the text in the allTexts list to the screen.
 * This function should be called once per display loop.
 * We do all of the looking at, lighting changes, etc.
 * one time here to improve efficiency.
 */
void GameState::drawAllText() {
   glPushMatrix();
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

   /* Don't draw stuff in front of the text. */
   //glDisable(GL_DEPTH_TEST);

   // If the player lost, draw the game over text
   if (!gameIsRunning && ship->getHealth() <= 0) {
      gameOverText->draw();
   }

   // If the player won, draw the win text
   else if (!gameIsRunning && ship->getHealth() > 0) {
      winText->draw();
   }

   // Draw all of the BitmapTextDisplay objects.
   FPSText->draw();
   numAsteroidsText->draw();
   scoreText->draw();
   shardText->draw();
   healthText->draw();
   weaponText->draw();

   glEnable(GL_LIGHTING);
   //glEnable(GL_DEPTH_TEST);
   usePerspective();
   glPopMatrix();
}

/**
 * Update the values contained in all of the texts.
 */
void GameState::updateText() {
   FPSText->updateBody(curFPS);
   numAsteroidsText->updateBody(custodian.asteroidCount);
   scoreText->updateBody(ship->getScore());
   shardText->updateBody(ship->getShards());
   healthText->updateBody(ship->getHealth());
   weaponText->updateBody(ship->getCurrentWeapon()->getName());
}

void GameState::checkCollisions() {
}

void GameState::initAsteroids() {
   Asteroid3D* tempAsteroid;
   std::set<Object3D*>* collisions;

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

bool GameState::getMenuMode() {
   return menuMode;
}

/**
 * Reset everything in the game to play again
 */
void GameState::reset() {
   delete ship;
   delete camera;
   custodian.clear();

   ship = new AsteroidShip();
   camera = new Camera(ship);
   gameIsRunning = true;

   custodian.add(ship);
   initAsteroids();
}

/**
 * Handles the player pressing down a key
 */
void GameState::keyDown(int key) {
   switch(key) {
   case SDLK_w:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateForward(1);
      break;

   case SDLK_s:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateForward(-1);
      break;

   case SDLK_a:
      if(!ship->flyingAI->isEnabled())
         ship->setYawSpeed(1.0);
      break;

   case SDLK_d:
      if(!ship->flyingAI->isEnabled())
         ship->setYawSpeed(-1.0);
      break;

   case SDLK_q:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateRight(-1);
      break;

   case SDLK_e:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateRight(1);
      break;

   case SDLK_m:
      menuMode = true;
      break;

   case SDLK_SPACE:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateUp(1);
      break;

   case SDLK_LCTRL:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateUp(-1);
      break;

   case SDLK_LSHIFT:
   case SDLK_RSHIFT:
      doYaw = !doYaw;
      ship->setRollSpeed(0);
      break;

   case SDLK_b:
      ship->setBrake(true);
      break;

   case SDLK_ESCAPE:
      exit(0);
      break;

   case SDLK_t:
      thirdPerson = !thirdPerson;
      break;

   case SDLK_g:
      //TODO: Based on how many shooting AIs this ship has, activate the correct one.
      if(ship->shooter->isEnabled())
         ship->shooter->disable();
      else
         ship->shooter->enable();

      break;

   case SDLK_f:
      if(ship->flyingAI->isEnabled())
         ship->flyingAI->disable();
      else
         ship->flyingAI->enable();
      break;

   case SDLK_v:
      ship->nextWeapon();
      break;
   case SDLK_z:
      ship->prevWeapon();
      break;
   case SDLK_1:
      minimapSizeFactor = 1;
      break;
   case SDLK_2:
      minimapSizeFactor = 0.5;
      break;
   case SDLK_3:
      minimapSizeFactor = 0.333;
      break;
   case SDLK_4:
      minimapSizeFactor = 0.25;
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
   case SDLK_w:
      if(!ship->flyingAI->isEnabled())
         ship->accelerateForward(0);
      break;

   case SDLK_a:
   case SDLK_d:
      if(!ship->flyingAI->isEnabled())
         ship->setYawSpeed(0);
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

   case SDLK_b:
      ship->setBrake(false);
      break;
   }
}

/**
 * Handles the player clicking the mouse
 */
void GameState::mouseDown(int button) {
   switch(button) {
   case 1:
      //ship->selectWeapon(0);
      ship->fire(true);
      break;

   case 3:
      doYaw = !doYaw;
      ship->setRollSpeed(0);
      printf("right mouse down\n");
      break;

   case 4:
      ship->prevWeapon();
      break;
   case 5:
      ship->nextWeapon();
      break;
   }
   //ship->fire(true);
}

/**
 * Handles the player letting go of a mouse click
 */
void GameState::mouseUp(int button) {
   switch (button) {
   case 1:
      ship->fire(false);
      break;
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


double GameState::getMouseX() { return mouseX; }
double GameState::getMouseY() { return mouseY; }
Camera* GameState::getCamera() { return camera; }

void GameState::menuFunc() {
   glDisable(GL_TEXTURE_2D);
   glClearColor(0.0, 0.0, 0.0, 1.0);
   // Enable the cursor
   SDL_ShowCursor(SDL_ENABLE);
   BitmapTextDisplay* newGame = new BitmapTextDisplay("New Game", GW/2 - 50, GH/2 - 60);
   BitmapTextDisplay* saveLoadGame = new BitmapTextDisplay("Save/Load Game", GW/2 - 50, GH/2 - 20);
   BitmapTextDisplay* settings = new BitmapTextDisplay("Settings", GW/2 - 50, GH/2 + 20);
   BitmapTextDisplay* quit = new BitmapTextDisplay("Quit", GW/2 - 50, GH/2 + 60);

   newGame->setFont(GLUT_BITMAP_TIMES_ROMAN_24);
   saveLoadGame->setFont(GLUT_BITMAP_TIMES_ROMAN_24);
   settings->setFont(GLUT_BITMAP_TIMES_ROMAN_24);
   quit->setFont(GLUT_BITMAP_TIMES_ROMAN_24);


   int x = 0;
   int y = 0;

   SDL_Event event;
   while (getMenuMode()) {
      // Clear the screen
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      //draw the text
      glPushMatrix();
      useOrtho();
      glDisable(GL_LIGHTING);
      newGame->draw();
      saveLoadGame->draw();
      settings->draw();
      quit->draw();
      glEnable(GL_LIGHTING);
      usePerspective();
      glPopMatrix();

      //draw the mouse icon
      //glPushMatrix();
      //glTranslatef(x,0,y);
      //gluSphere( quadric , .1 , 36 , 18 );
      //glPopMatrix();

      while (SDL_PollEvent(&event)) {
         if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.motion.x >= 350 &&
                  event.motion.x <= 460 &&
                  event.motion.y >= 225 &&
                  event.motion.y <= 245) {
               gameState->menuMode = false;
               gameState->reset();
            } else if (event.motion.x >= 350 &&
                  event.motion.x <= 450 &&
                  event.motion.y >= 345 &&
                  event.motion.y <= 365) {
               exit(0);
            }
         }
         if (event.type == SDL_MOUSEMOTION) {
            x = event.motion.x;
            y = event.motion.y;
            if (x >= 350 &&
                  x <= 450 &&
                  y >= 230 &&
                  y <= 240) {
               newGame->setColor(1.0,0.0,0.0);
            } else {
               newGame->setColor(1.0,1.0,1.0);
            }

            if (x >= 350 &&
                  x <= 450 &&
                  y >= 345 &&
                  y <= 365) {
               quit->setColor(1.0,0.0,0.0);
            } else {
               quit->setColor(1.0,1.0,1.0);
            }

         }

         if ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE ) {
            exit(0);
         }
      }

      SDL_GL_SwapBuffers();
   }
   glEnable(GL_TEXTURE_2D);

   // Disable the cursor
   SDL_ShowCursor(SDL_DISABLE);
}


