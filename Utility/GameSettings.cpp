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

   fclose(configFile);
}

/**
 * Read game settings from a file.
 */
void GameSettings::readIn() {
   FILE* configFile = fopen(CONFIG_FILE, "r");
   if (configFile == NULL)
      return;

   int readInt;
   fscanf(configFile, "bloom %d", &readInt);
   bloom = (bool) readInt;
}
