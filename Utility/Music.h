#ifndef __MUSIC_H__
#define __MUSIC_H__

#include "Utility/Vector3D.h"
#include "Utility/Point3D.h"
#include "Utility/GlobalUtility.h"
#include <string>
#include <list>
#include "SDL_mixer.h"

class Music {
   public:
      Music();
      ~Music();
      static std::list<Mix_Music*> musics;
      
      static void Add(const char *file);
      static void playMusic(int idx);
      static void stopMusic(int idx);
      
      
      
   protected:
};

#endif
