
#ifndef __SOUNDEFFECT_H__
#define __SOUNDEFFECT_H__

#include <list>
#include <map>
#include <iostream>
#include "SDL_mixer.h"

class SoundEffect {
   public:
      SoundEffect();
      ~SoundEffect();
      
      static int numChannels;
      
      static void Add(std::string file, std::string keyName);
      static int playSoundEffect(std::string file, bool loop = false);
      static void pauseSoundEffect(int handle);
      void resumeSoundEffect(int handle);
      static void stopSoundEffect(int handle);
      int currentlyPlaying(int handle);

   private:
      static std::map<std::string, Mix_Chunk*> soundEffects;
      static int currChannel;
      static int volume;
};

#endif
