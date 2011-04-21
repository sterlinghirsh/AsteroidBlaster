


#include "Utility/Music.h"
#include "Utility/GameState.h"
#include <iostream>
#include <map>


std::map<std::string, Mix_Music*> Music::musics;
std::string Music::currPlay = "\0";
int Music::volume = SDL_MIX_MAXVOLUME;

void Music::FreeAll() {
   std::map<std::string, Mix_Music*>::iterator it = Music::musics.begin();
   for (; it != Music::musics.end(); ++it ) {
      Mix_FreeMusic(it->second);
   }
}

void Music::Add(std::string file, std::string keyName) {
   Mix_Music *temp;

   if(!(temp=Mix_LoadMUS(file.c_str()))) {
      std::cerr << "could not load the music file: " << file << ", exiting!" << std::endl;
      exit(1);
   }

   musics.insert( std::pair<std::string, Mix_Music*>(keyName,temp) );
}


void Music::playMusic(std::string keyName) {
   if (currPlay == keyName) {
      std::cout << keyName << " is already playing!" << std::endl;
   } else if (currPlay != "\0") {
      Mix_HaltMusic();
   }


   std::map<std::string, Mix_Music*>::iterator iter = musics.find(keyName);


   if (iter == musics.end()) {
      std::cerr << "failed to find: " << keyName << " in Music::musics." << std::endl;
      return;
   }

   if(Mix_PlayMusic(iter->second, -1)==-1) {
      std::cerr << "could not play music " << keyName << ", exiting!" << std::endl;
      exit(1);
   }

   currPlay = keyName;
   Mix_VolumeMusic(volume);
   
   if (!gameSettings->musicOn) {
      Mix_PauseMusic();
   }
}

void Music::pauseMusic() {
   if (gameSettings->musicOn) {
      Mix_PauseMusic();
   }
}

void Music::resumeMusic() {
   if (!gameSettings->musicOn) {
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





