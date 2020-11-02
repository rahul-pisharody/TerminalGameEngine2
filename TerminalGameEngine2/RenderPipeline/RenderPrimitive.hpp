/*
GRAPHICS LIBRARY SPECIFIC
*/
#pragma once

#include "../Core/Math/Vec.hpp"

struct Triangle
{
	Vec3D p1, p2, p3;

	short col;
	short sym;
	float lum;
};