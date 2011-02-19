

#include "Utility/SoundEffect.h"
#include <iostream>

#define CHANNEL_MAX 16

std::list<Mix_Chunk*> SoundEffect::soundEffects;
int SoundEffect::volume = MIX_MAX_VOLUME/7;
int SoundEffect::numChannels = 8;
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


void SoundEffect::playSoundEffect(int idx) {
   std::list<Mix_Chunk*>::iterator it = soundEffects.begin();
   std::advance(it, idx);
   
	if(Mix_PlayChannel(-1, *it, 0)==-1) {
	   std::cerr << "could not play SoundEffect # " << idx << ", exiting!" << std::endl;
		//exit(0);
   }
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



