/**
 * BitmapTextDisplay
 * Class to store one element of data which will be displayed via text in the corner of the screen.
 * Acceptable format is <strin>, <string string string>, <string int string>, or <string double, string>,
 * The text defaults to use the font GLUT_BITMAP_HELVETICA_18, with a white color.
 * Taylor Arnicar / Sterling Hirsh
 * 1-19-11
 */

#include <cstring>
#include "Graphics/GlutUtility.h"
#include "Utility/BitmapTextDisplay.h"
#include <stdio.h>

std::ostringstream BitmapTextDisplay :: sstream;

// Constructor if you are displaying one string
BitmapTextDisplay :: BitmapTextDisplay(std::string text, double x, double y) {
   textToDisplay = text;
   // Set the pre & post strings for this BitmapTextDisplay to be empty.
   pre = "";
   post = "";
   xCoord = x;
   yCoord = y;
   // Set the default color to white
   r = g = b = 1;
   font = GLUT_BITMAP_HELVETICA_18;
}

// Constructor if you are displaying multiple strings
BitmapTextDisplay :: BitmapTextDisplay(std::string preText, std::string body, std::string postText, double x, double y) {
   textToDisplay = preText + body + postText;
   // Set the pre & post strings for this BitmapTextDisplay accordingly.
   pre = preText;
   post = postText;
   xCoord = x;
   yCoord = y;
   // Set the default color to white
   r = g = b = 1;
   font = GLUT_BITMAP_HELVETICA_18;
}

// Constructor if you are displaying an int
BitmapTextDisplay :: BitmapTextDisplay(std::string preText, int body, std::string postText, double x, double y) {
   sstream << preText << body << postText;
   // Get a string out of the osteringstream
   textToDisplay = sstream.str();
   sstream.str("");
   // Set the pre & post strings for this BitmapTextDisplay accordingly.
   pre = preText;
   post = postText;

/*
   textToDisplay = preText;
   string temp;
   // Convert the int to a string, using the decimal numbering system
   textToDisplay += itoa(body, temp, 10);
   textToDisplay += postText;
*/
   xCoord = x;
   yCoord = y;
   // Set the default color to white
   r = g = b = 1;
   font = GLUT_BITMAP_HELVETICA_18;
}

// Constructor if you are displaying a double
BitmapTextDisplay :: BitmapTextDisplay(std::string preText, double body, std::string postText, double x, double y) {
   sstream << preText << body << postText;
   // Get a string out of the osteringstream
   textToDisplay = sstream.str();
   sstream.str("");
   // Set the pre & post strings for this BitmapTextDisplay accordingly.
   pre = preText;
   post = postText;
   xCoord = x;
   yCoord = y;
   // Set the default color to white
   r = g = b = 1;
   font = GLUT_BITMAP_HELVETICA_18;
}

// Destructor
BitmapTextDisplay :: ~BitmapTextDisplay() {
   // There's not a whole lot to destruct here.
   // Possibly call delete font; ?
}

// Update the middle body text
void BitmapTextDisplay :: updateBody(std::string newText) {
   textToDisplay = pre + newText + post;
}

// Update the middle body int
void BitmapTextDisplay :: updateBody(int newInt) {
   sstream.str("");
   sstream << pre << newInt << post;
   // Get a string out of the osteringstream
   textToDisplay = sstream.str();

}

// Update the middle body double
void BitmapTextDisplay :: updateBody(double newDouble) {
   sstream.str("");
   sstream << pre << newDouble << post;
   // Get a string out of the osteringstream
   textToDisplay = sstream.str();
}

/* Possible font constants include:
 GLUT_BITMAP_8_BY_13
 GLUT_BITMAP_9_BY_15
 GLUT_BITMAP_TIMES_ROMAN_10
 GLUT_BITMAP_TIMES_ROMAN_24
 GLUT_BITMAP_HELVETICA_10
 GLUT_BITMAP_HELVETICA_12
 GLUT_BITMAP_HELVETICA_18
*/
void BitmapTextDisplay :: setFont(void *newFont) {
   /* Please excuse the giant verification block.
    * It's necessary, and it at least looks better grouped as such. */
   if (newFont == GLUT_BITMAP_8_BY_13 ||
       newFont == GLUT_BITMAP_9_BY_15 ||
       newFont == GLUT_BITMAP_TIMES_ROMAN_10 ||
       newFont == GLUT_BITMAP_TIMES_ROMAN_24 ||
       newFont == GLUT_BITMAP_HELVETICA_10 ||
       newFont == GLUT_BITMAP_HELVETICA_12 ||
       newFont == GLUT_BITMAP_HELVETICA_18)
      font = newFont;
}

void BitmapTextDisplay :: setColor(float newR, float newG, float newB) {
   r = newR;
   g = newG;
   b = newB;
}

// Sets the position where the text will be drawn
void BitmapTextDisplay :: setPosition(double x, double y) {
   xCoord = x;
   yCoord = y;
}

// Pass by reference. This function is called with two doubles, and it fills in the coordinates there.
void BitmapTextDisplay :: getPosition(double &x, double &y) {
/* Alternatively, you can do this to pass using a pointer
void BitmapTextDisplay :: getPosition(double* x, double* y)
   *x = xCoord;
   *y = yCoord;
*/

   x = xCoord;
   y = yCoord;
}

// Function used to draw an entire string to the screen at a certain position
void BitmapTextDisplay :: renderBitmapString(std::string str) {

   glRasterPos2f(p2wx(xCoord), p2wy(yCoord));
   for (unsigned i = 0; i < str.length(); i++) {
     glutBitmapCharacter(font, str[i]);
   }
}

/*TODO: Possible optimization - extract all the lookAt, useOrtho, lighting, and color code
  into a function that calls all of the draw() functions for text that we'll be doing each frame.
  This way, all of this looking, ortho, lighting, and color function are only done once.
*/
// Takes in the x and y position of where to draw the text
void BitmapTextDisplay :: draw() {
   glPushMatrix();
      // Set the camera using the location of your eye, the location where you're looking at, and the up vector.
      //gluLookAt(0, 0, 0.25, 0, 0, 0, 0, 1, 0);

      //useOrtho();
      // We need to disable the lighting temporarily in order to set the color properly.
      //glDisable(GL_LIGHTING);
      // Make the text white
      glColor3f(r, g, b);

      renderBitmapString(textToDisplay.c_str());

      //glEnable(GL_LIGHTING);
      //usePerspective();

   glPopMatrix();
      
}
