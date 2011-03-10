/**
 * BitmapTextDisplay
 * Class to store one element of data which will be displayed via text in the corner of the screen.
 * Acceptable format is <string>, <string string string>, <string int string>, or <string double, string>
 * Taylor Arnicar / Sterling Hirsh
 * 1-19-11
 */

#ifndef __BITMAPTEXTDISPLAY_H__
#define __BITMAPTEXTDISPLAY_H__

#include "Utility/Point3D.h"
#include <string>
#include <sstream>

class BitmapTextDisplay {
   // private variables go here
   std::string textToDisplay, pre, post;
   
   float r, g, b;
   void* font;
   static std::ostringstream sstream;
   
   // Function used to draw an entire string to the screen at a certain position
   void renderBitmapString(std::string str);

   public:
      double xCoord, yCoord;
      //Constructor if you are displaying one string
      BitmapTextDisplay(std::string text, double x, double y);
      // Constructor if you are displaying multiple strings
      BitmapTextDisplay(std::string preText, std::string body, std::string postText, double x, double y);
      // Constructor if you are displaying an int
      BitmapTextDisplay(std::string preText, int body, std::string postText, double x, double y);
      // Constructor if you are displaying a double
      BitmapTextDisplay(std::string preText, double body, std::string postText, double x, double y);

      // Destructor
      virtual ~BitmapTextDisplay();

      // Sets the new body text to be used
      virtual void updateBody(std::string newText);
      // Sets the new body text to be used
      virtual void updateBody(int newInt);
      // Sets the new body text to be used
      virtual void updateBody(double newDouble);
      // Sets the font to be used
      virtual void setFont(void *newFont);
      // Sets the color of the text
      virtual void setColor(float newR, float newG, float newB);
      // Sets the position where the text will be drawn
      virtual void setPosition(double x, double y);
      // Gets the position of the text, and puts it into x and y
      virtual void getPosition(double &x, double &y);
      // Takes in the x and y position of where to draw the text
      virtual void draw();
};

#endif
