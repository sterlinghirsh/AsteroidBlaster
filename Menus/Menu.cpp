
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Menus/Menu.h"
#include "Graphics/Texture.h"
#include "Text/Text.h"

Menu::Menu() {
   menuActive = false;
   x = 0;
   y = 0;
}


Menu::~Menu() {
   for(unsigned i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
}

void Menu::update(double timeDiff) {
   // Sleep by default to avoid wasting CPU.
   SDL_Delay(1);
}

