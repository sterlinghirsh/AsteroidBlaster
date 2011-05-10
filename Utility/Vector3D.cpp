
#include <math.h>
#include <algorithm>
#include "Vector3D.h"


/** Doubles have a lot of precision.
 * We want to cut that down a bit.
 */
#define EPSILON 0.001

using namespace std;


void Vector3D::movePoint(Vector3D& point, double scale) {
   point.x += x * scale;
   point.y += y * scale;
   point.z += z * scale;
}

void Vector3D::updateMagnitude(Vector3D inVector) {
   updateMagnitude(inVector.x, inVector.y, inVector.z);
}

void Vector3D::updateMagnitude(Vector3D* inVector) {
   updateMagnitude(inVector->x, inVector->y, inVector->z);
}

void Vector3D::updateMagnitude(Vector3D p1, Vector3D p2) {
   updateMagnitude(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
}

void Vector3D::updateMagnitude(Vector3D* p1, Vector3D* p2) {
   updateMagnitude(p2->x - p1->x, p2->y - p1->y, p2->z - p1->z);
}

void Vector3D::draw() {
   glBegin(GL_LINES);
   glVertex3f(0, 0, 0);
   glVertex3d(x, y, z);
   glEnd();
}

void Vector3D::addNormal() {
   glNormal3f((GLfloat) x, (GLfloat) y, (GLfloat) z);
}

void Vector3D::normalize() {
   double length = getLength();
   if (length == 0)
      return;

   x = x / length;
   y = y / length;
   z = z / length;
}

/**
 * Make sure that the vector is all positive.
 * Used by the Shooting AI's chooseTarget().
 */
void Vector3D::abs() {
   if (x < 0.0)
      x *= -1;
   if (y < 0.0)
      y *= -1;
   if (z < 0.0)
      z *= -1;
}

void Vector3D::randomMagnitude() {
   updateMagnitude(2 * randdouble() - 1, 2 * randdouble() - 1,
    2 * randdouble() - 1);
}

/**
 * Return a double of the distance from head to tail.
 */
double Vector3D::getLength() {
   return distance3D(x, y, z);
}

const double Vector3D::magnitude() const
{
   return sqrt(x * x + y * y + z * z);
}


/**
 * Returns a double that represents the 
 * distance from head to tail squared.
 * This allows us to do comparisons on the
 * length without doing a costly sqrt();
 */
double Vector3D::getComparisonLength() {
   return (x * x) + (y * y) + 
   (z * z);
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

Vector3D Vector3D::cross(Vector3D& rhs) {
   Vector3D product(
    (y * rhs.z) - (z * rhs.y),
    (z * rhs.x) - (x * rhs.z),
    (x * rhs.y) - (y * rhs.x));
   return product;
}

Vector3D Vector3D::scalarMultiply(double scalar) {
   Vector3D product (x * scalar, y * scalar, z * scalar);
   return product;
}

void Vector3D::setLength(double newLength) {
   normalize();
   x *= newLength;
   y *= newLength;
   z *= newLength;
}

/**
 * Return a new vector with the sum of the magnitudes and the pos of this vector.
 */
Vector3D Vector3D::add(Vector3D rhs) {
   Vector3D sum(x + rhs.x, y + rhs.y, z + rhs.z);
   return sum;
}

/**
 * Return a new vector with the difference of the magnitudes and the 
 * pos of this vector.
 */
Vector3D Vector3D::subtract(Vector3D& rhs) {
   Vector3D sum(x - rhs.x, y - rhs.y, z - rhs.z);
   return sum;
}

void Vector3D::subtractUpdate(const Vector3D rhs) {
   x -= rhs.x;
   y -= rhs.y;
   z -= rhs.z;
}

/**
 * Add rhs to this vector and save the result in this vector.
 */
void Vector3D::addUpdate(Vector3D rhs) {
   x += rhs.x;
   y += rhs.y;
   z += rhs.z;
}

void Vector3D::updateFromVirtualTrackball(double xi, double yi) {
   // Some stuff.
   double length = distance2D(xi, yi);
   if (length > 1) {
      x = xi/length;
      y = yi/length;
      z = 0;
   } else {
      x = xi;
      y = yi;
      z = sqrt(1 - ((xi * xi) + (yi * yi)));
   }
}

void Vector3D::updateFromVirtualTrackball(int xp, int yp) {
   updateFromVirtualTrackball(p2ix(xp), p2iy(yp));
}

/**
 * Angle is in radians.
 * Axis will be called u, v, w.
 * Found the body of this function at:
 * http://www.blitzbasic.com/Community/posts.php?topic=57616
 */
void Vector3D::rotate(double angle, const Vector3D& axis) {
   double u = axis.x;
   double v = axis.y;
   double w = axis.z;
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
   x = u * (ux+vy+wz) + (x * (v*v + w*w) - u * (vy+wz)) * ca + (-wy+vz) * sa;
   y = v * (ux+vy+wz) + (y * (u*u + w*w) - v * (ux+wz)) * ca + (wx-uz)  * sa;
   z = w * (ux+vy+wz) + (z * (u*u + v*v) - w * (ux+vy)) * ca + (-vx+uy) * sa;
   normalize();
}

void Vector3D::rotateByDegrees(double angle, const Vector3D& axis) {
   rotate(angle / (180 / 3.14159265), axis);
}

void Vector3D::reflect(Vector3D& axis) {
   double u = axis.x;
   double v = axis.y;
   double w = axis.z;
   double l_dot_n = dot(axis);

   x = x - (2 * l_dot_n * u);
   y = y - (2 * l_dot_n * v);
   z = z - (2 * l_dot_n * w);
}

Vector3D Vector3D::getNormalVector() {
   double closestToZero = min(fabs(x), min(fabs(y), fabs(z)));
   Vector3D otherPoint(1, 1, 1);

   if (fabs(x) == closestToZero) {
      otherPoint.updateMagnitude(1, 0, 0);
   } else if (fabs(y) == closestToZero) {
      otherPoint.updateMagnitude(0, 1, 0);
   } else {
      otherPoint.updateMagnitude(0, 0, 1);
   }
   Vector3D normal(cross(otherPoint));
   return normal;
}

void Vector3D::negativeX() {
   if (x > 0)
      x = 0 - x;
}

void Vector3D::positiveX() {
   if (x < 0)
      x = 0 - x;
}


void Vector3D::negativeY() {
   if (y > 0)
      y = 0 - y;
}

void Vector3D::positiveY() {
   if (y < 0)
      y = 0 - y;
}


void Vector3D::negativeZ() {
   if (z > 0)
      z = 0 - z;
}

void Vector3D::positiveZ() {
   if (z < 0)
      z = 0 - z;
}

void Vector3D::clone(Vector3D* other) {
   x = other->x;
   y = other->y;
   z = other->z;
}

void Vector3D::update(double x2, double y2, double z2) {
   x = x2;
   y = y2;
   z = z2;
}
   
double Vector3D::distanceFrom(Vector3D& rhs) {
   return distance3D(rhs.x - x, rhs.y - y, rhs.z - z);
}

void Vector3D::offsetBy(double x2, double y2, double z2) {
   x += x2;
   y += y2;
   z += z2;
}

void Vector3D::midpoint(const Vector3D& p1, const Vector3D& p2) {
   x = (p1.x + p2.x) / 2;
   y = (p1.y + p2.y) / 2;
   z = (p1.z + p2.z) / 2;
}

const Vector3D Vector3D::operator^(const Vector3D &rhs) const {
  return Vector3D(
   (y * rhs.z) - (z * rhs.y),
   (z * rhs.x) - (x * rhs.z),
   (x * rhs.y) - (y * rhs.x));
}


const Vector3D Vector3D::operator+(const Vector3D &rhs) const {
   return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);
}

const Vector3D Vector3D::operator-(const Vector3D &rhs) const {
   return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
}

const double Vector3D::operator*(const Vector3D &rhs) const {
   return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vector3D& Vector3D::operator/=(double scalar) {
   x /= scalar;
   y /= scalar;
   z /= scalar;

   return *this;
}

bool Vector3D::operator==(const Vector3D &rhs) const {
   if (fabs(x - rhs.x) < EPSILON)
      if (fabs(y - rhs.y) < EPSILON)
         if (fabs(z - rhs.z) < EPSILON)
            return true;
   return false;
}

Vector3D& Vector3D::operator=(const Vector3D &src) {
   if (&src == this)
      return *this;

   x = src.x;
   y = src.y;
   z = src.z;

   return *this;
}

const Vector3D Vector3D::operator*(double scalar) const {
   return Vector3D(x * scalar, y * scalar, z * scalar);
}

const Vector3D Vector3D::getNormalized() const {
   Vector3D normalized(*this);
   normalized.normalize();
   return normalized;
}
