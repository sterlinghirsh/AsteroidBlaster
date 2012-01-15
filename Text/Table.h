/**
 * I guess I'm writing a table class.
 * @author Sterling Hirsh
 * @date 1/15/2012
 */

#ifndef __TABLE_H__
#define __TABLE_H__

#include <string>
#include <vector>

class Table {
   public:
      Table(std::vector<std::string> _headers, std::string _title);
      void setCell(unsigned row, unsigned column, std::string newString);
      void setCell(unsigned row, unsigned column, int newInt);
      void setRows(int _numRows);
      void setColWidth(unsigned col, unsigned relativeWidth);
      void draw();
   private:
      std::vector<std::string> headers;
      unsigned numColumns;
      unsigned numRows;
      unsigned totalColWidths;
      int colPadding;
      int rowHeight;
      int topPadding;
      std::vector< std::vector<std::string> > rows;
      std::vector<unsigned> colWidths;
      std::string title;
};

#endif
