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
      void setHorizontal(bool vert);
      void setSkew(float _xSkew, float _ySkew);
      void draw();
      void setCentered(bool isCentered);
      void setIcon(std::string _texture);
      
      float height;
      float width;
      float x;
      float y;
      bool centered;
   private:
      // Height, width, x, and y are in world coords.
      float amount; // The amount full the progress bar is, from 0 to 1.
      float outerBoxThickness;
      bool hasIcon;
      bool isHorizontal;
      float xSkew;
      float ySkew;
      std::string icon;
};

#endif
