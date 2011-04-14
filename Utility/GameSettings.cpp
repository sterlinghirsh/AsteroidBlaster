/**
 * GameSettings
 * Stores global settings for the game.
 * @author Sterling Hirsh
 * @date 4-12-11
 * CPE 476
 */

#include "Utility/GameSettings.h"

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

   windowedGW = 800;
   windowedGH = 600;

   fullscreenGW = 1280;
   fullscreenGH = 1024;

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
   fprintf(configFile, "fullscreen %d\n", (int) fullscreen);
   fprintf(configFile, "musicOn %d\n", (int) musicOn);
   fprintf(configFile, "soundOn %d\n", (int) soundOn);

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
   FILE* configFile = fopen(CONFIG_FILE, "r");

   if (configFile == NULL)
      return;

   if (fscanf(configFile, "bloom %d ", &readInt) > 0) {
      bloom = (bool) readInt;
   }

   if (fscanf(configFile, "fullscreen %d ", &readInt) > 0) {
      fullscreen = (bool) readInt;
   }

   if (fscanf(configFile, "musicOn %d ", &readInt) > 0) {
      musicOn = (bool) readInt;
   }

   if (fscanf(configFile, "soundOn %d ", &readInt) > 0) {
      soundOn = (bool) readInt;
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
