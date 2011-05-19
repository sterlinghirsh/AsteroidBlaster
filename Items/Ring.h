#include <vector>

class Ring {
  public:
    Ring() {};
    Ring(int pts, double rad, double h, Mesh3D *mesh);
    ~Ring() {};
    int size();
    void draw(int at);
    void generate();
    MeshPoint makePt(double angle);
    int upCur(Ring prev, int ndx);
    double maxFromCenter();
    double maxX();
    double minX();
    double maxY();
    double minY();
    double maxZ();
    double minZ();
    std::vector<Point3D> _pList;
    std::vector<int> _nList;
    MeshPoint _head;
    int _pts;
    double _rad;
    double _h;
    double _offset;
    Mesh3D *_mesh;
};
