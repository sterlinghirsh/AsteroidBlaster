/**
 * WeaponDisplay.cpp
 * Displays a progress bar on the hud, for things like cooldown or health.
 * @author Chris Brenton
 * @date 4/30/2011
 */

#ifndef __WEAPONDISPLAY_H__
#define __WEAPONDISPLAY_H__

#include <string>
#include <vector>

class GameState;

class WeaponDisplay {
   public:
      WeaponDisplay(float _height, float _width, float _x, float _y, const GameState *_gameState);
      void draw();
      void setIcon(std::string _texture);
   private:
      std::vector<int> weaponList;
      const GameState* gameState;
      bool isVertical;
      float outerBoxThickness;
      // Height, width, x, and y are in world coords.
      float height;
      float width;
      float x;
      float y;
      std::string icon;
};

#endif
