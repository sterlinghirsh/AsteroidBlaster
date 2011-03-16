
#include <iostream>
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Utility/GameState.h"
#include "Menus/CreditsMenu.h"
#include "Utility/Image.h"
#include "Utility/Texture.h"
#include "Utility/Music.h"
#include "Utility/SoundEffect.h"

#define TITLE_STRING_INDEX 0
//#define QUIT_STRING_INDEX 6
#define QUIT_STRING_INDEX (menuTexts.size() - 1)
#define TITLE_TYPE 0
#define DUAL_NAME_TYPE 1
#define SINGLE_NAME_TYPE 2

CreditsMenu::CreditsMenu() {
   menuActive = false;
   x = y = -1;
   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT;

   // Feel free to change your nickname, everyone.
   // Just make sure it's better than the one I gave you.
   addTitle("The Boyz");
   addDualColName("Taylor \'\'White Chocolate\'\' Arnicar");
   addDualColName("Chris \'\'Rolling Thunder\'\' Brenton");
   addDualColName("Sterling \'\'Breakin\' Evil\'\' Hirsh");
   addDualColName("Jake \'\'Jacuzzi\'\' Juszak");
   addDualColName("Ryuho \'\'Silent But Deadly\'\' Kudo");
   // AI boyz, please pick nicknames for yourselves (or better yet, for each
   // other). Pretty much anything would be better than these.
   addTitle("The A.I. Boyz");
   addDualColName("Sean \'\'GEOCELL\'\' Ghiocel");
   addDualColName("Justin \'\'The Coon\'\' Kuehn");
   addDualColName("Mike \'\'Dammit Mike\'\' Smith");
   addTitle("The Music Man");
   addSingleColName("George Michael Himself");
   
   menuTexts.push_back(new Text("Return to Main Menu", fontName, position, 18));

   menuTexts[QUIT_STRING_INDEX]->selectable = true;

   for (int i = 0; i < QUIT_STRING_INDEX; i++) {
      //menuTexts[i]->centered = true;
      menuTexts[i]->alignment = CENTERED;
   }
   menuTexts[QUIT_STRING_INDEX]->alignment = RIGHT_ALIGN;

   firstDrawn = false;
   yOffset = 0.0;
}

CreditsMenu::~CreditsMenu() {
   for(int i = 0; i < menuTexts.size(); i++) {
      delete menuTexts[i];
   }
}

void CreditsMenu::addTitle(std::string name)
{
   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT; 
   menuTexts.push_back(new Text(name, fontName, position, 48));
   types.push_back(TITLE_TYPE);
}

void CreditsMenu::addDualColName(std::string name)
{
   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT;
   menuTexts.push_back(new Text(name, fontName, position, 24));
   types.push_back(DUAL_NAME_TYPE);
}

void CreditsMenu::addSingleColName(std::string name)
{
   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT;
   menuTexts.push_back(new Text(name, fontName, position, 28));
   types.push_back(SINGLE_NAME_TYPE);
}

void CreditsMenu::update(double ms)
{
   if (!firstDrawn)
   {
      firstDrawn = true;
      yOffset = 0.0;
      lastTick = ms;
      draw();
   }
   else
   {
      //yOffset += SCROLL_RATE * ms;
      yOffset -= 10.0 * (ms - lastTick);
      //printf("tick %f\n", (ms - lastTick));
      lastTick = ms;
      draw();
   }
}

void CreditsMenu::draw() {
   double yPos = 0.0;
   SDL_Rect position;
   position.x = 0.0;
   yPos = 0.0;
   position.y = yPos + yOffset;
   //position.y = GH/5;
   //menuTexts[TITLE_STRING_INDEX]->setPosition(position);
   for(int i = 0; i < menuTexts.size() - 1; i++) {
      int j = 0;
      while (types[i + j] == 0 && i + j < menuTexts.size() - 1)
      {
         yPos += GH/5;
         position.x = GW/2;
         position.y = yPos + yOffset;
         menuTexts[i + j]->setPosition(position);
         j++;
      }
      int k = j;
      while ((types[i + j] == DUAL_NAME_TYPE
               || types[i + j] == SINGLE_NAME_TYPE)
            && i + j < menuTexts.size() - 1)
      {
         if (types[i + j] == DUAL_NAME_TYPE)
         {
            position.x = GW/4 + (GW/2 * ((j - k) % 2));
            yPos += GH / 10 * (j % 2);
         }
         else
         {
            position.x = GW/2;
            yPos += GH / 10 * 1.5;
         }
         position.y = yPos + yOffset;
         menuTexts[i + j]->setPosition(position);
         j++;
      }
      i += j - 1;
   }
   position.x = GW;
   yPos = GH;
   position.y = yPos;
   menuTexts[QUIT_STRING_INDEX]->setPosition(position);

   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //draw the text
   glPushMatrix();
   useOrtho();
   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);

   /*
      glColor3f(0.0, 1.0, 0.0);
      glBegin(GL_QUADS);
      glVertex2f(0.0, 0.0);
      glVertex2f(0.0, 1.0);
      glVertex2f(1.0, 1.0);
      glVertex2f(1.0, 0.0);
      glEnd();
      */

   for(int i = 0; i < menuTexts.size(); i++) {
      int loc = menuTexts[i]->getPosition().y + yOffset;
      if (loc < GH * 1.3 + menuTexts[i]->size && loc > GH * -3.0)
      {
         menuTexts[i]->draw();
      }
   }
   /*
   if (menuTexts[QUIT_STRING_INDEX - 1]->getPosition().y + yOffset < GH / 2)
   {
      printf("last item at halfway point.\n");
   }
   */

   usePerspective();
   glPopMatrix();

   glEnable(GL_TEXTURE_2D);

   /* Enable smooth shading */
   glShadeModel( GL_SMOOTH );

   /* Depth buffer setup */
   glClearDepth( 1.0f );

   /* Enables Depth Testing */
   glEnable( GL_DEPTH_TEST );

   /* The Type Of Depth Test To Do */
   glDepthFunc( GL_LEQUAL );

   /* Really Nice Perspective Calculations */
   glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
         GL_CLAMP );
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
         GL_CLAMP );

   glPushMatrix();
   glLoadIdentity( );
   glTranslatef( 0.0f, 0.2f, -1.0f );
   glScalef( 0.3625, 0.1, 1.0 );
   glColor3f(1.0, 1.0, 1.0);

   // Select Our Texture
   /*
      glBindTexture( GL_TEXTURE_2D, Texture::getTexture("Logo.png") );
      */

   /* NOTE:
    *   The x coordinates of the glTexCoord2f function need to inverted
    * for SDL because of the way SDL_LoadBmp loads the data. So where
    * in the tutorial it has glTexCoord2f( 1.0f, 0.0f ); it should
    * now read glTexCoord2f( 0.0f, 1.0f );
    */

   /*
      glBegin(GL_QUADS);
   // Front Face
   // Bottom Left Of The Texture and Quad
   glTexCoord2f( 0.0f, 1.0f ); glVertex2f( -1.0f, -1.0f);
   // Bottom Right Of The Texture and Quad
   glTexCoord2f( 1.0f, 1.0f ); glVertex2f(  1.0f, -1.0f);
   // Top Right Of The Texture and Quad
   glTexCoord2f( 1.0f, 0.0f ); glVertex2f(  1.0f,  1.0f);
   // Top Left Of The Texture and Quad
   glTexCoord2f( 0.0f, 0.0f ); glVertex2f( -1.0f,  1.0f);
   glEnd();
   */
   glEnable(GL_CULL_FACE);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);

   SDL_GL_SwapBuffers();

}

/**
 * Handles the player pressing down a key
 */
void CreditsMenu::keyDown(int key) {
   if (!menuActive) { return; }

   switch(key) {
   }
}

/**
 * Handles the player letting go of a key
 */
void CreditsMenu::keyUp(int key) {
   if (!menuActive) { return; }
}

/**
 * Handles the player clicking the mouse
 */
void CreditsMenu::mouseDown(int button) {
   if (!menuActive) { return; }
   if(menuTexts[QUIT_STRING_INDEX]->mouseSelect(x,y)) {
      menuActive = false;
      firstDrawn = false;
      mainMenu->menuActive = true;
      x = y = -1;
      Music::stopMusic();
      Music::playMusic("8-bit3.ogg");
   }
}

/**
 * Handles the player letting go of a mouse click
 */
void CreditsMenu::mouseUp(int button) {
   if (!menuActive) { return; }
}

void CreditsMenu::mouseMove(int dx, int dy, int _x, int _y) {
   if (!menuActive) { return; }
   x = _x;
   y = _y;

   menuTexts[QUIT_STRING_INDEX]->mouseHighlight(x,y);
}

