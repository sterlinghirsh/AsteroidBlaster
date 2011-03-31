
#ifndef __SOUNDEFFECT_H__
#define __SOUNDEFFECT_H__

#include <list>
#include <map>
#include <iostream>
#include "SDL_mixer.h"

#define CHANNEL_MAX 32

class SoundEffect {
   public:
      static void FreeAll();
      static int numChannels;
      
      static void Add(std::string file, std::string keyName);
      static int playSoundEffect(std::string file, bool loop = false);
      static void pauseSoundEffect(int handle);
      static void resumeSoundEffect(int handle);
      static void stopSoundEffect(int handle);
      static int currentlyPlaying(int handle);
      static void stopAllSoundEffect();

   private:
      static std::map<std::string, Mix_Chunk*> soundEffects;
      static int currChannel;
      static int volume;
};

#endif
