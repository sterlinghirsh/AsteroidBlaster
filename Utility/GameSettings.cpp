/**
 * GameSettings
 * Stores global settings for the game.
 * @author Sterling Hirsh
 * @date 4-12-11
 * CPE 476
 */

#include "Utility/GameSettings.h"
#include "Utility/GlobalUtility.h"

#define CONFIG_FILE "config.cfg"

#include <string>
#include <stdio.h>

GameSettings::GameSettings() {
   // Default Settings.
   bloom = false;
   fullscreen = false;
   soundOn = true;
   musicOn = true;
   mouseCapture = false;
   useOverlay = false;
   goodBuffers = false;
   
   drawDeferred = false;

   windowedGW = 800;
   windowedGH = 600;

   fullscreenGW = 1280;
   fullscreenGH = 1024;

   musicVolume = 0.2f;
   soundEffectVolume = 1.0f;

   drawStereo = true;

   readIn();

   GW = fullscreen ? fullscreenGW : windowedGW;
   GH = fullscreen ? fullscreenGH : windowedGH;
}

void GameSettings::toggleFullScreen() {
   fullscreen = !fullscreen;
   GW = fullscreen ? fullscreenGW : windowedGW;
   GH = fullscreen ? fullscreenGH : windowedGH;
}

/**
 * Write game settings to a file.
 */
void GameSettings::writeOut() {
   FILE* configFile = fopen(CONFIG_FILE, "w+");
   if (configFile == NULL) {
      perror("write config");
      return;
   } else {
      printf("Writing config...\n");
   }
   fprintf(configFile, "bloom %d\n", (int) bloom);
   fprintf(configFile, "drawDeferred %d\n", (int) drawDeferred);
   fprintf(configFile, "useOverlay %d\n", (int) useOverlay);
   fprintf(configFile, "fullscreen %d\n", (int) fullscreen);
   fprintf(configFile, "musicOn %d\n", (int) musicOn);
   fprintf(configFile, "soundOn %d\n", (int) soundOn);
   
   fprintf(configFile, "musicVolume %f\n", musicVolume);
   fprintf(configFile, "soundEffectVolume %f\n", soundEffectVolume);

   fprintf(configFile, "windowedGW %d\n", windowedGW);
   fprintf(configFile, "windowedGH %d\n", windowedGH);
   fprintf(configFile, "fullscreenGW %d\n", fullscreenGW);
   fprintf(configFile, "fullscreenGH %d\n", fullscreenGH);

   fclose(configFile);
}

/**
 * Read game settings from a file.
 */
void GameSettings::readIn() {
   int readInt;
   float readFloat;
   FILE* configFile = fopen(CONFIG_FILE, "r");

   if (configFile == NULL)
      return;

   if (fscanf(configFile, "bloom %d ", &readInt) > 0) {
      bloom = (0 != readInt);
   }
   
   if (fscanf(configFile, "drawDeferred %d ", &readInt) > 0) {
      drawDeferred = (0 != readInt);
   }

   if (fscanf(configFile, "useOverlay %d ", &readInt) > 0) {
      useOverlay = (0 != readInt);
   }

   if (fscanf(configFile, "fullscreen %d ", &readInt) > 0) {
      fullscreen = (0 != readInt);
   }

   if (fscanf(configFile, "musicOn %d ", &readInt) > 0) {
      musicOn = (0 != readInt);
   }

   if (fscanf(configFile, "soundOn %d ", &readInt) > 0) {
      soundOn = (0 != readInt);
   }
   
   if (fscanf(configFile, "musicVolume %f ", &readFloat) > 0) {
      musicVolume = clamp(readFloat, 0.0, 1.0);
   }
   
   if (fscanf(configFile, "soundEffectVolume %f ", &readFloat) > 0) {
      soundEffectVolume = clamp(readFloat, 0.0, 1.0);
   }

   if (fscanf(configFile, "windowedGW %d ", &readInt) > 0) {
      windowedGW = readInt;
   }
   
   if (fscanf(configFile, "windowedGH %d ", &readInt) > 0) {
      windowedGH = readInt;
   }
   
   if (fscanf(configFile, "fullscreenGW %d ", &readInt) > 0) {
      printf("read fullscreenGW %d\n", readInt);
      fullscreenGW = readInt;
   }
   
   if (fscanf(configFile, "fullscreenGH %d ", &readInt) > 0) {
      fullscreenGH = readInt;
   }

   printf("fullscreenGW: %d, windowedGW: %d\n", fullscreenGW, windowedGW);

   fclose(configFile);
}
