/**
 * Display.h
 * A screen that can display additional views of game data (e.g. bloom overlay, effects, etc.)
 * @author Chris Brenton
 * @date 3/28/2011
 */

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "Utility/GlobalUtility.h"

class Display {
   public:
      float displaySize;

      Display(int x, int y, GLuint tex);
      void draw();
      void update(double timediff);
      void toggle();
      bool isEnabled();
      int _x, _y;
      GLuint _tex;
   private:
      bool hidden;
};

#endif
