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

GameState::GameState(double worldSizeIn) {
  gameIsRunning = true;
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
  healthText = new BitmapTextDisplay("Health: ", ship->getHealth(), "", 10, 80);
  gameOverText = new BitmapTextDisplay("GAME OVER", GW/2, GH/2);
  winText = new BitmapTextDisplay("YOU WIN!", GW/2, GH/2);

  // Set up objects.
  custodian.add(ship);
  initAsteroids();
  doYaw = 0;
  mouseX = 0;
  mouseY = 0;

  scoreToWin = 10000;
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
   if(ship->getHealth() <= 0) {
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
  
  glPushMatrix();
     Vector3D oppositeOfPosition(*(ship->position));
     // Translate everything so that the ship is at 0, 0 and everything is centered there.
     oppositeOfPosition.updateMagnitude(oppositeOfPosition);
     glScalef(0.05, 0.05, 0.05);
     glRotatef(180, 0, 1, 0);
     oppositeOfPosition.glTranslate(-1);
     for (listIter = objects->begin(); listIter != objects->end(); ++listIter) {
       if (*listIter == NULL)
         continue;
       (*listIter)->drawInMinimap();
     }
  glPopMatrix();

}

void GameState::draw() {
   std::vector<Object3D*>* objects = custodian.getListOfObjects();
   // Draw all of the text objects to the screen.
   drawAllText();
   if (thirdPerson) {
      Vector3D newOffset(ship->forward->scalarMultiply(-3));
      newOffset.addUpdate(ship->up->scalarMultiply(0.5));
      camera->setOffset(newOffset);
   } else {
      camera->setOffset(0, 0, 0);
   }
   
   camera->setCamera(true);
   skybox->draw(camera);
   cube->draw();
   for (item = objects->begin(); item != objects->end(); ++item) {
      if (*item == NULL)
         continue;
      // Don't draw the ship in thirdPerson mode.
      if (thirdPerson || (*item != ship)) {
         (*item)->draw();
      }
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
   if(!gameIsRunning && ship->getHealth() <= 0){
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
   healthText->draw();

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
  healthText->updateBody(ship->getHealth());
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
  for (int i = 0; i < 15; ++i) {
    tempAsteroid = new Asteroid3D(5 + 5 * randdouble(), worldSize);
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
    ship->accelerateForward(1);
    break;

  case SDLK_s:
    ship->accelerateForward(-1);
    break;

  case SDLK_a:
    ship->setYawSpeed(1.0);
    break;

  case SDLK_d:
    ship->setYawSpeed(-1.0);
    break;

  case SDLK_q:
    ship->accelerateRight(-1);
    break;

  case SDLK_e:
    ship->accelerateRight(1);
    break;

  case SDLK_SPACE:
    ship->accelerateUp(1);
    break;

  case SDLK_LCTRL:
    ship->accelerateUp(-1);
    break;

  case SDLK_LSHIFT:
  case SDLK_RSHIFT:
    doYaw = !doYaw;
    ship->setRollSpeed(0);
    break;
    
  case SDLK_p:
    std::cout << "meh: (" << ship->position->x << "," << ship->position->y << "," << ship->position->z << ")" << std::endl;
    Particle::Add(new Point3D(*ship->position), new Vector3D(1,0,0));
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
    //TODO: ship->enableFlyingAI();
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
    ship->accelerateForward(0);
    break;

  case SDLK_a:
  case SDLK_d:
    ship->setYawSpeed(0);
    break;

  case SDLK_q:
  case SDLK_e:
    ship->accelerateRight(0);
    break;

  case SDLK_SPACE:
  case SDLK_LCTRL:
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
    ship->selectWeapon(0);
    break;

  case 3:
    ship->selectWeapon(1);
    break;
  }
  ship->fire(true);
}

/**
 * Handles the player letting go of a mouse click
 */
void GameState::mouseUp(int button) {
  ship->fire(false);
}

void GameState::mouseMove(int dx, int dy, int x, int y) {
  double worldX = p2wx(x);
  double worldY = p2wy(y);

  mouseX = worldX;
  mouseY = worldY;

  ship->updateShotDirection(worldX, worldY);

  worldX = clamp(worldX * fabs(worldX), -1, 1);
  worldY = clamp(-worldY * fabs(worldY), -1, 1);

  if (doYaw) {
    ship->setYawSpeed(-worldX);
  }
  else {
    ship->setRollSpeed(worldX);
  }

  ship->setPitchSpeed(worldY);

}


double GameState::getMouseX() { return mouseX; }
double GameState::getMouseY() { return mouseY; }
