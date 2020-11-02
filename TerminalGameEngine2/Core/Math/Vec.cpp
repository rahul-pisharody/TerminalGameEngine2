#include "Vec.hpp"


Vec2D Vec2D::operator+ (const Vec2D& v2)
{
	return { this->X + v2.X, this->Y + v2.Y };
}
Vec2D Vec2D::operator-(const Vec2D &v2)
{
	return { this->X - v2.X, this->Y - v2.Y };
}



Vec3D Vec3D::operator+(float a)
{
	return Vec3D(this->X + a, this->Y + a, this->Z + a);
}
Vec3D Vec3D::operator-(float s)
{
	return Vec3D(this->X - s, this->Y - s, this->Z - s);
}
Vec3D Vec3D::operator*(float m)
{
	return Vec3D(this->X * m, this->Y * m, this->Z * m);
}
Vec3D Vec3D::operator/(float d)
{
	return Vec3D(this->X * d, this->Y * d, this->Z * d);
}

Vec3D Vec3D::operator+ (const Vec3D& v2)
{
	return { this->X + v2.X, this->Y + v2.Y, this->Z + v2.Z };
}
Vec3D Vec3D::operator- (const Vec3D& v2)
{
	return { this->X - v2.X, this->Y - v2.Y, this->Z - v2.Z };
}
Vec3D Vec3D::operator*(const Vec3D & v2)
{
	return { this->X * v2.X, this->Y * v2.Y, this->Z * v2.Z };
}

Vec3D Vec3D::operator/(const Vec3D & v2)
{
	return { this->X / v2.X, this->Y / v2.Y, this->Z / v2.Z };
}

float Vec3D::dot(const Vec3D& v2)
{
	return (this->X * v2.X + this->Y * v2.Y + this->Z * v2.Z);
}
Vec3D Vec3D::cross(const Vec3D& v2)
{
	Vec3D res;
	res.X = this->Y * v2.Z - v2.Y * this->Z;
	res.Y = this->Z * v2.X - v2.Z * this->X;
	res.Z = this->X * v2.Y - v2.X * this->Y;
	return res;
}

Vec3D Vec3D::normalized()
{
	Vec3D norm;
	float l = sqrtf(X * X + Y * Y + Z * Z);
	norm.X = X/l; norm.Y = Y/l; norm.Z = Z/l;
	return norm;
}
