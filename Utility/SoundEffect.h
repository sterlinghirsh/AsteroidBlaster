
#ifndef __SOUNDEFFECT_H__
#define __SOUNDEFFECT_H__

#include <list>
#include <map>
#include <iostream>
#include "SDL_mixer.h"

#define CHANNEL_MAX 32
#define DEFAULT_VOLUME 48

class Object3D;
class Vector3D;

class SoundEffect {
   public:
      static void FreeAll();
      static int numChannels;
      
      static void Init();
      static void Add(std::string file, std::string keyName);
      static int playSoundEffect(std::string file, Vector3D* source = NULL, bool internal = false, int volume = DEFAULT_VOLUME, bool loop = false);
      static void pauseSoundEffect(int handle);
      static void resumeSoundEffect(int handle);
      static void stopSoundEffect(int handle);
      static int currentlyPlaying(int handle);
      static void stopAllSoundEffect();
      static void resetPosition(int handle);
      static void updatePositions(Object3D* receiver);
      void updateSource(int handle, Vector3D* source, int volume = DEFAULT_VOLUME);

   private:
      static std::map<std::string, Mix_Chunk*> soundEffects;
      static int currChannel;
      static void setPosition(int handle, Vector3D* source, Object3D* receiver);
};

#endif
