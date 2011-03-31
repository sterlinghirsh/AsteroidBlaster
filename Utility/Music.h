

#ifndef __MUSIC_H__
#define __MUSIC_H__

#include <list>
#include <iostream>
#include <map>
#include "SDL_mixer.h"

class Music {
   public:
      static void FreeAll(); 
      static void Add(std::string file, std::string keyName);
      static void playMusic(std::string file);
      static void pauseMusic();
      static void resumeMusic();
      static void stopMusic();
      static std::string currentlyPlaying();
   private:
      static std::map<std::string, Mix_Music*> musics;
      static std::string currPlay;
      static int volume;
};

#endif
