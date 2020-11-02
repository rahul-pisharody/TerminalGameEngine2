/*
Vector classes for Math use
*/
#pragma once
#include <math.h>

class Vec2D
{
public:
	float X;
	float Y;
	Vec2D() : X(0), Y(0) {}
	//Vec2D(const Vec2D &v2) : X(v2.X), Y(v2.Y) {}
	//Vec2D(const Vec3D &v3) : X(v3.X), Y(v3.Y) {}
	Vec2D(float x, float y) : X(x), Y(y) {}

	Vec2D operator+ (const Vec2D& v2);
	Vec2D operator- (const Vec2D& v2);
private:

};

class Vec3D
{
public:
	float X;
	float Y;
	float Z;
	Vec3D() : X(0), Y(0), Z(0) {}
	//Vec3D(const Vec3D &v3) : X(v3.X), Y(v3.Y), Z(v3.Z) {}
	Vec3D(const Vec2D &v2) : X(v2.X), Y(v2.Y), Z(0) {}
	Vec3D(float x, float y, float z) : X(x), Y(y), Z(z) {}

	Vec3D operator+ (float a);
	Vec3D operator- (float d);
	Vec3D operator*(float d);
	Vec3D operator/(float d);

	Vec3D operator+ (const Vec3D& v2);
	Vec3D operator- (const Vec3D& v2);
	Vec3D operator* (const Vec3D& v2);
	Vec3D operator/ (const Vec3D& v2);

	float dot(const Vec3D& v2);
	Vec3D cross(const Vec3D& v2);

	Vec3D normalized();
};