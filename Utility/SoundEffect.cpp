

#include "Utility/SoundEffect.h"
#include "Utility/GlobalUtility.h"
#include <iostream>
#include <map>

std::map<std::string, Mix_Chunk*> SoundEffect::soundEffects;
int SoundEffect::volume = MIX_MAX_VOLUME/3;
int SoundEffect::numChannels = CHANNEL_MAX;
int SoundEffect::currChannel = 0;


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
	   std::cerr << "Failed to play " << file << "\r";
		//exit(0);
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



