/**
 * I guess I'm writing a table class.
 * @author Sterling Hirsh
 * @date 1/15/2012
 */

#include "Utility/GlobalUtility.h"
#include "Text/Table.h"
#include "Text/Text.h"
#include <assert.h>
#include <iostream>

using namespace std;

static SDL_Rect textPosition = {0, 0};
Text* tableDisplayText = NULL;

Table::Table(std::vector<std::string> _headers, std::string _title) :
 headers(_headers),
 numColumns(_headers.size()),
 colWidths(numColumns, 1),
 title(_title)
 {
    numRows = 0;

    colPadding = 20; // px
    rowHeight = 30; // px
    topPadding = 10; // px
    totalColWidths = numColumns;
    highlight = false;
    highlightRow = 0;
   
    if (tableDisplayText == NULL)
      tableDisplayText = new Text("ScoreDisplay", hudFont, textPosition);
}

void Table::setRows(int _numRows) {
   numRows = _numRows;
   // Allocate extra space.
   while (numRows > rows.size()) {
      vector<string> tmpRow(numColumns, "");
      rows.push_back(tmpRow);
   }
}

void Table::setColWidth(unsigned col, unsigned relativeWidth) {
   assert(col < numColumns);
   
   colWidths[col] = relativeWidth;

   totalColWidths = 0;
   for (unsigned curCol = 0; curCol < numColumns; ++curCol) {
      totalColWidths += colWidths[curCol];
   }
}

void Table::setCell(unsigned row, unsigned column, string newString) {
   assert(row < numRows);
   assert(column < numColumns);
   rows[row][column] = newString;
};

void Table::setCell(unsigned row, unsigned column, int newInt) {
   assert(row < numRows);
   assert(column < numColumns);

   ostringstream oss;
   oss << newInt;

   rows[row][column] = oss.str();
};

void Table::draw() {
   glClear(GL_DEPTH_BUFFER_BIT);
   glDisable(GL_LIGHTING);
   glDisable(GL_CULL_FACE);

   int margin;
   int leftSide;
   int rightSide;
   int eyeOffset;

   if (drawStereo_enabled) {
      margin = 25;
      rightSide = gameSettings->GW / 2;
      leftSide = 0;
      eyeOffset = stereo_eye_left ? 5 : -5;
      leftSide += eyeOffset;
      rightSide += eyeOffset;
   } else {
      margin = 50; // px
      leftSide = 0;
      rightSide = gameSettings->GW;
      eyeOffset = 0;
   }

   leftSide += margin;
   rightSide -= margin;
   
   int top = margin;
   int bottom = gameSettings->GH - margin;
   
   // Draw a quad.
   glColor4f(0, 0, 0, 0.75);
   glBegin(GL_QUADS);
      glVertex2f(p2wx(leftSide), p2wy(top));
      glVertex2f(p2wx(rightSide), p2wy(top));
      glVertex2f(p2wx(rightSide), p2wy(bottom));
      glVertex2f(p2wx(leftSide), p2wy(bottom));
   glEnd();

   int tableWidth = (rightSide - leftSide) - (numColumns * colPadding);

   int baseColWidth = tableWidth / totalColWidths;
   int colStart = leftSide + colPadding;
   
   textPosition.y = top + topPadding + rowHeight;

   // Print Title

   textPosition.x = leftSide + ((rightSide - leftSide) / 2);
   tableDisplayText->alignment = CENTERED;
   tableDisplayText->setColor(SDL_YELLOW);
   tableDisplayText->setPosition(textPosition);
   tableDisplayText->updateBody(title);
   tableDisplayText->draw();
   textPosition.y += rowHeight;
   
   // Print Column Headers
   tableDisplayText->setColor(SDL_RED);
   tableDisplayText->alignment = LEFT_ALIGN;
   
   for (unsigned col = 0; col < numColumns; ++col) {
      // Set position.
      textPosition.x = colStart;

      tableDisplayText->setPosition(textPosition);
      tableDisplayText->updateBody(headers[col]);
      tableDisplayText->draw();

      colStart += (colWidths[col] * baseColWidth) + colPadding;
   }
   
   for (unsigned row = 0; row < numRows; ++row) {
      if (highlight && row == highlightRow) {
         tableDisplayText->setColor(SDL_GREEN);
      } else {
         tableDisplayText->setColor(SDL_WHITE);
      }

      colStart = leftSide + colPadding;
      textPosition.y += rowHeight;

      for (unsigned col = 0; col < numColumns; ++col) {
         // Set position.
         textPosition.x = colStart;
         
         tableDisplayText->setPosition(textPosition);
         tableDisplayText->updateBody(rows[row][col]);
         tableDisplayText->draw();

         colStart += (colWidths[col] * baseColWidth) + colPadding;
      }
   }
}

void Table::setHighlight(unsigned row) {
   assert (row < numRows);
   highlightRow = row;
   highlight = true;
}

void Table::clearHighlight() {
   highlight = false;
}
