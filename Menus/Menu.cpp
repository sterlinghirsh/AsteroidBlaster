
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Menus/Menu.h"
#include "Utility/Image.h"
#include "Utility/Texture.h"
#include "Utility/Text.h"

Menu::Menu() {
   menuActive = false;
   x = 0;
   y = 0;
}


Menu::~Menu() {
   for(int i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
}


