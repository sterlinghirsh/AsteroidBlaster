
#include <math.h>
#include <algorithm>
#include "Vector3D.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Utility/GlobalUtility.h"

using namespace std;

// Default, magnitude-only, and magnitude-and-position constructors all in one.
Vector3D::Vector3D(double x, double y, double z, double a, 
 double b, double c) : xMag(x), yMag(y), zMag(z), xPos(a), yPos(b), 
zPos(c) {}

Vector3D::Vector3D(Point3D point) {
   updateMagnitude(point);
   updatePosition(0, 0, 0);
}

Vector3D::Vector3D(Point3D p1, Point3D p2) {
   updateMagnitude(p1, p2);
   updatePosition(p1);
}

void Vector3D::movePoint(Point3D& point, double scale) {
   point.x += xMag * scale;
   point.y += yMag * scale;
   point.z += zMag * scale;
}

void Vector3D::updateMagnitude(Point3D point) {
   updateMagnitude(point.x, point.y, point.z);
}

void Vector3D::updateMagnitude(Vector3D inVector) {
   updateMagnitude(inVector.xMag, inVector.yMag, inVector.zMag);
}

void Vector3D::updateMagnitude(Vector3D* inVector) {
   updateMagnitude(inVector->xMag, inVector->yMag, inVector->zMag);
}

void Vector3D::updateMagnitude(Point3D p1, Point3D p2) {
   updateMagnitude(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
}

void Vector3D::updateMagnitude(Point3D* p1, Point3D* p2) {
   updateMagnitude(p2->x - p1->x, p2->y - p1->y, p2->z - p1->z);
}

void Vector3D::updatePosition(Point3D point) {
   updatePosition(point.x, point.y, point.z);
}

void Vector3D::draw() {
   glBegin(GL_LINES);
   glVertex3f(xPos, yPos, zPos);
   glVertex3f(xPos + xMag * drawScale, yPos + yMag * drawScale, zPos + zMag * drawScale);
   glEnd();
}

void Vector3D::addNormal() {
   glNormal3f(xMag, yMag, zMag);
}

void Vector3D::normalize() {
   double length = getLength();
   if (length == 0)
      return;

   xMag = xMag / length;
   yMag = yMag / length;
   zMag = zMag / length;
}

void Vector3D::updateMagnitude(double x, double y, double z) {
   xMag = x;
   yMag = y;
   zMag = z;
}

void Vector3D::randomMagnitude() {
   updateMagnitude(2 * randdouble() - 1, 2 * randdouble() - 1,
    2 * randdouble() - 1);
}

void Vector3D::updatePosition(double x, double y, double z) {
   xPos = x;
   yPos = y;
   zPos = z;
}

void Vector3D::update(double x, double y, double z, double a, double b, double c) {
   updateMagnitude(x, y, z);
   updatePosition(a, b, c);
}

/**
 * Return a double of the distance from head to tail.
 */
double Vector3D::getLength() {
   return distance3D(xMag, yMag, zMag);
}

/**
 * Returns a double that represents the 
 * distance from head to tail squared.
 * This allows us to do comparisons on the
 * length without doing a costly sqrt();
 */
double Vector3D::getComparisonLength() {
   return (xMag * xMag) + (yMag * yMag) + 
   (zMag * zMag);
}

/**
 * This returns the angle between two vectors.
 */
double Vector3D::getAngleInRadians(Vector3D& rhs) {
   double l1 = getLength(), l2 = rhs.getLength();
   if (l1 <= 0 || l2 <= 0)
      return 0;

   double cosValue = dot(rhs) / (l1 * l2);
   if (cosValue <= -1 || cosValue >= 1)
      return 0;
   return acos(cosValue);
}

double Vector3D::getAngleInDegrees(Vector3D& rhs) {
   return getAngleInRadians(rhs) * 180 / 3.14159265;
}

double Vector3D::dot(Vector3D& rhs) {
   return (xMag * rhs.xMag) + (yMag * rhs.yMag) + (zMag * rhs.zMag);
}

Vector3D Vector3D::cross(Vector3D& rhs) {
   Vector3D product(
    (yMag * rhs.zMag) - (zMag * rhs.yMag),
    (zMag * rhs.xMag) - (xMag * rhs.zMag),
    (xMag * rhs.yMag) - (yMag * rhs.xMag));
   return product;
}

void Vector3D::scalarMultiplyUpdate(double scalar) {
   xMag *= scalar;
   yMag *= scalar;
   zMag *= scalar;
}

Vector3D Vector3D::scalarMultiply(double scalar) {
   Vector3D product (xMag * scalar, yMag * scalar, zMag * scalar);
   return product;
}

void Vector3D::setLength(double newLength) {
   normalize();
   xMag *= newLength;
   yMag *= newLength;
   zMag *= newLength;
}

/**
 * Return a new vector with the sum of the magnitudes and the pos of this vector.
 */
Vector3D Vector3D::add(Vector3D rhs) {
   Vector3D sum(xMag + rhs.xMag, yMag + rhs.yMag, zMag + rhs.zMag,
    xPos, yPos, zPos);
   return sum;
}

/**
 * Return a new vector with the difference of the magnitudes and the 
 * pos of this vector.
 */
Vector3D Vector3D::subtract(Vector3D& rhs) {
   Vector3D sum(xMag - rhs.xMag, yMag - rhs.yMag, zMag - rhs.zMag,
    xPos, yPos, zPos);
   return sum;
}

void Vector3D::subtractUpdate(const Vector3D rhs) {
   xMag -= rhs.xMag;
   yMag -= rhs.yMag;
   zMag -= rhs.zMag;
}

/**
 * Add rhs to this vector and save the result in this vector.
 */
void Vector3D::addUpdate(Vector3D rhs) {
   xMag += rhs.xMag;
   yMag += rhs.yMag;
   zMag += rhs.zMag;
}

void Vector3D::addToPosition(Vector3D& rhs) {
   xPos += rhs.xMag;
   yPos += rhs.yMag;
   zPos += rhs.zMag;
}

void Vector3D::updateFromVirtualTrackball(double xi, double yi) {
   // Some stuff.
   double length = distance2D(xi, yi);
   if (length > 1) {
      xMag = xi/length;
      yMag = yi/length;
      zMag = 0;
   } else {
      xMag = xi;
      yMag = yi;
      zMag = sqrt(1 - ((xi * xi) + (yi * yi)));
   }
}

void Vector3D::updateFromVirtualTrackball(int xp, int yp) {
   updateFromVirtualTrackball(p2ix(xp), p2iy(yp));
}

void Vector3D::setCamera(double upx = 0, double upy = 1, double upz = 0) {
   gluLookAt(xPos, yPos, zPos, xPos + xMag, yPos + yMag, zPos + zMag, upx, upy, upz);
}

/**
 * Angle is in radians.
 * Axis will be called u, v, w.
 * Found the body of this function at:
 * http://www.blitzbasic.com/Community/posts.php?topic=57616
 */
void Vector3D::rotate(double angle, const Vector3D& axis) {
   double x = xMag;
   double y = yMag;
   double z = zMag;
   double u = axis.xMag;
   double v = axis.yMag;
   double w = axis.zMag;
   double ux = x * u;
   double uy = y * u;
   double uz = z * u;
   double vx = x * v;
   double vy = y * v;
   double vz = z * v;
   double wx = x * w;
   double wy = y * w;
   double wz = z * w;
   double sa = sin(angle);
   double ca = cos(angle);
   xMag = u * (ux+vy+wz) + (x * (v*v + w*w) - u * (vy+wz)) * ca + (-wy+vz) * sa;
   yMag = v * (ux+vy+wz) + (y * (u*u + w*w) - v * (ux+wz)) * ca + (wx-uz)  * sa;
   zMag = w * (ux+vy+wz) + (z * (u*u + v*v) - w * (ux+vy)) * ca + (-vx+uy) * sa;
   normalize();
}

void Vector3D::rotateByDegrees(double angle, const Vector3D& axis) {
   rotate(angle / (180 / 3.14159265), axis);
}

void Vector3D::glTranslate(double length = 1) {
   glTranslatef(length * xMag, length * yMag, length * zMag);
}

void Vector3D::reflect(Vector3D& axis) {
   double x = xMag;
   double y = yMag;
   double z = zMag;
   double u = axis.xMag;
   double v = axis.yMag;
   double w = axis.zMag;
   double l_dot_n = dot(axis);

   xMag = -x + (2 * l_dot_n * u);
   yMag = -y + (2 * l_dot_n * v);
   zMag = -z + (2 * l_dot_n * w);
}

Vector3D Vector3D::getNormalVector() {
   double closestToZero = min(fabs(xMag), min(fabs(yMag), fabs(zMag)));
   Vector3D otherPoint(1, 1, 1);

   if (fabs(xMag) == closestToZero) {
      otherPoint.updateMagnitude(1, 0, 0);
   } else if (fabs(yMag) == closestToZero) {
      otherPoint.updateMagnitude(0, 1, 0);
   } else {
      otherPoint.updateMagnitude(0, 0, 1);
   }
   Vector3D normal(cross(otherPoint));
   return normal;
}

void Vector3D::negativeX() {
   if (xMag > 0)
      xMag = 0 - xMag;
}

void Vector3D::positiveX() {
   if (xMag < 0)
      xMag = 0 - xMag;
}


void Vector3D::negativeY() {
   if (yMag > 0)
      yMag = 0 - yMag;
}

void Vector3D::positiveY() {
   if (yMag < 0)
      yMag = 0 - yMag;
}


void Vector3D::negativeZ() {
   if (zMag > 0)
      zMag = 0 - zMag;
}

void Vector3D::positiveZ() {
   if (zMag < 0)
      zMag = 0 - zMag;
}

Vector3D &Vector3D::operator=(const Vector3D& rhs) {
   xMag = rhs.xMag;
   yMag = rhs.yMag;
   zMag = rhs.zMag;
   xPos = rhs.xPos;
   yPos = rhs.yPos;
   zPos = rhs.zPos;

   return *this;
}
