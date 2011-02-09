#include "Vector3D.h"
#include <math.h>
#include <algorithm>
#include "Quaternion.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Graphics/GlutUtility.h"


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
	vecQuat.x = vn.xMag;
	vecQuat.y = vn.yMag;
	vecQuat.z = vn.zMag;
	vecQuat.w = 0.0f;
 
	resQuat = vecQuat * getConjugate();
	resQuat = *this * resQuat;
 
	return (Vector3D(resQuat.x, resQuat.y, resQuat.z));
}

void Quaternion::FromAxis(Vector3D &v, double angle)
{
	double sinAngle;
	double otherAngle = (angle / (180/3.14159265)) * .5;
	double cosAngle;
	//angle *= 0.5f;
        printf("Vec XMag: %f\n", v.xMag);
	Vector3D vn = v;
	//vn.normalize();
 
	sinAngle = sin(otherAngle);
 	cosAngle = cos(otherAngle);
	x = (vn.xMag * sinAngle);
	y = (vn.yMag * sinAngle);
	z = (vn.zMag * sinAngle);
	w = cosAngle;
}

void Quaternion::FromEuler(double pitch, double yaw, double roll)
{
	// Basically we create 3 Quaternions, one for pitch, one for yaw, one for roll
	// and multiply those together.
	// the calculation below does the same, just shorter
 
	double p = pitch * 3.14159265/360;
	double y = yaw * 3.14159265/360;
	double r = roll * 3.14159265/360;
 
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
	axis->xMag = x / scale;
	axis->yMag = y / scale;
	axis->zMag = z / scale;
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
 
        rotQuat.x=sinCoeff*rotVec.xMag;
        rotQuat.y=sinCoeff*rotVec.yMag;
        rotQuat.z=sinCoeff*rotVec.zMag;
 
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

GLfloat* Quaternion::getMatrix()
{
	printf("X: %f, ", x);
	printf("Y: %f, ", y);
	printf("Z: %f, ", z);
	printf("W: %f\n", w);
	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;
 
        GLfloat matri[16] = {
  			1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
			2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
			2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			};

	//GLfloat *other = (GLfloat*)matri;
	// This calculation would be a lot more complicated for non-unit length quaternions
	// Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
	//   OpenGL
	return matri;
}

void Quaternion::print(){
	printf("X: %f, ", x);
	printf("Y: %f, ", y);
	printf("Z: %f, ", z);
	printf("W: %f\n", w);
}






