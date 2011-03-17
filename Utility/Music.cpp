


#include "Utility/Music.h"
#include "Utility/GameState.h"
#include <iostream>
#include <map>


std::map<std::string, Mix_Music*> Music::musics;
std::string Music::currPlay = "\0";
int Music::volume = SDL_MIX_MAXVOLUME;

Music::Music() {

}

Music::~Music() {
   std::map<std::string, Mix_Music*>::iterator it = Music::musics.begin();
   for (; it != Music::musics.end(); ++it ) {
      Mix_FreeMusic(it->second);
   }
}

void Music::Add(std::string file, std::string keyName) {
   Mix_Music *temp;

   if(!(temp=Mix_LoadMUS(file.c_str()))) {
      std::cerr << "could not load the music file: " << file << ", exiting!" << std::endl;
      exit(0);
   }

   musics.insert( std::pair<std::string, Mix_Music*>(keyName,temp) );
}


void Music::playMusic(std::string filename) {
   if (currPlay == filename) {
      std::cout << filename << " is already playing!" << std::endl;
   } else if (currPlay != "\0") {
      Mix_HaltMusic();
   }


   std::map<std::string, Mix_Music*>::iterator iter = musics.find(filename);


   if (iter == musics.end()) {
      std::cerr << "failed to find: " << filename << " in Music::musics." << std::endl;
      return;
   }

   if(Mix_PlayMusic(iter->second, -1)==-1) {
      std::cerr << "could not play music " << filename << ", exiting!" << std::endl;
      exit(0);
   }

   currPlay = filename;
   Mix_VolumeMusic(volume);
   
   if (!musicOn) {
      Mix_PauseMusic();
   }
}

void Music::pauseMusic() {
   if (musicOn) {
      Mix_PauseMusic();
   }
}

void Music::resumeMusic() {
   if (!musicOn) {
      Mix_ResumeMusic();
   }
}

void Music::stopMusic() {
   Mix_HaltMusic();
   currPlay != "\0";
}

std::string Music::currentlyPlaying() {
   return currPlay;
}





