/**
 * KeyBindings
 * Stores keybindings for the game.
 * @author Ryuho Kudo
 * @date 4-21-11
 * CPE 476
 */

#include "Utility/KeyBindings.h"

#define CONFIG_FILE "keyBind.cfg"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::map<std::string, int> KeyBindings::KeyBindingsMap;

KeyBindings::KeyBindings() {
   // Default Settings.
   
   KeyBindingsMap["bloom"] = 0;
   KeyBindingsMap["fullscreen"] = 0;
   KeyBindingsMap["soundOn"] = 1;
   KeyBindingsMap["musicOn"] = 1;
   KeyBindingsMap["mouseCapture"] = 0;
   
   KeyBindingsMap["windowedGW"] = 800;
   KeyBindingsMap["windowedGH"] = 600;
   KeyBindingsMap["fullscreenGW"] = 1280;
   KeyBindingsMap["fullscreenGH"] = 1024;
   
   readIn();

   if(KeyBindingsMap.find("fullscreen")->second == 1) {
      KeyBindingsMap["GW"] = KeyBindingsMap.find("fullscreenGW")->second;
      KeyBindingsMap["GH"] = KeyBindingsMap.find("fullscreenGH")->second;
   } else {
      KeyBindingsMap["GW"] = KeyBindingsMap.find("windowedGW")->second;
      KeyBindingsMap["GH"] = KeyBindingsMap.find("windowedGH")->second;
   }
   
}

KeyBindings::~KeyBindings() {

}

int KeyBindings::mapGet(std::string keyName) {
   return KeyBindingsMap.find(keyName)->second;
}

void KeyBindings::mapSet(std::string keyName, int value) {
   KeyBindingsMap[keyName] = value;
}
 
void KeyBindings::toggleFullScreen() {
   if(KeyBindingsMap.find("fullscreen")->second == 1) {
      KeyBindingsMap["fullscreen"] = 0;
   } else {
      KeyBindingsMap["fullscreen"] = 1;
   }
   
   if(KeyBindingsMap.find("fullscreen")->second == 1) {
      KeyBindingsMap["GW"] = KeyBindingsMap.find("fullscreenGW")->second;
      KeyBindingsMap["GH"] = KeyBindingsMap.find("fullscreenGH")->second;
   } else {
      KeyBindingsMap["GW"] = KeyBindingsMap.find("windowedGW")->second;
      KeyBindingsMap["GH"] = KeyBindingsMap.find("windowedGH")->second;
   }
}

/**
 * Write game settings to a file.
 */
void KeyBindings::writeOut() {
   std::cout << "Writing config..." << std::endl;
   std::ofstream myfile (CONFIG_FILE, std::ofstream::out);
   
   if (!myfile.is_open()) { 
      std::cerr << "Could not open " << CONFIG_FILE << "!" << std::endl;
      exit(1);
   }
   
   std::map<std::string, int>::iterator iter = KeyBindingsMap.begin();
   
   for(; iter != KeyBindingsMap.end(); ++iter) {
      std::cout << iter->first << " " << iter->second << std::endl;
   }
   
   myfile << "bloom " << KeyBindingsMap.find("bloom")->second << std::endl;
   myfile << "fullscreen " << KeyBindingsMap.find("fullscreen")->second << std::endl;
   myfile << "musicOn " << KeyBindingsMap.find("musicOn")->second << std::endl;
   myfile << "soundOn " << KeyBindingsMap.find("soundOn")->second << std::endl;
   
   myfile << "windowedGW " << KeyBindingsMap.find("windowedGW")->second << std::endl;
   myfile << "windowedGH " << KeyBindingsMap.find("windowedGH")->second << std::endl;
   myfile << "fullscreenGW " << KeyBindingsMap.find("fullscreenGW")->second << std::endl;
   myfile << "fullscreenGH " << KeyBindingsMap.find("fullscreenGH")->second << std::endl;

   myfile.close();
   std::cout << "Done!" << std::endl;
}

/**
 * Read game settings from a file.
 */
void KeyBindings::readIn() {
   std::cout << "Reading config..." << std::endl;
   
   std::ifstream myfile (CONFIG_FILE, std::ifstream::in);

   if (!myfile.is_open()) {
      std::cerr << "Could not open " << CONFIG_FILE << "!" << std::endl;
      exit(1);
   }

   int index = -1;
   std::string readLine;
   std::string keyName = "";
   int mapValue = -1;
   
   while (myfile.good()) {
      getline (myfile, readLine);
      //if blank line or start with #, skip
      if(readLine.size() == 0 || readLine.at(0) == '#') {
         continue;
      }
      keyName = readLine.substr(0, readLine.find(" "));
      std::stringstream convert(readLine.substr(readLine.find(" ")+1));
      convert >> mapValue;
      KeyBindingsMap.insert(std::pair<std::string, int>(keyName,mapValue));
      keyName = "";
      mapValue = -1;
   }

   myfile.close();
   return;
   
}


