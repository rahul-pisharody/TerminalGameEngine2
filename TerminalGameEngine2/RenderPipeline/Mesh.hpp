#pragma once

#include <vector>
#include <fstream>
#include <sstream>

#include "../Core/Math/Vec.hpp"
#include "RenderPrimitive.hpp"

struct Mesh
{
	std::vector<Triangle> triangles;

	bool loadObjectFromFile(std::string f_name);
};

