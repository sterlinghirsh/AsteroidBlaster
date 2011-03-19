/**
 * Text class
 * Stores everything you need to draw text on screen. Uses GlobalUtility just for GW/GH info so very easy to export
 * to another project. It can handle simple string, string-int-string, string-string-string and string-double-string.
 * @author Ryuho Kudo <ryuhokudo@gmail.com>
 * @date 3/11/11
 */

#include "Text.h"
#include "Utility/GlobalUtility.h"
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <math.h>

#ifdef WIN32
#include "Utility/WindowsMathLib.h"
#endif

#define TEXT_INVERT_VALUE 20

// Constructor if you are displaying one string
//Text::Text(std::string text, std::string fontName, SDL_Rect _pos, int _size, bool _centered) {
Text::Text(std::string text, TTF_Font* _font, SDL_Rect _pos) {
   font = _font;
   sstream << text;
   textToDisplay = sstream.str();
   pos = _pos;
   color = SDL_WHITE;
   selectable = selected = false;

   alignment = LEFT_ALIGN;
   
}

// Constructor if you are displaying multiple strings
Text::Text(std::string preText, std::string body, std::string postText, TTF_Font* _font, SDL_Rect _pos) {
   font = _font;
   
   sstream << preText << body << postText;
   textToDisplay = preText + body + postText;
   // Set the pre & post strings for this Text accordingly.
   pre = preText;
   post = postText;
   pos = _pos;
   color = SDL_WHITE;
   selectable = selected = false;
   
   alignment = LEFT_ALIGN;
   
}

// Constructor if you are displaying an int
Text::Text(std::string preText, int body, std::string postText, TTF_Font* _font, SDL_Rect _pos) {
   font = _font;
   
   sstream << preText << body << postText;
   // Get a string out of the osteringstream
   textToDisplay = sstream.str();
   sstream.str("");
   // Set the pre & post strings for this Text accordingly.
   pre = preText;
   post = postText;
   pos = _pos;
   color = SDL_WHITE;
   selectable = selected = false;
   alignment = LEFT_ALIGN;
}

// Constructor if you are displaying a double
Text::Text(std::string preText, double body, std::string postText, TTF_Font* _font, SDL_Rect _pos) {
   font = _font;
   
   sstream << preText << body << postText;
   // Get a string out of the osteringstream
   textToDisplay = sstream.str();
   sstream.str("");
   // Set the pre & post strings for this Text accordingly.
   pre = preText;
   post = postText;
   pos = _pos;
   color = SDL_WHITE;
   selectable = selected = false;
   alignment = LEFT_ALIGN;
}

// Destructor
Text::~Text() {
   // There's not a whole lot to destruct here.
   // Possibly call delete font; ?
   
}

// Update the middle body text
void Text::updateBody(std::string newText) {
   textToDisplay = pre + newText + post;
}

// Update the middle body int
void Text::updateBody(int newInt) {
   sstream.str("");
   sstream << pre << newInt << post;
   // Get a string out of the osteringstream
   textToDisplay = sstream.str();
}

// Update the middle body double
void Text::updateBody(double newDouble) {
   sstream.str("");
   sstream << pre << newDouble << post;
   // Get a string out of the osteringstream
   textToDisplay = sstream.str();
}

void Text::setFont(TTF_Font* _font) {
      font = _font;
}

void Text::setColor(SDL_Color _color) {
   color = _color;
}

// Sets the position where the text will be drawn
void Text::setPosition(SDL_Rect val) {
   pos = val;
}

// Pass by reference. This function is called with two doubles, and it fills in the coordinates there.
SDL_Rect Text::getPosition() {
   return pos;
}

void Text::draw() {
   /* Go in HUD-drawing mode */
   glEnable2D();
   glDisable(GL_DEPTH_TEST);
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   /* Draw some text */
   /** A quick note about position.
   * Enable2D puts the origin in the lower-left corner of the
   * screen, which is different from the normal coordinate
   * space of most 2D api's. position, therefore,
   * gives the X,Y coordinates of the lower-left corner of the
   * rectangle **/
   
   //SO let's translate it back to standard SDL (normal coordinate)
   
   SDL_Rect temp = {pos.x, GH - pos.y - TEXT_INVERT_VALUE};

   if(!selectable) { selected = false;}

   if(selected) {
      SDL_GL_RenderText(textToDisplay.c_str(), font, SDL_RED, &temp);
   } else {
      SDL_GL_RenderText(textToDisplay.c_str(), font, color, &temp);
   }

   
   
   pos.w = temp.w * 1.5;
   pos.h = temp.h;
   //if (centered) {
   if (alignment == CENTERED)
   {
      // Centered.
      pos.x -= temp.w / 2;
   }
   else if (alignment == RIGHT_ALIGN)
   {
      // Right Aligned.
      pos.x -= temp.w;
   }

   /* Come out of HUD mode */
   glEnable(GL_DEPTH_TEST);
   glDisable2D();

}

void Text::glEnable2D()
{
	int vPort[4];
  
	glGetIntegerv(GL_VIEWPORT, vPort);
  
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
  
	glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void Text::glDisable2D()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();   
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	
}

void Text::SDL_GL_RenderText(const char *text, 
                      TTF_Font *font,
                      SDL_Color color,
                      SDL_Rect *location)
{
	SDL_Surface *initial;
	SDL_Surface *intermediary;
	SDL_Rect rect;
	int w,h;
	
	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(font, text, color);
	
	/* Convert the rendered text to a known format */
	w = (int)(pow(2,ceil(log((double)initial->w) / log(2.0)) + 0.5));
	h = (int)(pow(2,ceil(log((double)initial->h) / log(2.0)) + 0.5));
	
	intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	SDL_BlitSurface(initial, 0, intermediary, 0);
	
	/* Tell GL about our new texture */
	glGenTextures(1, &textTexture);
	glBindTexture(GL_TEXTURE_2D, textTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, 
			GL_UNSIGNED_BYTE, intermediary->pixels );
			
	/* GL_NEAREST looks horrible, if scaled... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	/* prepare to render our texture */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textTexture);
	glColor3f(1.0f, 1.0f, 1.0f);

        /* Center the text */
        double textX = location->x;
        double textW = w;
        //if (centered) {
        if (alignment == CENTERED)
        {
           // Centered.
           textX -= initial->w / 2.0;
        }
        else if (alignment == RIGHT_ALIGN)
        {
           textX -= initial->w;
        }

	/* Draw a quad at location */
	glBegin(GL_QUADS);
		/* Recall that the origin is in the lower-left corner
		   That is why the TexCoords specify different corners
		   than the Vertex coors seem to. */
		glTexCoord2f(0.0f, 1.0f); 
			glVertex2f(textX, location->y);
		glTexCoord2f(1.0f, 1.0f); 
			glVertex2f(textX + textW, location->y);
		glTexCoord2f(1.0f, 0.0f); 
			glVertex2f(textX + textW, location->y + h);
		glTexCoord2f(0.0f, 0.0f); 
			glVertex2f(textX, location->y + h);
	glEnd();
	
	/* Bad things happen if we delete the texture before it finishes */
	glFinish();
	
	/* return the deltas in the unused w,h part of the rect */
	int width = -1;
	int height = -1;
   TTF_SizeText(font,text, &width, &height);
	location->w = textW / 2;
	location->h = height;
	
	/* Clean up */
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &textTexture);
}

bool Text::mouseSelect(int x, int y) {
   if(!selectable) { return false;}
   if(x >= pos.x &&
      x <= pos.x + pos.w &&
      y >= pos.y - TEXT_INVERT_VALUE &&
      y <= pos.y - TEXT_INVERT_VALUE + pos.h){
      return true;
   }
   return false;
}

bool Text::mouseSelect(double x, double y) {
   return mouseSelect((int) x, (int) y);
}

void Text::mouseHighlight(int x, int y) {
   if(!selectable) { return;}
   if(x >= pos.x &&
      x <= pos.x + pos.w &&
      y >= pos.y - TEXT_INVERT_VALUE &&
      y <= pos.y - TEXT_INVERT_VALUE + pos.h){
      selected = true;
   } else {
      selected = false;
   }
}



