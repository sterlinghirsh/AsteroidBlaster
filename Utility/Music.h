

#ifndef __MUSIC_H__
#define __MUSIC_H__

#include <list>
#include "SDL_mixer.h"

class Music {
   public:
      Music();
      ~Music();
      
      
      static void Add(const char *file);
      static void playMusic(int idx);
      static void pauseMusic();
      static void resumeMusic();
      static void stopMusic();
      static int currentlyPlaying();
   private:
      static std::list<Mix_Music*> musics;
      static int currPlay;
      static int volume;
};

#endif
