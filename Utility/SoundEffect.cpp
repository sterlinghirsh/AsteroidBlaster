

#include "Utility/SoundEffect.h"
#include <iostream>
#include <map>

#define CHANNEL_MAX 16

std::map<std::string, Mix_Chunk*> SoundEffect::soundEffects;
int SoundEffect::volume = MIX_MAX_VOLUME/7;
int SoundEffect::numChannels = 16;
int SoundEffect::currChannel = 0;


SoundEffect::SoundEffect() {

}

SoundEffect::~SoundEffect() {
   std::map<std::string, Mix_Chunk*>::iterator it = SoundEffect::soundEffects.begin();
   for (; it != SoundEffect::soundEffects.end(); ++it ) {
      Mix_FreeChunk(it->second);
   }
}

void SoundEffect::Add(std::string file, std::string keyName) {
   Mix_Chunk *temp;
   
	if(!(temp=Mix_LoadWAV(file.c_str()))) {
	   std::cerr << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
		exit(0);
   }
   
   soundEffects.insert( std::pair<std::string, Mix_Chunk*>(keyName,temp) );
}


/**
 * loop defaults to false.
 */
int SoundEffect::playSoundEffect(std::string file, bool loop) {
   std::map<std::string, Mix_Chunk*>::iterator iter = soundEffects.find(file);

   if (iter == soundEffects.end()) {
      std::cerr << "failed to find: " << file << " in SoundEffect::soundEffects." << std::endl;
      return -1;
   }
   
   int handle = Mix_PlayChannel(-1, iter->second, loop ? -1 : 0);
   
	if(handle == -1) {
	   std::cerr << "could not play SoundEffect " << file << ", something went wrong!" << std::endl;
		//exit(0);
   }
   
   return handle;
}

void SoundEffect::pauseSoundEffect(int handle) {
   Mix_Pause(handle);
}

void SoundEffect::resumeSoundEffect(int handle) {
   Mix_Resume(handle);
}

void SoundEffect::stopSoundEffect(int handle) {
   Mix_HaltChannel(handle);
}

int SoundEffect::currentlyPlaying(int handle) {
   return Mix_Playing(handle);
}



