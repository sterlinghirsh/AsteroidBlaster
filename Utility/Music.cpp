


#include "Utility/Music.h"
#include <math.h>
#include <iostream>
#include "Utility/GameState.h"


#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif
#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif 


std::list<Mix_Music*> Music::musics;

Music::Music() {

}

Music::~Music() {
   std::list<Mix_Music*>::iterator it = Music::musics.begin();
   for (; it != Music::musics.end(); ++it ) {
      Mix_FreeMusic(*it);
   }
}

void Music::Add(const char *file) {
   Mix_Music *temp;
   
	if(!(temp=Mix_LoadMUS(file))) {
	   std::cerr << "could not load the music file: " << file << ", exiting!" << std::endl;
		exit(0);
   }
   

	musics.push_back(temp);
}


void Music::playMusic(int idx) {
   std::list<Mix_Music*>::iterator it = musics.begin();
   std::advance(it, idx);
   
	if(Mix_PlayMusic(*it, idx)==-1) {
	   std::cerr << "could not play music # " << idx << ", exiting!" << std::endl;
		exit(0);
   }
   
	Mix_VolumeMusic(SDL_MIX_MAXVOLUME);
}

void Music::stopMusic(int i) {
   Mix_HaltMusic();
}


