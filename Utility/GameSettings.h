/**
 * GameSettings
 * Stores global settings for the game.
 * @author Sterling Hirsh
 * @date 4-12-11
 * CPE 476
 */

#ifndef __GAMESETTINGS_H__
#define __GAMESETTINGS_H__

#include <string>

class GameSettings {
   public:
      GameSettings();

      bool bloom;
      bool fullscreen;
      bool soundOn;
      bool musicOn;

      // Window width and height.
      int GW, GH;
      int windowedGW, windowedGH;
      int fullscreenGW, fullscreenGH;
      bool mouseCapture;
      bool useOverlay;
      bool goodBuffers;
      
      bool drawDeferred;

      float musicVolume;
      float soundEffectVolume;

      bool drawStereo;

      bool enableMinimap;

      std::string name;

      void toggleFullScreen();

      void writeOut();
      void readIn();
};

#endif
