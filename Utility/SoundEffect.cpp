

#include "Utility/SoundEffect.h"
#include "Utility/GlobalUtility.h"
#include "Items/Object3D.h"
#include "Graphics/Camera.h"
#include <iostream>
#include <map>
#include "Utility/Vector3D.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"


FMOD::System *fmodSystem;
FMOD::Sound *sound;
FMOD_RESULT result;
static SoundChannel* curMusic;
static std::string currPlay;

// Crazy high channel max for fmod. <3
#define CHANNEL_MAX 1000

std::map<std::string, FMOD::Sound*> soundEffects;

struct SoundChannel : FMOD::Channel {};

void Vector3DToFMOD_VECTOR(Vector3D* in, FMOD_VECTOR* out) {
   if (in != NULL) {
      out->x = in->x;
      out->y = in->y;
      out->z = in->z;
   } else {
      out->x = out->y = out->z = 0;
   }
}

/**
 * From fmod examples/playsound/main.cpp
 */
void ERRCHECK(FMOD_RESULT result) {
   if (result != FMOD_OK) {
      printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
      exit(-1);
   }
}

void SoundEffect::Init() {
   unsigned int      version;
   
   result = FMOD::System_Create(&fmodSystem);
   ERRCHECK(result);

   result = fmodSystem->getVersion(&version);
   ERRCHECK(result);

   if (version < FMOD_VERSION) {
      printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
      exit( EXIT_FAILURE);
   }

   // dopplerscale, distancefactor, rollofscale
   result = fmodSystem->set3DSettings(1.0f, 2.0f, 0.5f);
   ERRCHECK(result);

   result = fmodSystem->init(CHANNEL_MAX, FMOD_INIT_3D_RIGHTHANDED, NULL);
   ERRCHECK(result);
}

void SoundEffect::FreeAll() {
   std::map<std::string, FMOD::Sound*>::iterator it = soundEffects.begin();
   for (; it != soundEffects.end(); ++it ) {
      it->second->release();
   }
   result = fmodSystem->release();
   ERRCHECK(result);
}

void SoundEffect::Add(std::string file, std::string keyName, bool isMusic) {
   if (isMusic) {
      result = fmodSystem->createSound(file.c_str(), FMOD_2D | FMOD_CREATESTREAM, 0, &sound);
   } else {
      result = fmodSystem->createSound(file.c_str(), FMOD_3D, 0, &sound);
   }
   ERRCHECK(result);
   
   soundEffects.insert( std::pair<std::string, FMOD::Sound*>(keyName, sound) );
}

/**
 * loop defaults to false.
 */
SoundChannel* SoundEffect::playSoundEffect(std::string file, Vector3D* source, 
 Vector3D* velocity, bool internal, float volume, bool loop, bool isMusic) {
   FMOD::Channel* channel = NULL;
   SoundChannel* soundChannel = NULL;
   if (gameSettings->soundOn) {
      std::map<std::string, FMOD::Sound*>::iterator iter = soundEffects.find(file);

      if (iter == soundEffects.end()) {
         std::cerr << "failed to find: " << file << " in soundEffects." << std::endl;
         return NULL;
      }

      result = fmodSystem->playSound(FMOD_CHANNEL_FREE, iter->second, false, &channel);
      ERRCHECK(result);
      
      soundChannel = static_cast<SoundChannel*>(channel);

      if (loop) {
         soundChannel->setMode(FMOD_LOOP_NORMAL);
         soundChannel->setLoopCount(-1);
         ERRCHECK(result);
      }

      if (internal) {
         soundChannel->setMode(FMOD_2D);
         // Probably don't have to set it back to worldrelative, right?
      } else {
         updateSource(soundChannel, source, velocity);
      }

      // Always set music to musicVolume, but multiply soundEffects by soundEffectVolume.
      // Note: when music is playing, volume should be set to 1.0f.
      result = soundChannel->setVolume(isMusic ? 
       volume * gameSettings->musicVolume : 
       volume * gameSettings->soundEffectVolume);
      ERRCHECK(result);
   }
   
   return soundChannel;
}

void SoundEffect::stopSoundEffect(SoundChannel* channel) {
   channel->stop();
}

void SoundEffect::stopAllSoundEffect(){
   FMOD::ChannelGroup* group;
   result = fmodSystem->getMasterChannelGroup(&group);
   ERRCHECK(result);
   group->stop();
}

void SoundEffect::updatePositions(Vector3D* position,
 Vector3D* velocity, Vector3D* forward, Vector3D* up) {
   FMOD_VECTOR pos, vel, fwd, newUp;
   Vector3DToFMOD_VECTOR(position, &pos);
   Vector3DToFMOD_VECTOR(velocity, &vel);
   Vector3DToFMOD_VECTOR(up, &newUp);
   Vector3DToFMOD_VECTOR(forward, &fwd);
   
   fmodSystem->set3DListenerAttributes(0, &pos, &vel, &fwd, &newUp);
   fmodSystem->update();
}

void SoundEffect::updateSource(SoundChannel* channel, Vector3D* position, Vector3D* velocity) {
   FMOD_VECTOR pos, vel;
   Vector3DToFMOD_VECTOR(position, &pos);
   Vector3DToFMOD_VECTOR(velocity, &vel);
   channel->set3DAttributes(&pos, &vel);
}

// Music functions.
void SoundEffect::playMusic(std::string keyName) {
   if (!gameSettings->musicOn) {
      return;
   }

   if (currPlay == keyName) {
      return;
   }
   
   if (currPlay != "\0" && curMusic != NULL) {
      //Mix_HaltMusic();
      curMusic->stop();
   }

   // Volume 1.0f gets multiplied by gameSettings->musicVolume.
   curMusic = playSoundEffect(keyName, NULL, NULL, true, 1.0f, true, true);
   currPlay = keyName;
}

void SoundEffect::pauseMusic() {
   if (curMusic != NULL)
      curMusic->setPaused(true);
}

void SoundEffect::resumeMusic() {
   if (curMusic != NULL && gameSettings->musicOn)
      curMusic->setPaused(false);
}

void SoundEffect::stopMusic() {
   if (curMusic != NULL) {
      curMusic->stop();
      curMusic = NULL;
   }
   currPlay != "\0";
}

std::string SoundEffect::currentlyPlaying() {
   return currPlay;
}

bool SoundEffect::musicPlaying() {
   bool paused;
   if (curMusic == NULL)
      return false;

   result = curMusic->getPaused(&paused);
   ERRCHECK(result);
   return !paused;
}
