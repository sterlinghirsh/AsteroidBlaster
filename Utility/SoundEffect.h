#ifndef __SOUNDEFFECT_H__
#define __SOUNDEFFECT_H__

#include <list>
#include <map>
#include <iostream>

#define DEFAULT_VOLUME 48

class Camera;
class Vector3D;
struct SoundChannel;

class SoundEffect {
   public:
      static void FreeAll();
      static void Init();
      static void Add(std::string file, std::string keyName, bool isMusic = false);
      static SoundChannel* playSoundEffect(std::string file, Vector3D* source = NULL, Vector3D* velocity = NULL, bool internal = false, int volume = DEFAULT_VOLUME, bool loop = false);
      static void stopSoundEffect(SoundChannel* channel);
      static void stopAllSoundEffect();
      static void updatePositions(Camera* receiver);
      static void updatePositions(Vector3D* position, Vector3D* velocity, Vector3D* forward, Vector3D* up);
      static void updateSource(SoundChannel* channel, Vector3D* position, Vector3D* velocity);
      static void playMusic(std::string file);
      static void pauseMusic();
      static void resumeMusic();
      static void stopMusic();
      static std::string currentlyPlaying();
      static bool musicPlaying();
};

#endif
