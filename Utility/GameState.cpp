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

extern double minimapSizeFactor;

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
  shardText = new BitmapTextDisplay("Shards: ", ship->getShards(), "", 10, 80);
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

  isW = isA = isS = isD = false;
  /* A view frustum culled list of objects to be used for drawing and by
     the shooting AI.
   */
  std::list<Object3D*>* viewFrustumObjects;


  // Bloom
  xSize = ySize = 256;
  //xSize = ySize = 8;
  colorBits = new char[ xSize * ySize * 3 ];
  //texture creation..
  glGenTextures(1,&texture);
  glBindTexture(GL_TEXTURE_2D,texture);

  glTexImage2D(GL_TEXTURE_2D,0 ,3 , xSize,
      ySize, 0 , GL_RGB,
      GL_UNSIGNED_BYTE, colorBits);

  //you can set other texture parameters if you want
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //clean up
  delete[] colorBits;

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
  // If the player lost, draw the game over text
  if (!gameIsRunning && ship->getHealth() <= 0) {
    SoundEffect::playSoundEffect(7);
  }
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
  const float ringWidth = 1;
  double radius2D; // Radius when an object is projected onto the forward-right plane of the ship.
  double radius3D; // Radius from the ship in 3D space.

  positionVector.updateMagnitude(*ship->position);

  glDisable(GL_LIGHT0); // turn off the normal light, put a light at the center.
  glEnable(GL_LIGHT1);

  glPushMatrix();
  ship->glRotate(false);
  Vector3D oppositeOfPosition(*(ship->position));

  // Translate everything so that the ship is at 0, 0 and everything is centered there.
  oppositeOfPosition.updateMagnitude(oppositeOfPosition);
  glScalef(scaleFactor, scaleFactor, scaleFactor);
  //glRotatef(180, 0, 1, 0);
  oppositeOfPosition.glTranslate(-1);
  glDisable(GL_COLOR_MATERIAL);

  // For each item that needs to be drawn in the minimap
  for (listIter = objects->begin(); listIter != objects->end(); ++listIter) {
    // Make sure it's not null, & then draw it in the minimap
    // TODO: Make this not copy code from below.
    if (*listIter != NULL && ((*listIter)->shouldDrawInMinimap || *listIter == ship)) {
      radius3D = ship->position->distanceFrom(*(*listIter)->position);
      if (radius3D < 20) {
        glColor3f(1, 0, 0);
        setMaterial(RedFlat);
      } else if (radius3D < 30) {
        glColor3f(1, 1, 0);
        setMaterial(YellowFlat);
      } else {
        glColor3f(0, 1, 0);
        setMaterial(GreenTransparent);
      }
      (*listIter)->drawInMinimap();
    }
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
  setMaterial(WhiteSolid);
  for (listIter = objects->begin(); listIter != objects->end(); ++listIter) {
    // Make sure it's not null, & then draw it in the minimap
    if (*listIter != NULL && (*listIter)->shouldDrawInMinimap && *listIter != ship) {
      objectPosition = *(*listIter)->position; // Get the position.
      positionVector.movePoint(objectPosition, -1); // Center on the ship
      objectPosition = modelViewMatrix * (objectPosition); // Rotate about the ship
      radius2D = distance2D(objectPosition.x, objectPosition.z);
      radius3D = distance3D(objectPosition.x, objectPosition.y, objectPosition.z);
      if (radius3D < 20) {
        glColor3f(1, 0, 0);
        setMaterial(RedTransparent);
      } else if (radius3D < 30) {
        glColor3f(1, 1, 0);
        setMaterial(YellowTransparent);
      } else {
        glColor3f(0, 1, 0);
        setMaterial(GreenTransparent);
      }

      // Draw a disc.
      // This is the way that one game did it with the circles on a plane.
      glPushMatrix();
      glEnable(GL_LIGHTING);
      glRotatef(-90.0,1.0f,0.0f,0.0f);   // Rotate By 0 On The X-Axis
      gluDisk(quadric, radius2D - (ringWidth / 2), radius2D + (ringWidth / 2) ,16,2);
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

  // Draw the sphere around it.
  glEnable(GL_LIGHTING);
  glDisable(GL_LIGHT1); // Reenable the normal light.
  glEnable(GL_LIGHT0);
  setMaterial(GrayTransparent);
  //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  gluSphere(quadric, 2, 20, 20);
  glEnable(GL_CULL_FACE);
}

/**
 * Draw the main view.
 */
void GameState::draw() {

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
  //std::list<Object3D*>* objects = ship->getRadar()->getViewFrustumReading();
  viewFrustumObjects = ship->getRadar()->getViewFrustumReading();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  if (thirdPerson) {
    ship->drawShotDirectionIndicators();
  } else {
    ship->drawCrosshair();
  }

  for (listIter = viewFrustumObjects->begin(); listIter != viewFrustumObjects->end(); ++listIter) {
    if (*listIter == NULL || *listIter == ship)
      continue;
    (*listIter)->draw();
  }

  // Don't draw the ship in first Person mode.
  if (thirdPerson) {
    ship->draw();
  }
}

/**
 * Draw the glowing objects.
 */
void GameState::drawGlow() {

  if (thirdPerson) {
    Vector3D newOffset(ship->forward->scalarMultiply(-3));
    newOffset.addUpdate(ship->up->scalarMultiply(0.5));
    camera->setOffset(newOffset);
  } else {
    camera->setOffset(0, 0, 0);
  }

  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  camera->setCamera(true);
  //skybox->draw(camera);
  cube->draw();

  // Get a list of all of the objects after culling them down to the view frustum.
  //std::list<Object3D*>* objects = ship->getRadar()->getViewFrustumReading();
  viewFrustumObjects = ship->getRadar()->getViewFrustumReading();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  if (thirdPerson) {
    ship->drawShotDirectionIndicators();
  } else {
    ship->drawCrosshair();
  }

  for (listIter = viewFrustumObjects->begin(); listIter != viewFrustumObjects->end(); ++listIter) {
    if (*listIter == NULL || *listIter == ship)
      continue;
    (*listIter)->drawGlow();
  }

  // Don't draw the ship in first Person mode.
  if (thirdPerson) {
    ship->draw();
  }
}

void GameState::hBlur() {
  useOrtho();
  glPushMatrix();
  glBlendFunc(GL_ONE, GL_ONE);
  glDisable(GL_LIGHTING);
  float minY = -1.0;
  float maxY = 1.0;
  float minX = -1.0 * aspect;
  float maxX = 1.0 * aspect;
  int tex = texture;
  GLint texLoc = glGetUniformLocation(hBlurShader, "RTscene");
  glUseProgram(hBlurShader);
  glUniform1i(texLoc, tex);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  //glColor4f(1.0, 1.0, 1.0, 0.5);
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
  //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(0);

  glBindTexture(GL_TEXTURE_2D, tex);
  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0,0, (maxX - minX), (maxY - minY), 0);
}

void GameState::vBlur() {
  useOrtho();
  glPushMatrix();
  glBlendFunc(GL_ONE, GL_ONE);
  glDisable(GL_LIGHTING);
  float minY = -1.0;
  float maxY = 1.0;
  float minX = -1.0 * aspect;
  float maxX = 1.0 * aspect;
  int tex = texture;
  GLint texLoc = glGetUniformLocation(vBlurShader, "RTBlurH");
  glUseProgram(vBlurShader);
  glUniform1i(texLoc, tex);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  //glColor4f(1.0, 1.0, 1.0, 0.5);
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
  //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(0);

}

void GameState::drawBloom() {
  hBlur();
  vBlur();
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
  ship->drawWeaponReadyBar();
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
  glPushMatrix();
  /* Don't draw stuff in front of the text. */
  //glDisable(GL_DEPTH_TEST);

  // If the player lost, draw the game over text
  if (!gameIsRunning && ship->getHealth() <= 0) {
    gameOverText->draw();
  } else if (!gameIsRunning && ship->getHealth() > 0) {
    // If the player won, draw the win text
    winText->draw();
  }

  // Draw all of the BitmapTextDisplay objects.
  FPSText->draw();
  numAsteroidsText->draw();
  scoreText->draw();
  shardText->draw();
  healthText->draw();
  weaponText->draw();

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

/**
 * Reset everything in the game to play again
 */
void GameState::reset() {
  //delete ship;
  delete camera;
  custodian.clear();

  //ship = new AsteroidShip();
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
    if(ship->flyingAI->isEnabled()) {
      ship->flyingAI->disable();
    } else {
      ship->accelerateRight(0);
      ship->accelerateUp(0);
      ship->setYawSpeed(0.0);
      ship->setPitchSpeed(0.0);
      ship->setRollSpeed(0.0);
      ship->flyingAI->enable();
    }
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
  case SDLK_KP_ENTER:
    if (Mix_PausedMusic()) {
      Music::resumeMusic();
    } else {
      Music::pauseMusic();
    }
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



