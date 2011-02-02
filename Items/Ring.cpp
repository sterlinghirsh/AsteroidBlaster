/**
 * Chris Brenton
 * Ring.cpp
 * A semi-randomly generated ring of points for an asteroid object.
 */
#include <math.h>
#include "Items/Asteroid3D.h"

#define RADIUS 1.0
#define OFFSET_MIN 0.0
#define OFFSET_MAX M_PI / 10
#define TWEAK_MIN -(RADIUS / 30.0)
#define TWEAK_MAX _rad / 5.0
#define LINE_W 5
//ORANGE
//#define LINE_CLR glColor3f(0.996, 0.612, 0.0)
//YELLOW
//#define LINE_CLR glColor3f(1.0, 0.902, 0.302)
//CYAN
#define LINE_CLR glColor3f(0.325, 0.71, 0.808)
#define AST_CLR glColor3f(0.0, 0.0, 0.0)

using namespace std;

Ring::Ring(int points, double radius, double h) :
  _pts(points), _rad(radius), _h(h) {
    //cout << "points (in ring) : " << points << endl;
    //srand(time(0));
    _offset = (double)rand() / (double)RAND_MAX * (OFFSET_MAX - OFFSET_MIN) + OFFSET_MIN;
    generate();
  }

void Ring::draw(Ring prev) {
  double step = (double)size() / (double)prev.size();
  double count = 0.0;
  while(count < size() && prev.size() != 1 && size() != 1) {
    int prevPt = upCur(prev, (int)count - 1);
    while (prevPt >= prev.size()) {
      prevPt -= prev.size();
    }
    while (prevPt <= 0) {
      prevPt += prev.size();
    }

    AST_CLR;
    glBegin(GL_TRIANGLES);
    _pList[(int)(count) % size()].draw();
    prev.draw(prevPt % prev.size());
    int prevNdx = (int)(count - 1) % size();
    while (prevNdx < 0) {
      prevNdx += size();
    }
    draw(prevNdx);
    glEnd();

    glBegin(GL_TRIANGLES);
    int nextNdx = (upCur(prev, (int)count) + size()) % size();
    _pList[(int)(count) % size()].draw();
    prev.draw(prevPt % prev.size());
    prev.draw(nextNdx);
    glEnd();

    count+= step;
  }
  for (int i = 0; i < size(); i++) {
    int nextNdx = (i + 1) % size();
    int upPt = upCur(prev, i);

    AST_CLR;
    glBegin(GL_TRIANGLES);
    _pList[i].draw();
    _pList[nextNdx].draw();
    prev.draw(upPt);
    glEnd();

    LINE_CLR;
    glLineWidth(LINE_W);
    glBegin(GL_LINE_LOOP);
    _pList[i].draw();
    _pList[nextNdx].draw();
    prev.draw(upPt);
    glEnd();
    glLineWidth(1);

  }
}

void Ring::draw(int at) {
  _pList[at].draw();
}

int Ring::upCur(Ring prev, int ndx) {
  double count = 0.0;
  double step = (double)size() / (double)prev.size();
  int ret = 0;
  while((int)count <= ndx) {
    ret++;
    count+= step;
  }
  ret -= 1;
  if (ret >= prev.size()) {
    ret = prev.size() - 1;
  }
  return ret;
}

double Ring::maxX() {
  double max = _pList[0].x;
  for (unsigned i = 0; i < _pList.size(); i++) {
    if (_pList[i].x > max) {
      max = _pList[i].x;
    }
  }
  return max;
}

double Ring::minX() {
  double min = _pList[0].x;
  for (unsigned i = 0; i < _pList.size(); i++) {
    if (_pList[i].x < min) {
      min = _pList[i].x;
    }
  }
  return min;
}

double Ring::maxY() {
  double max = _pList[0].y;
  for (unsigned i = 0; i < _pList.size(); i++) {
    if (_pList[i].y > max) {
      max = _pList[i].y;
    }
  }
  return max;
}

double Ring::minY() {
  double min = _pList[0].y;
  for (unsigned i = 0; i < _pList.size(); i++) {
    if (_pList[i].y < min) {
      min = _pList[i].y;
    }
  }
  return min;
}

double Ring::maxZ() {
  double max = _pList[0].z;
  for (unsigned i = 0; i < _pList.size(); i++) {
    if (_pList[i].z > max) {
      max = _pList[i].z;
    }
  }
  return max;
}

double Ring::minZ() {
  double min = _pList[0].z;
  for (unsigned i = 0; i < _pList.size(); i++) {
    if (_pList[i].z < min) {
      min = _pList[i].z;
    }
  }
  return min;
}

void Ring::generate() {
  _head = makePt(0.0);
  _pList.push_back(_head);
  for (int i = 1; i < _pts; i++) {
    _pList.push_back(makePt(M_PI * 2 / _pts * i + _offset));
  }
  //printf("\ngenerated\n\tsize = %d\n\tpts = %d\n", size(), _pts);
}

Point3D Ring::makePt(double angle) {
  double tweakX = (double)rand() / (double)RAND_MAX * TWEAK_MAX;
  double tweakY = (double)rand() / (double)RAND_MAX * TWEAK_MAX;
  double tweakZ = (double)rand() / (double)RAND_MAX * TWEAK_MAX;
  double tmpx = _rad * cos(angle) + tweakX;
  double tmpy = _h + tweakY;
  double tmpz = _rad * sin(angle) + tweakZ;
  Point3D tmpPt = Point3D(tmpx, tmpy, tmpz);
  return tmpPt;
}

int Ring::size() {
  return _pList.size();
}
