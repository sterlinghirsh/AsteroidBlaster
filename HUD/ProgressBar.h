/**
 * ProgressBar.cpp
 * Displays a progress bar on the hud, for things like cooldown or health.
 * @author Sterling Hirsh (shirsh@calpoly.edu)
 * @date 2/22/2011
 */

#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__

#include <string>

class ProgressBar {
   public:
      ProgressBar(float _height, float _width, float _x, float _y);
      void setAmount(float _amount);
      void setVertical(bool vert);
      void draw();
      void setIcon(std::string _texture);
   private:
      bool isVertical;
      float outerBoxThickness;
      float amount; // The amount full the progress bar is, from 0 to 1.
      // Height, width, x, and y are in world coords.
      float height;
      float width;
      float x;
      float y;
      bool hasIcon;
      std::string icon;
};

#endif
