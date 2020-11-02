#include "Mesh.hpp"


bool Mesh::loadObjectFromFile(std::string f_name)
{
	std::ifstream f;
	f.open(f_name);
	if (f.fail())
	{
		return false;
	}

	std::vector<Vec3D> verts;

	std::string line;
	while (std::getline(f, line))
	{
		std::stringstream s_stream;
		s_stream << line;

		char junk;
		Vec3D vert;
		switch (line[0])
		{
		case 'v':
			s_stream >> junk >> vert.X >> vert.Y >> vert.Z;
			verts.push_back(vert);
			break;
		case 'f':
			int face[3];
			s_stream >> junk >> face[0] >> face[1] >> face[2];
			triangles.push_back(Triangle{ verts[face[0] - 1], verts[face[1] - 1], verts[face[2] - 1] });
		}
	}
	return true;
}