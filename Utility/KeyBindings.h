/**
 * KeyBindings
 * Stores global settings for the game.
 * @author Sterling Hirsh
 * @date 4-12-11
 * CPE 476
 */

#ifndef __KEYBINDINGS_H__
#define __KEYBINDINGS_H__

#include<map>
#include<string>

class KeyBindings {
   public:
      KeyBindings();
      ~KeyBindings();
      
      static void writeOut();
      static void readIn();

      static int mapGet(std::string keyName);
      static void mapSet(std::string keyName, int value);
      
   private:
      static std::map<std::string, int> KeyBindingsMap;
};

#endif
