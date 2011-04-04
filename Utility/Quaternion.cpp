

#include <math.h>
#include <algorithm>
#include "Vector3D.h"
#include "Quaternion.h"

#include "Utility/GlobalUtility.h"


using namespace std;


Quaternion::Quaternion(){
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Quaternion::Quaternion(double a, double b, double c, double d){
	x = a;
	y = b;
	z = c;
	w = d;
}


void Quaternion::normalize(){
	double mag2 = w * w + x * x + y * y + z * z;
	if (  mag2!=0.f && (fabs(mag2 - 1.0f) > .00001f)) {
		double mag = sqrt(mag2);
		w /= mag;
		x /= mag;
		y /= mag;
		z /= mag;
	}

}

Quaternion Quaternion::getConjugate()
{
	return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::operator* (const Quaternion &rq) const
{
	// the constructor takes its arguments as (x, y, z, w)
	return Quaternion(w * rq.x + x * rq.w + y * rq.z - z * rq.y,
	                  w * rq.y + y * rq.w + z * rq.x - x * rq.z,
	                  w * rq.z + z * rq.w + x * rq.y - y * rq.x,
	                  w * rq.w - x * rq.x - y * rq.y - z * rq.z);
}

Vector3D Quaternion::operator*(Vector3D &vec)
{
	Vector3D vn = vec;
	vn.normalize();
 
	Quaternion vecQuat, resQuat;
	vecQuat.x = vn.x;
	vecQuat.y = vn.y;
	vecQuat.z = vn.z;
	vecQuat.w = 0.0f;
 
	resQuat = vecQuat * getConjugate();
	resQuat = *this * resQuat;
 
	return (Vector3D(resQuat.x, resQuat.y, resQuat.z));
}

Point3D Quaternion::operator*(Point3D &vec)
{
	Point3D vn = vec;
	vn.normalize();
 
	Quaternion vecQuat, resQuat;
	vecQuat.x = vn.x;
	vecQuat.y = vn.y;
	vecQuat.z = vn.z;
	vecQuat.w = 0.0f;
 
	resQuat = vecQuat * getConjugate();
	resQuat = *this * resQuat;
 
	return (Point3D(resQuat.x, resQuat.y, resQuat.z));
}

void Quaternion::FromAxis(const Vector3D &v, double angle)
{
	double sinAngle;
	double otherAngle = angle * .5;
	double cosAngle;
	//angle *= 0.5f;
	Vector3D vn = v;
	vn.normalize();
 
	sinAngle = sin(otherAngle);
 	cosAngle = cos(otherAngle);
	x = (vn.x * sinAngle);
	y = (vn.y * sinAngle);
	z = (vn.z * sinAngle);
	w = cosAngle;
}

void Quaternion::FromAxis(const Point3D &v, double angle)
{
	double sinAngle;
	double otherAngle = angle * .5;
	double cosAngle;
   Point3D vn = v.normalize(); 

	sinAngle = sin(otherAngle);
 	cosAngle = cos(otherAngle);
	x = (vn.x * sinAngle);
	y = (vn.y * sinAngle);
	z = (vn.z * sinAngle);
	w = cosAngle;
}

void Quaternion::FromEuler(double pitch, double yaw, double roll)
{
	// Basically we create 3 Quaternions, one for pitch, one for yaw, one for roll
	// and multiply those together.
	// the calculation below does the same, just shorter
 
	double p = pitch * 3.14159265/180;
	double y = yaw * 3.14159265/180;
	double r = roll * 3.14159265/180;
 
	double sinp = sin(p);
	double siny = sin(y);
	double sinr = sin(r);
	double cosp = cos(p);
	double cosy = cos(y);
	double cosr = cos(r);
 
	this->x = sinr * cosp * cosy - cosr * sinp * siny;
	this->y = cosr * sinp * cosy + sinr * cosp * siny;
	this->z = cosr * cosp * siny - sinr * sinp * cosy;
	this->w = cosr * cosp * cosy + sinr * sinp * siny;
 
	normalize();
}

void Quaternion::getAxisAngle(Vector3D *axis, double *angle)
{
	double scale = sqrt(x * x + y * y + z * z);
	axis->x = x / scale;
	axis->y = y / scale;
	axis->z = z / scale;
	*angle = acos(w) * 2.0f;
}

void Quaternion::getAxisAngle(Point3D *axis, double *angle)
{
	double scale = sqrt(x * x + y * y + z * z);
	axis->x = x / scale;
	axis->y = y / scale;
	axis->z = z / scale;
	*angle = acos(w) * 2.0f;
}

/*void Quaternion::Rotate(Vector3D rotVec, double angle){
	double sinCoeff;
        Quaternion rotQuat;
        Quaternion pointQuat;
        Quaternion conjQuat;
        Quaternion temp;
 
        sinCoeff=sin(angle*0.5);
 
        rotQuat.w = cos(angle*0.5);
 
        rotQuat.x=sinCoeff*rotVec.x;
        rotQuat.y=sinCoeff*rotVec.y;
        rotQuat.z=sinCoeff*rotVec.z;
 
        pointQuat.a =0
        pointQuat.b = 0
        pointQuat.c = 0
        pointQuat.d = 0
        //calculate conjugate of the quaternion
        conjQuat.a = rotQuat.a
        conjQuat.b = -rotQuat.b
        conjQuat.c = -rotQuat.c
        conjQuat.d = -rotQuat.d
 
        //perform  rotation
        temp=quat_mult(rotQuat,pointQuat)
        point=quat_pointmult(temp,conjQuat)

}*/

void Quaternion::getMatrix(GLfloat* matri)
{
	GLfloat x2 = (GLfloat) (x * x);
	GLfloat y2 = (GLfloat) (y * y);
	GLfloat z2 = (GLfloat) (z * z);
	GLfloat xy = (GLfloat) (x * y);
	GLfloat xz = (GLfloat) (x * z);
	GLfloat yz = (GLfloat) (y * z);
	GLfloat wx = (GLfloat) (w * x);
	GLfloat wy = (GLfloat) (w * y);
	GLfloat wz = (GLfloat) (w * z);
    
   GLfloat matrix[16] = {
  			1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
			2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
			2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			};
   
   for (int i = 0; i < 16; i++) {
      matri[i] = matrix[i];
   }
	//GLfloat *other = (GLfloat*)matri;
	// This calculation would be a lot more complicated for non-unit length quaternions
	// Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
	//   OpenGL
	//return matri;
}
/*
Quaternion Quaternion::slerp(const Quat &q1, const Quat &q2, float t) {
  Quaternion result, _q2 = q2;

  float cosOmega = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;

  if ( cosOmega < 0.0f ) {
    _q2.x = -_q2.x;
    _q2.y = -_q2.y;
    _q2.z = -_q2.z;
    _q2.w = -_q2.w;
    cosOmega = -cosOmega;
  }

  float k0, k1;
  if ( cosOmega > 0.99999f ) {
    k0 = 1.0f - t;
    k1 = t;
  }
  else {
    float sinOmega = (float)sqrt( double(1.0f - cosOmega*cosOmega) );
    float omega = (float)atan2( double(sinOmega), double(cosOmega) );

    float invSinOmega = 1.0f/sinOmega;

    k0 = float( sin(double((1.0f - t)*omega)) )*invSinOmega;
    k1 = float( sin(double(t*omega)) )*invSinOmega;
  }

  for ( int i=0; i < 4; i++ )
    result[i] = q1[i]*k0 + _q2[i]*k1;

  return result;
}*/

void Quaternion::print(){
	printf("X: %f, ", x);
	printf("Y: %f, ", y);
	printf("Z: %f, ", z);
	printf("W: %f\n", w);
}






