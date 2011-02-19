


#include "Utility/Music.h"
#include <iostream>
#include "Utility/GameState.h"

std::list<Mix_Music*> Music::musics;
int Music::currPlay = -1;
int Music::volume = SDL_MIX_MAXVOLUME;

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
   if (currPlay != -1) {
      Mix_HaltMusic();
   } else if (currPlay == idx) {
      std::cout << "that music is already playing!" << std::endl;
   }
   
   std::list<Mix_Music*>::iterator it = musics.begin();
   std::advance(it, idx);
   
	if(Mix_PlayMusic(*it, -1)==-1) {
	   std::cerr << "could not play music # " << idx << ", exiting!" << std::endl;
		exit(0);
   }
   
   currPlay = idx;
	Mix_VolumeMusic(volume);
}

void Music::pauseMusic() {
   Mix_PauseMusic();
}

void Music::resumeMusic() {
   Mix_ResumeMusic();
}

void Music::stopMusic() {
   Mix_HaltMusic();
   currPlay != -1;
}

int Music::currentlyPlaying() {
   return currPlay;
}





