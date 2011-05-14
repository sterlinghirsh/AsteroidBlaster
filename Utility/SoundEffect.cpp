

#include "Utility/SoundEffect.h"
#include "Utility/GlobalUtility.h"
#include "Items/Object3D.h"
#include <iostream>
#include <map>
#include "Utility/Vector3D.h"

// Hold a position and a sound handle.
struct SoundEffectHolder {
   bool playing;
   bool internal;
   int volume;
   Vector3D position;
};

SoundEffectHolder channels[CHANNEL_MAX];

std::map<std::string, Mix_Chunk*> SoundEffect::soundEffects;
int SoundEffect::numChannels = CHANNEL_MAX;
int SoundEffect::currChannel = 0;

void SoundEffect::Init() {
   for (int i = 0; i < CHANNEL_MAX; ++i) {
      channels[i].internal = true;
      channels[i].volume = DEFAULT_VOLUME;
      channels[i].position.x = 0;
      channels[i].position.y = 0;
      channels[i].position.z = 0;
      channels[i].playing = false;
   }
}

void SoundEffect::FreeAll() {
   std::map<std::string, Mix_Chunk*>::iterator it = SoundEffect::soundEffects.begin();
   for (; it != SoundEffect::soundEffects.end(); ++it ) {
      Mix_FreeChunk(it->second);
   }
}

void SoundEffect::Add(std::string file, std::string keyName) {
   Mix_Chunk *temp;
   
	if(!(temp=Mix_LoadWAV(file.c_str()))) {
	   std::cerr << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
		exit(1);
   }
   
   soundEffects.insert( std::pair<std::string, Mix_Chunk*>(keyName,temp) );
}


/**
 * loop defaults to false.
 */
int SoundEffect::playSoundEffect(std::string file, Vector3D* source, bool internal, int volume, bool loop) {
   std::map<std::string, Mix_Chunk*>::iterator iter = soundEffects.find(file);

   if (iter == soundEffects.end()) {
      std::cerr << "failed to find: " << file << " in SoundEffect::soundEffects." << std::endl;
      return -1;
   }
   
   int handle = Mix_PlayChannel(-1, iter->second, loop ? -1 : 0);
   
	if(handle == -1) {
	   //std::cerr << "Failed to play " << file << "\r";
		//exit(0);
   }
   if (handle >= 0 && handle < CHANNEL_MAX) {
      channels[handle].volume = volume;
      if (source == NULL) {
         channels[handle].internal = true;
      } else {
         channels[handle].position = *source;
         // Set internal to true if internal is true or source is null.
         channels[handle].internal = internal;
      }
      channels[handle].playing = true;
   }

   Mix_Volume(handle, volume);

   if (!gameSettings->soundOn) {
      Mix_Pause(handle);
   }
   
   return handle;
}

void SoundEffect::pauseSoundEffect(int handle) {
   if (gameSettings->soundOn) {
      Mix_Pause(handle);
   }
}

void SoundEffect::resumeSoundEffect(int handle) {
   if (!gameSettings->soundOn) {
      Mix_Resume(handle);
   }
}

void SoundEffect::stopSoundEffect(int handle) {
   Mix_HaltChannel(handle);
}

int SoundEffect::currentlyPlaying(int handle) {
   return Mix_Playing(handle);
}

void SoundEffect::stopAllSoundEffect(){
   Mix_HaltChannel(-1);
}

void SoundEffect::resetPosition(int handle) {
   Mix_SetPosition(handle, 0, 0);
}

void SoundEffect::updatePositions(Object3D* receiver) {
   for (int i = 0; i < CHANNEL_MAX; ++i) {
      if (channels[i].playing) {
         if (channels[i].internal) {
            resetPosition(i);
         } else {
            setPosition(i, &(channels[i].position), receiver);
         }
         Mix_Volume(i, channels[i].volume);
      }
   }
}

void SoundEffect::updateSource(int handle, Vector3D* source, int volume) {
   channels[handle].position.clone(source);
   channels[handle].volume = volume;
}

// This is how we set the position of sounds.
void SoundEffect::setPosition(int handle, Vector3D* position, Object3D* receiver) {
   Sint16 angle;
   Uint8 distance;

   Vector3D receiverToSource(receiver->position, position);
   const double maxSoundDistance = 80;
   distance = (Uint8) (255 * clamp(receiverToSource.getLength() / maxSoundDistance, 0, 1)); 
   
   double upDownAmount = receiver->up->dot(receiverToSource);
   receiverToSource.subtractUpdate(receiver->up->scalarMultiply(upDownAmount));

   receiverToSource.normalize();
   double leftRightAmount = receiver->right->dot(receiverToSource);
   double forwardBackwardAmount = receiver->forward->dot(receiverToSource);

   // Calculate how much the forward contribution should be.
   // This gives a number from 0 (directly ahead) to 180 (directly behind).
   angle = (Sint16) ((1 - forwardBackwardAmount) * 90);

   // If it's on the left, make it from 360 (directly ahead) to 180 (directly behind).
   if (leftRightAmount < 0) {
      angle = 360 - angle;
   }

   Mix_SetPosition(handle, angle, distance);
}


