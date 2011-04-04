/**
 * @file
 * Point3D implementation.
 *
 * @author Sterling Hirsh, Mike Smith
 */

#include "Utility/Point3D.h"
#include "Utility/Vector3D.h"
#include <math.h>

/** Doubles have a lot of precision.
 * We want to cut that down a bit.
 */
#define EPSILON 0.001

Point3D::Point3D(const Point3D& copy) {
   x = copy.x;
   y = copy.y;
   z = copy.z;
}

Point3D::Point3D(const Vector3D& copy) {
   x = copy.x;
   y = copy.y;
   z = copy.z;
}

const Point3D Point3D::Zero(0, 0, 0);

const Point3D Point3D::operator+(const Point3D &rhs) const {
   return Point3D(x + rhs.x, y + rhs.y, z + rhs.z);
}

const Point3D Point3D::operator-(const Point3D &rhs) const {
   return Point3D(x - rhs.x, y - rhs.y, z - rhs.z);
}

const double Point3D::operator*(const Point3D &rhs) const {
   return x * rhs.x + y * rhs.y + z * rhs.z;
}

Point3D& Point3D::operator/=(double scalar) {
   x /= scalar;
   y /= scalar;
   z /= scalar;

   return *this;
}

bool Point3D::operator==(const Point3D &rhs) const {
   if (fabs(x - rhs.x) < EPSILON)
      if (fabs(y - rhs.y) < EPSILON)
         if (fabs(z - rhs.z) < EPSILON)
            return true;
   return false;
}

Point3D& Point3D::operator=(const Point3D &src) {
   if (&src == this)
      return *this;

   x = src.x;
   y = src.y;
   z = src.z;

   return *this;
}

const Point3D Point3D::operator*(double scalar) const {
   return Point3D(x * scalar, y * scalar, z * scalar);
}

const Point3D Point3D::normalize() const {
   double len = magnitude();

   if (len < EPSILON)
      return Point3D::Zero;

   if (*this == Point3D::Zero)
      return Point3D::Zero;

   return Point3D(x / len, y / len, z / len);
}

const double Point3D::magnitude() const
{
   return sqrt(x * x + y * y + z * z);
}

const Point3D Point3D::operator^(const Point3D &rhs) const {
  return Point3D(
   (y * rhs.z) - (z * rhs.y),
   (z * rhs.x) - (x * rhs.z),
   (x * rhs.y) - (y * rhs.x));
}

void Point3D::clone(Point3D* other) {
   x = other->x;
   y = other->y;
   z = other->z;
}

void Point3D::update(double x2, double y2, double z2) {
   x = x2;
   y = y2;
   z = z2;
}
   
double Point3D::distanceFrom(Point3D& rhs) {
   return distance3D(rhs.x - x, rhs.y - y, rhs.z - z);
}

void Point3D::draw() {
   glVertex3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
}

void Point3D::print() {
   printf("(%f, %f, %f)\n", x, y, z);
}

void Point3D::glTranslate() {
   glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z);
}

void Point3D::offsetBy(double x2, double y2, double z2) {
   x += x2;
   y += y2;
   z += z2;
}

Point3D Point3D::add(Point3D other) {
   Point3D answer;
   answer.x = x + other.x;
   answer.y = y + other.y;
   answer.z = z + other.z;
   return answer;
}

void Point3D::midpoint(const Point3D& p1, const Point3D& p2) {
   x = (p1.x + p2.x) / 2;
   y = (p1.y + p2.y) / 2;
   z = (p1.z + p2.z) / 2;
}
