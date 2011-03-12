/**
 * Text
 * Class to store one element of data which will be displayed via text in the corner of the screen.
 * Acceptable format is <string>, <string string string>, <string int string>, or <string double, string>
 * Ryuho Kudo
 * 1-19-11
 */
#ifndef __TEXT_H__
#define __TEXT_H__

#include <string>
#include <sstream>
#include "SDL.h"
#include <SDL_ttf.h>

static const SDL_Color WHITE = {255,255,255};
static const SDL_Color BLACK = {0,0,0};
static const SDL_Color BLUE = {0,0,255}; 

class Text {
   private:
      void SDL_GL_RenderText(const char *text, 
                      TTF_Font *font,
                      SDL_Color color,
                      SDL_Rect *location);
                      
      void glEnable2D();
      void glDisable2D();
   public:
      //Constructor if you are displaying one string
      Text(std::string text, std::string fontName, SDL_Rect _pos, int _size);
      // Constructor if you are displaying multiple strings
      Text(std::string preText, std::string body, std::string postText, std::string fontName, SDL_Rect _pos, int _size);
      // Constructor if you are displaying an int
      Text(std::string preText, int body, std::string postText, std::string fontName, SDL_Rect _pos, int _size);
      // Constructor if you are displaying a double
      Text(std::string preText, double body, std::string postText, std::string fontName, SDL_Rect _pos, int _size);

      // Destructor
      virtual ~Text();

      // Sets the new body text to be used
      virtual void updateBody(std::string newText);
      // Sets the new body text to be used
      virtual void updateBody(int newInt);
      // Sets the new body text to be used
      virtual void updateBody(double newDouble);
      // Sets the font to be used
      virtual void setFont(int newFont);
      // Sets the color of the text
      virtual void setColor(SDL_Color _color);
      // Sets the position where the text will be drawn
      virtual void setPosition(SDL_Rect val);
      // Gets the position of the text, and puts it into x and y
      virtual SDL_Rect getPosition();
      // Takes in the x and y position of where to draw the text
      virtual void draw();
      
      TTF_Font* font;
      SDL_Color color;
      SDL_Surface* textSurface;
      SDL_Rect pos;
      int size;
      std::ostringstream sstream;
      std::string textToDisplay, pre, post;
      GLuint texture;
      int w, h;
};

#endif
