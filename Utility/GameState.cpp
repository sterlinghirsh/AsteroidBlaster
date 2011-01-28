/**
 * GameState - Stores the state of the game.
 * Lists of objects, etc.
 * @author Sterling Hirsh
 * @date 1-23-11
 * CPE 476
 */

#include "Utility/GameState.h"
#include "Graphics/GlutUtility.h"

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
   numAsteroidsText = new BitmapTextDisplay("Asteroids Remaining: ", (int)asteroids.size(), "", 10, 40);
   scoreText = new BitmapTextDisplay("Score: ", ship->getScore(), "", 10, 60);
   healthText = new BitmapTextDisplay("Health: ", ship->getHealth(), "", 10, 80);
   gameOverText = new BitmapTextDisplay("GAME OVER", GW/2, GH/2);
   winText = new BitmapTextDisplay("YOU WIN!", GW/2, GH/2);
   
   // Set up objects.
   custodian.add(ship);
   initAsteroids();
}

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

   if(ship->getHealth() <= 0) {
      gameIsRunning = false;
   } else if (ship->getScore() >= 2000) {
      gameIsRunning = false;
   }
   

   //ship->updatePosition(timeDiff);
   ship->keepFiring();
   cube->constrain(ship);
   for (item = objects->begin(); item != objects->end(); ++item) {
      (*item)->update(timeDiff);
      cube->constrain(*item);
   }
   // Update the values of all of the text objects.
   
   custodian.update();
   
   for (item = objects->begin(); item != objects->end(); ++item) {
      collisions = custodian.findCollisions(*item, false);
      for (otherObject = collisions->begin(); otherObject != collisions->end(); ++otherObject) {
         (*item)->handleCollision(*otherObject);
         (*otherObject)->handleCollision(*item);
      }
      delete collisions;
   }

   updateText();
}

void GameState::draw() {
   std::vector<Object3D*>* objects = custodian.getListOfObjects();
   // Draw all of the text objects to the screen.
   drawAllText();
   camera->setCamera(true);
   skybox->draw(camera);
   cube->draw();
   for (item = objects->begin(); item != objects->end(); ++item) {
      (*item)->draw();
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
   
   // Draw all of the BitmapTextDisplay objects.
   if(!gameIsRunning && ship->getHealth() <= 0){
      gameOverText->draw();
   }
   else if (!gameIsRunning && ship->getHealth() > 0) {
      winText->draw();
   }
   
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
 * Clear the list, then re-make it.
 */
void GameState::updateText() {
   FPSText->updateBody(curFPS);
   numAsteroidsText->updateBody((int)asteroids.size());
   scoreText->updateBody(ship->getScore());
   healthText->updateBody(ship->getHealth());
}

void GameState::checkCollisions() {
   ship->checkAsteroidCollisions(asteroids);
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
   for (int i = 0; i < 5; ++i) {
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

bool GameState::isGameRunning() {
   return gameIsRunning;
}

void GameState::reset() {
   delete ship;
   delete camera;

   ship = new AsteroidShip();
   camera = new Camera(ship);
   curFPS = 0;

   custodian.add(ship);
   initAsteroids();
}

void GameState::keyUp(int key) {
   switch(key) {
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

void GameState::keyDown(int key) {
   switch(key) {
   case SDLK_ESCAPE:
      exit(0);
      break;

   case SDLK_r:
      reset();
      break;
      
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

   case SDLK_b:
      ship->setBrake(true);
      break;
   }
}
