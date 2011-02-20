

#include "Utility/SoundEffect.h"
#include <iostream>

#define CHANNEL_MAX 16

std::list<Mix_Chunk*> SoundEffect::soundEffects;
int SoundEffect::volume = MIX_MAX_VOLUME/7;
int SoundEffect::numChannels = 16;
int SoundEffect::currChannel = 0;


SoundEffect::SoundEffect() {

}

SoundEffect::~SoundEffect() {
   std::list<Mix_Chunk*>::iterator it = soundEffects.begin();
   for (; it != SoundEffect::soundEffects.end(); ++it ) {
      Mix_FreeChunk(*it);
   }
}

void SoundEffect::Add(const char *file) {
   Mix_Chunk *temp;
   
   temp=Mix_LoadWAV(file);
   
	if(!temp) {
	   std::cerr << "Mix_LoadWAV: " << Mix_GetError() << std::endl;
		exit(0);
   }
   
   Mix_Volume(-1,volume);
	soundEffects.push_back(temp);
}


/**
 * loop defaults to false.
 */
int SoundEffect::playSoundEffect(int idx, bool loop) {
   std::list<Mix_Chunk*>::iterator it = soundEffects.begin();
   std::advance(it, idx);

   int handle = Mix_PlayChannel(-1, *it, loop ? -1 : 0);
   
	if (handle == -1) {
	   std::cerr << "could not play SoundEffect # " << idx << ", exciting!" << std::endl;
   }
   return handle;
}

void SoundEffect::pauseSoundEffect(int idx) {
   Mix_Pause(idx);
}

void SoundEffect::resumeSoundEffect(int idx) {
   Mix_Resume(idx);
}

void SoundEffect::stopSoundEffect(int idx) {
   Mix_HaltChannel(idx);
}

int SoundEffect::currentlyPlaying(int idx) {
   return Mix_Playing(idx);
}



