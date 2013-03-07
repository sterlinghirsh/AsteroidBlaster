
#include "SDL.h"
#include "Utility/GlobalUtility.h"
#include "Utility/GameState.h"
#include "Text/Text.h"
#include "Text/Input.h"
#include <cctype>

Input::Input(GameState*& _gameState) : gameState(_gameState) {
   active = false;
   x = y = -1;

   ignoreNext = false;
   
   SDL_Rect position = {0,0};
   std::string fontName = DEFAULT_FONT;

   SDL_EnableUNICODE(1);

   text = new Text("", menuFont, position);
   text->alignment = CENTERED;
}

Input::~Input() {
   delete text;
}

void Input::update(double timeDiff) {
   return;
}

void Input::draw() {
   //text->textToDisplay += line;
   //text->textToDisplay = line;
   text->textToDisplay = "TEST";
   SDL_Rect position;
   position.y = (Sint16) (gameSettings->GH/20 * 19);
   position.x = (Sint16) (gameSettings->GW/8);
   text->setPosition(position);

   //fboBegin();
   glDrawBuffer(HUD_BUFFER);
   // Clear the screen
   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //draw the text
   glPushMatrix();
   useOrtho();
   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);

   text->draw();

   usePerspective();
   glPopMatrix();

   glEnable(GL_LIGHTING);
   //fboEnd();

   //SDL_GL_SwapBuffers();

}

/**
 * Handles the player pressing down a key
 */
void Input::keyDown(int key, int unicode) {
   if (!active) { return; }
   if (ignoreNext) {
      ignoreNext = false;
      return;
   }

   if (key == SDLK_RETURN || key == SDLK_ESCAPE) {
      deactivate();
   } else if (key == SDLK_DELETE || key == SDLK_BACKSPACE) {
      if (line.length() > 0)
         line.erase(line.length() - 1);
   } else if(unicode < 0x80 && unicode > 0 ) {
      line += (char)unicode;
   }

   return;
}

/**
 * Handles the player letting go of a key
 */
void Input::keyUp(int key) {
   if (!active) { return; }
}

/**
 * Handles the player clicking the mouse
 */
void Input::mouseDown(int button) {
   return;
}

/**
 * Handles the player letting go of a mouse click
 */
void Input::mouseUp(int button) {
   return;
}

void Input::mouseMove(int dx, int dy, int x, int y) {
   return;
}

void Input::activate(bool _ignoreNext) {
   active = true;
   line = "";
   ignoreNext = _ignoreNext;
   SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}

void Input::deactivate() {
   printf("line: %s\n", line.c_str());
   active = false;
   SDL_EnableKeyRepeat(0, 0);
}
