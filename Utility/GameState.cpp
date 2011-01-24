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
   ship = new AsteroidShip(GL_LIGHT0, worldSize);
   camera = new Camera(ship);
   cube = new BoundingSpace(worldSize / 2, 0, 0, 0);
   // Set up our text objects to be displayed on screen.
   curFPS = 0;
   FPSText = new BitmapTextDisplay("FPS: ", curFPS, "", 10, 20);
   numAsteroidsText = new BitmapTextDisplay("Asteroids Remaining: ", (int)asteroids.size(), "", 10, 40);
   scoreText = new BitmapTextDisplay("Score: ", ship->getScore(), "", 10, 60);
   healthText = new BitmapTextDisplay("Health: ", ship->getHealth(), "", 10, 80);
   gameOverText = new BitmapTextDisplay("GAME OVER", GW/2, GH/2);
   winText = new BitmapTextDisplay("YOU WIN!", GW/2, GH/2);
   initAsteroids();
}

GameState::~GameState() {
   delete skybox;
   delete ship;
   delete camera;
   delete cube;
}

void GameState::update(double timeDiff) {
   if(ship->getHealth() <= 0){
      gameIsRunning = false;
   }
   else if (ship->score >= 4000){
      gameIsRunning = false;
   }
   
   cube->constrain(ship);
   for (asteroid = asteroids.begin(); asteroid != asteroids.end(); ++asteroid) {
      (*asteroid)->updatePosition(timeDiff);
      cube->constrain(*asteroid);
   }
   // Update the values of all of the text objects.
   updateText();
}

void GameState::draw() {
   // Draw all of the text objects to the screen.
   drawAllText();
   camera->setCamera(true);
   skybox->draw(camera);
   drawAsteroids();
   ship->draw();
}

void GameState::drawAsteroids() {
   materials(Rock);
   asteroid = asteroids.begin();
   for (asteroid = asteroids.begin(); asteroid != asteroids.end(); asteroid++) {
      (*asteroid)->draw(true);
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

   /* Set the camera using the location of your eye, 
    * the location where you're looking at, and the up vector.
    * The camera is set to be just 0.25 behind where you're looking at.
    */
   gluLookAt(0, 0, 0.25, 0, 0, 0, 0, 1, 0);

   /* Use orthonormal view so the text stays perpendicular 
    * to the camera at all times.
    */
   useOrtho();

   /* We need to disable the lighting temporarily 
    * in order to set the color properly.
    */
   glDisable(GL_LIGHTING);
   
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
   for (int i = 0; i < 5; ++i) {
      asteroids.push_back(new Asteroid3D(10 + (10 * randdouble()), worldSize));
   }
}
