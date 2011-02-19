
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
      static void playSoundEffect(int idx);
      void pauseSoundEffect(int idx);
      void resumeSoundEffect(int idx);
      void stopSoundEffect(int idx);
      int currentlyPlaying(int idx);

   private:
      static std::list<Mix_Chunk*> soundEffects;
      static int currChannel;
      static int volume;
};

#endif


