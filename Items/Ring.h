#include "Utility/Point3D.h"
#include <vector>

class Ring {
  public:
    Ring() {};
    Ring(int pts, double rad, double h);
    ~Ring() {};
    void generate();
    void draw(Ring prev);
    void draw(int at);
    double maxX();
    double minX();
    double maxY();
    double minY();
    double maxZ();
    double minZ();
    Point3D makePt(double angle);
    int upCur(Ring prev, int ndx);
    int size();
    std::vector<Point3D> _pList;
    Point3D _head;
    int _pts;
    double _rad;
    double _h;
    double _offset;
};
