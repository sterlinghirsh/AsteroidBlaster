/**
 * GameSettings
 * Stores global settings for the game.
 * @author Sterling Hirsh
 * @date 4-12-11
 * CPE 476
 */

#include "Utility/GameSettings.h"

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
