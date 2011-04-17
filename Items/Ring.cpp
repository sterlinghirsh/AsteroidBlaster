/**
 * Chris Brenton
 * Ring.cpp
 * A semi-randomly generated ring of points for an asteroid object.
 */
#define _USE_MATH_DEFINES
#include <math.h>
#include "Items/Asteroid3D.h"

#define RADIUS 1.0
#define OFFSET_MIN 0.0
#define OFFSET_MAX M_PI / 10
#define TWEAK_MIN -(RADIUS / 30.0)
#define TWEAK_MAX _rad / 15.0
#define LINE_W 5
//ORANGE
//#define LINE_CLR glColor3f(0.996, 0.612, 0.0)
//YELLOW
//#define LINE_CLR glColor3f(1.0, 0.902, 0.302)
//CYAN
#define LINE_CLR glColor3f(0.325, 0.71, 0.808)
#define AST_CLR glColor3f(0.0, 0.0, 0.0)

using namespace std;

Ring::Ring(int points, double radius, double h, Mesh3D *mesh) :
   _pts(points), _rad(radius), _h(h), _mesh(mesh) {
      //cout << "points (in ring) : " << points << endl;
      //srand(time(0));
      _offset = (double)rand() / (double)RAND_MAX * (OFFSET_MAX - OFFSET_MIN) + OFFSET_MIN;
      generate();
   }

void Ring::draw(int at) {
   _pList[at].draw();
}

void Ring::generate() {
   _head = makePt(0.0);
   _pList.push_back(_head);
   _nList.push_back(_mesh->addPoint(&_head));
   for (int i = 1; i < _pts; i++) {
      MeshPoint tmpPt = makePt(M_PI * 2 / _pts * i + _offset);
      _pList.push_back(tmpPt);
      _nList.push_back(_mesh->addPoint(&tmpPt));
   }
}

MeshPoint Ring::makePt(double angle) {
   double tweakX = (double)rand() / (double)RAND_MAX * TWEAK_MAX;
   double tweakY = (double)rand() / (double)RAND_MAX * TWEAK_MAX;
   double tweakZ = (double)rand() / (double)RAND_MAX * TWEAK_MAX;
   double tmpx = _rad * cos(angle) + tweakX;
   double tmpy = _h + tweakY;
   double tmpz = _rad * sin(angle) + tweakZ;
   double u, v;
   v = acos(tmpz / _rad) / M_PI;
   if (tmpy >= 0)
      u = acos(tmpx / (_rad * sin(M_PI * v))) / (2 * M_PI);
   else
      u = (M_PI + acos(tmpx / (_rad * sin(M_PI * v)))) / (2 * M_PI);
   /*
   printf("u0: %f, u1: %f, v0: %f\n", M_PI * v, 
         tmpx / (_rad * sin(M_PI * v)), tmpz / _rad);
   printf("u: %f, v: %f\n", u, v);
   */
   MeshPoint tmpPt = MeshPoint(tmpx, tmpy, tmpz);
   return tmpPt;
}

int Ring::size() {
   return (int) _pList.size();
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

double Ring::maxFromCenter() {
   double max = 0.0;
   MeshPoint center(0.0, 0.0, 0.0);
   for (unsigned i = 0; i < _pList.size(); i++) {
      double tmpD = _pList[i].distanceFrom(center);
      if (tmpD > max) {
         max = tmpD;
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
