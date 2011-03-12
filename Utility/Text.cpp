/**
 * Text
 * Class to store one element of data which will be displayed via text in the corner of the screen.
 * Acceptable format is <strin>, <string string string>, <string int string>, or <string double, string>,
 * The text defaults to use the font GLUT_BITMAP_HELVETICA_18, with a white color.
 * Taylor Arnicar / Sterling Hirsh
 * 1-19-11
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "Text.h"


// Constructor if you are displaying one string
Text::Text(std::string text, double x, double y) {
   font = TTF_OpenFont("FreeMonoBold.ttf", 12);
   if(!font)
   {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
      exit(3);
   }
   
   sstream << text;
   textToDisplay = sstream.str();
   pos.x = x;
   pos.y = y;
   color.r = color.g = color.b = 0.0;
   
}

// Constructor if you are displaying multiple strings
Text::Text(std::string preText, std::string body, std::string postText, double x, double y) {
   font = TTF_OpenFont("FreeMono.ttf", 12);
   if(!font)
   {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
      exit(3);
   }
   
   sstream << preText << body << postText;
   textToDisplay = preText + body + postText;
   // Set the pre & post strings for this Text accordingly.
   pre = preText;
   post = postText;
   pos.x = x;
   pos.y = y;
   color.r = color.g = color.b = 0.0;
   
}

// Constructor if you are displaying an int
Text::Text(std::string preText, int body, std::string postText, double x, double y) {
   font = TTF_OpenFont("FreeMono.ttf", 12);
   if(!font)
   {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
      exit(3);
   }
   
   sstream << preText << body << postText;
   // Get a string out of the osteringstream
   textToDisplay = sstream.str();
   sstream.str("");
   // Set the pre & post strings for this Text accordingly.
   pre = preText;
   post = postText;
   pos.x = x;
   pos.y = y;
   color.r = color.g = color.b = 0.0;
}

// Constructor if you are displaying a double
Text::Text(std::string preText, double body, std::string postText, double x, double y) {
   font = TTF_OpenFont("FreeMono.ttf", 12);
   if(!font)
   {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
      exit(3);
   }
   
   sstream << preText << body << postText;
   // Get a string out of the osteringstream
   textToDisplay = sstream.str();
   sstream.str("");
   // Set the pre & post strings for this Text accordingly.
   pre = preText;
   post = postText;
   pos.x = x;
   pos.y = y;
   color.r = color.g = color.b = 0.0;
}

// Destructor
Text::~Text() {
   // There's not a whole lot to destruct here.
   // Possibly call delete font; ?
}

// Update the middle body text
void Text::updateBody(std::string newText) {
   textToDisplay = pre + newText + post;
   //textSurface = TTF_RenderText_Shaded(font, textToDisplay.c_str(), foregroundColor, backgroundColor);
}

// Update the middle body int
void Text::updateBody(int newInt) {
   sstream.str("");
   sstream << pre << newInt << post;
   // Get a string out of the osteringstream
   textToDisplay = sstream.str();
   //textSurface = TTF_RenderText_Shaded(font, textToDisplay.c_str(), foregroundColor, backgroundColor);
}

// Update the middle body double
void Text::updateBody(double newDouble) {
   sstream.str("");
   sstream << pre << newDouble << post;
   // Get a string out of the osteringstream
   textToDisplay = sstream.str();
   //textSurface = TTF_RenderText_Shaded(font, textToDisplay.c_str(), foregroundColor, backgroundColor);
}

void Text::setFont(int newFont) {

}

void Text::setColor(SDL_Color _color) {
   color = _color;
   //textSurface = TTF_RenderText_Shaded(font, textToDisplay.c_str(), foregroundColor, backgroundColor);
}

// Sets the position where the text will be drawn
void Text::setPosition(SDL_Rect val) {
   pos = val;
}

// Pass by reference. This function is called with two doubles, and it fills in the coordinates there.
SDL_Rect Text::getPosition() {
   return pos;
}

/*TODO: Possible optimization - extract all the lookAt, useOrtho, lighting, and color code
  into a function that calls all of the draw() functions for text that we'll be doing each frame.
  This way, all of this looking, ortho, lighting, and color function are only done once.
*/
// Takes in the x and y position of where to draw the text
void Text::draw() {
      
   /* Go in HUD-drawing mode */
   glEnable2D();
   glDisable(GL_DEPTH_TEST);

   /* Draw some text */
   color.r = 255;
   color.g = 255;
   color.b = 255;
   /** A quick note about position.
   * Enable2D puts the origin in the lower-left corner of the
   * screen, which is different from the normal coordinate
   * space of most 2D api's. position, therefore,
   * gives the X,Y coordinates of the lower-left corner of the
   * rectangle **/
   SDL_GL_RenderText(textToDisplay.c_str(), font, color, &pos);

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
	w = (int)(pow(2,ceil(log(initial->w) / log(2)) + 0.5));
	h = (int)(pow(2,ceil(log(initial->h) / log(2)) + 0.5));
	
	intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	SDL_BlitSurface(initial, 0, intermediary, 0);
	
	/* Tell GL about our new texture */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, 
			GL_UNSIGNED_BYTE, intermediary->pixels );
			
	/* GL_NEAREST looks horrible, if scaled... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	/* prepare to render our texture */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);
	
	/* Draw a quad at location */
	glBegin(GL_QUADS);
		/* Recall that the origin is in the lower-left corner
		   That is why the TexCoords specify different corners
		   than the Vertex coors seem to. */
		glTexCoord2f(0.0f, 1.0f); 
			glVertex2f(location->x    , location->y);
		glTexCoord2f(1.0f, 1.0f); 
			glVertex2f(location->x + w, location->y);
		glTexCoord2f(1.0f, 0.0f); 
			glVertex2f(location->x + w, location->y + h);
		glTexCoord2f(0.0f, 0.0f); 
			glVertex2f(location->x    , location->y + h);
	glEnd();
	
	/* Bad things happen if we delete the texture before it finishes */
	glFinish();
	
	/* Clean up */
	glDeleteTextures(1, &texture);
}
