
#ifndef __SOUNDEFFECT_H__
#define __SOUNDEFFECT_H__

#include <list>
#include "SDL_mixer.h"

class SoundEffect {
   public:
      SoundEffect();
      ~SoundEffect();
      
      static int numChannels;
      
      static void Add(const char *file);
      static int playSoundEffect(int idx, bool loop = false);
      static void pauseSoundEffect(int idx);
      void resumeSoundEffect(int idx);
      static void stopSoundEffect(int idx);
      int currentlyPlaying(int idx);

   private:
      static std::list<Mix_Chunk*> soundEffects;
      static int currChannel;
      static int volume;
};

#endif
