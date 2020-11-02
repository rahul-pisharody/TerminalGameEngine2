#include "RenderSystem3D.hpp"
#include "../Terminal.hpp"

RenderSystem3D::RenderSystem3D()
{
}


RenderSystem3D::~RenderSystem3D()
{
}

RenderSystem3D & RenderSystem3D::getInstance()
{
	static RenderSystem3D *instance = new RenderSystem3D();
	return *instance;
}

int RenderSystem3D::setActiveCamera(Camera * cam_new_active)
{
	if (cam_new_active == nullptr)
	{
		_cam_active = cam_new_active;
		return 0;
	}
	if (cam_new_active->getComponent("CameraView") == nullptr)
	{
		return 1;//ERROR No CameraViewComponent attached
	}
	if (_cam_active != nullptr)
	{
		CameraViewComponent* tmp = dynamic_cast<CameraViewComponent*>(_cam_active->getComponent("CameraView"));
		tmp->setActive(false);
		//dynamic_cast<CameraViewComponent*>(_cam_active->getComponent("CameraView"))->setActive(false);
	}
	_cam_active = cam_new_active;
	return 0;
}

Camera * RenderSystem3D::getActiveCamera()
{
	return _cam_active;
}


int RenderSystem3D::renderMesh(Mesh &mesh, Mat4x4 world_transform)
{
	if (getActiveCamera() == nullptr)
	{
		return 1;//TODO: ERROR NO CAMERA
	}
	Mat4x4 view_transform = dynamic_cast<CameraViewComponent*>(_cam_active->getComponent("CameraView"))->getViewMatrix();


	std::vector<Triangle> triangles_to_raster;

	for (auto tri : mesh.triangles)
	{
		std::vector<Triangle> vec_tri_processed = vertexProcess(tri, world_transform, view_transform);
		for (auto &tri_proj_space : vec_tri_processed)
		{
			triangles_to_raster.push_back(tri_proj_space);
		}
	}


	std::sort(triangles_to_raster.begin(), triangles_to_raster.end(), [](Triangle t1, Triangle t2)
	{
		float z1 = (t1.p1.Z + t1.p2.Z + t1.p3.Z) / 3.0f;
		float z2 = (t2.p1.Z + t2.p2.Z + t2.p3.Z) / 3.0f;

		return z1 > z2;
	});


	for (auto &tri : triangles_to_raster)
	{
		std::deque<Triangle> list_triangles;
		clipScreen(tri, list_triangles);
		for (auto &tri_to_raster : list_triangles)
		{
			rasterizeTriangle(tri_to_raster);
		}
	}
	return 0;
}

int RenderSystem3D::spaceTransform(Triangle &tri_to_trans, Triangle &tri_transformed, Mat4x4 transform)
{
	tri_transformed.p1 = transform.multVector(tri_to_trans.p1);
	tri_transformed.p2 = transform.multVector(tri_to_trans.p2);
	tri_transformed.p3 = transform.multVector(tri_to_trans.p3);
	tri_transformed.sym = tri_to_trans.sym;
	tri_transformed.col = tri_to_trans.col;
	tri_transformed.lum = tri_to_trans.lum;
	return 0;
}

int RenderSystem3D::normalCalc(Triangle &tri, Vec3D &norm)
{
	Vec3D line_A, line_B;
	line_A = tri.p2 - tri.p1;
	line_B = tri.p3 - tri.p1;
	norm = line_A.cross(line_B);

	norm = norm.normalized();
	return 0;
}

int RenderSystem3D::applyLighting(Triangle &tri, Vec3D &vec_norm)
{
	Vec3D light_dir = { 0.0f, -0.25f, 1.0f };
	light_dir = light_dir.normalized(); // Normalizing
	float dp_light_norm = min(-0.1f, vec_norm.dot(light_dir));
	CHAR_INFO c_info = display->getColourGrey(-1.0f*dp_light_norm);
	tri.lum = -1.0f*dp_light_norm;
	tri.col = c_info.Attributes;
	tri.sym = c_info.Char.UnicodeChar;
	return 0;
}


int RenderSystem3D::clipView(Triangle &tri_to_clip, std::deque<Triangle> &list_triangles)
{
	int n_clipped_triangles = 0;
	Triangle clipped[2];
	n_clipped_triangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, tri_to_clip, clipped[0], clipped[1]);
	for (int i = 0; i < n_clipped_triangles; i++)
	{
		list_triangles.push_back(clipped[i]);
	}
	return n_clipped_triangles;
}

// Clip triangles against all four screen edges 
// This could yield a bunch of triangles, so create a queue that we traverse to ensure we only test new triangles generated against planes
int RenderSystem3D::clipScreen(Triangle tri_to_clip, std::deque<Triangle> &list_triangles)
{
	Triangle clipped[2];

	// Add initial triangle
	list_triangles.push_back(tri_to_clip);
	int nNewTriangles = 1;

	for (int p = 0; p < 4; p++)
	{
		int nTrisToAdd = 0;
		while (nNewTriangles > 0)
		{
			// Take triangle from front of queue
			Triangle test = list_triangles.front();
			list_triangles.pop_front();
			nNewTriangles--;

			// Clip it against a plane. We only need to test each 
			// subsequent plane, against subsequent new triangles
			// as all triangles after a plane clip are guaranteed
			// to lie on the inside of the plane. I like how this
			// comment is almost completely and utterly justified
			switch (p)
			{
			case 0:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
			case 1:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, (float)display->getDisplayHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
			case 2:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
			case 3:	nTrisToAdd = Triangle_ClipAgainstPlane({ (float)display->getDisplayWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
			}

			// Clipping may yield a variable number of triangles, so
			// add these new ones to the back of the queue for subsequent
			// clipping against next planes
			for (int w = 0; w < nTrisToAdd; w++)
				list_triangles.push_back(clipped[w]);
		}
		nNewTriangles = list_triangles.size();
	}
	return 0;
}


std::vector<Triangle> RenderSystem3D::vertexProcess(Triangle tri, Mat4x4 &world_tranform, Mat4x4 &view_transform)
{
	Triangle tri_world_space, tri_view_space, tri_proj_space;
	spaceTransform(tri, tri_world_space, world_tranform);
	Vec3D vec_normal;
	normalCalc(tri_world_space, vec_normal);

	Vec3D vec_camera_ray = tri_world_space.p1 - getActiveCamera()->getGlobalTransform().getPosition();

	std::vector<Triangle> triangles_to_raster;
	if (vec_normal.dot(vec_camera_ray) < 0.0f)
	{
		applyLighting(tri_world_space, vec_normal);
		spaceTransform(tri_world_space, tri_view_space, view_transform);
		std::deque<Triangle> clipped_list;
		int n_tris = clipView(tri_view_space, clipped_list);

		for (auto &tri_clipped : clipped_list)
		{
			spaceTransform(tri_clipped, tri_proj_space, dynamic_cast<CameraViewComponent*>(_cam_active->getComponent("CameraView"))->getProjectionMatrix());

			// X/Y are inverted so put them back
			/*tri_proj_space.p1.X *= -1.0f;
			tri_proj_space.p2.X *= -1.0f;
			tri_proj_space.p3.X *= -1.0f;*/
			tri_proj_space.p1.Y *= -1.0f;
			tri_proj_space.p2.Y *= -1.0f;
			tri_proj_space.p3.Y *= -1.0f;


			//Scale to view coord
			tri_proj_space.p1 = tri_proj_space.p1 + Vec3D{ 1.0f, 1.0f, 0.0f };
			tri_proj_space.p2 = tri_proj_space.p2 + Vec3D{ 1.0f, 1.0f, 0.0f };
			tri_proj_space.p3 = tri_proj_space.p3 + Vec3D{ 1.0f, 1.0f, 0.0f };
			tri_proj_space.p1 = tri_proj_space.p1 * Vec3D{ 0.5f * (float)display->getDisplayWidth(), 0.5f * (float)display->getDisplayHeight() , 1.0f };
			tri_proj_space.p2 = tri_proj_space.p2 * Vec3D{ 0.5f * (float)display->getDisplayWidth(), 0.5f * (float)display->getDisplayHeight() , 1.0f };
			tri_proj_space.p3 = tri_proj_space.p3 * Vec3D{ 0.5f * (float)display->getDisplayWidth(), 0.5f * (float)display->getDisplayHeight() , 1.0f };

			triangles_to_raster.push_back(tri_proj_space);
		}
	}
	return triangles_to_raster;
}

//Also Draw the triangle
int RenderSystem3D::rasterizeTriangle(Triangle tri_to_raster)//, Triangle & tri_rasterized)
{
	//getTerminal()->drawFilledTriangle({ tri.p1.X, tri.p1.Y }, { tri.p2.X, tri.p2.Y }, { tri.p3.X, tri.p3.Y }, tri.col, tri.sym);
	//display->drawTriangleBarycentric({ tri_to_raster.p1.X, tri_to_raster.p1.Y }, { tri_to_raster.p2.X, tri_to_raster.p2.Y }, { tri_to_raster.p3.X, tri_to_raster.p3.Y }, tri_to_raster.col, tri_to_raster.sym);
	display->drawTriangleBresenham({ tri_to_raster.p1.X, tri_to_raster.p1.Y }, { tri_to_raster.p2.X, tri_to_raster.p2.Y }, { tri_to_raster.p3.X, tri_to_raster.p3.Y }, tri_to_raster.col, tri_to_raster.sym);
	//getTerminal()->drawTriangleWireframe({ tri.p1.X, tri.p1.Y }, { tri.p2.X, tri.p2.Y }, { tri.p3.X, tri.p3.Y }, FG_BLACK); //Wireframe
	return 0;
}

void RenderSystem3D::setDisplay(Display* d)
{
	display = d;
}


Vec3D RenderSystem3D::Vector_IntersectPlane(Vec3D & plane_p, Vec3D & plane_n, Vec3D & lineStart, Vec3D & lineEnd)
{
	plane_n = plane_n.normalized();
	float plane_d = -plane_n.dot(plane_p);
	float ad = lineStart.dot(plane_n);
	float bd = lineEnd.dot(plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	Vec3D lineStartToEnd = lineEnd - lineStart;
	Vec3D lineToIntersect = lineStartToEnd * t;
	return (lineStart + lineToIntersect);
}

int RenderSystem3D::Triangle_ClipAgainstPlane(Vec3D plane_p, Vec3D plane_n, Triangle & in_tri, Triangle & out_tri1, Triangle & out_tri2)
{
	plane_n = plane_n.normalized();
	auto dist = [&](Vec3D &p)
	{
		Vec3D n = p.normalized();
		return (plane_n.X * p.X + plane_n.Y * p.Y + plane_n.Z * p.Z - plane_n.dot(plane_p));
	};
	Vec3D* inside_points[3];  int nInsidePointCount = 0;
	Vec3D* outside_points[3]; int nOutsidePointCount = 0;

	float d1 = dist(in_tri.p1);
	float d2 = dist(in_tri.p2);
	float d3 = dist(in_tri.p3);

	if (d1 >= 0) inside_points[nInsidePointCount++] = &in_tri.p1;
	else outside_points[nOutsidePointCount++] = &in_tri.p1;
	if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p2; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p2; }
	if (d3 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p3; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p3; }

	if (nInsidePointCount == 0)
	{
		// All points lie on the outside of plane, so clip whole triangle
		// It ceases to exist

		return 0; // No returned triangles are valid
	}

	if (nInsidePointCount == 3)
	{
		// All points lie on the inside of plane, so do nothing
		// and allow the triangle to simply pass through
		out_tri1 = in_tri;

		return 1; // Just the one returned original triangle is valid
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		// Triangle should be clipped. As two points lie outside
		// the plane, the triangle simply becomes a smaller triangle

		// Copy appearance info to new triangle
		out_tri1.col = FG_BLUE;//in_tri.col;
		out_tri1.sym = in_tri.sym;

		// The inside point is valid, so keep that...
		out_tri1.p1 = *inside_points[0];

		// but the two new points are at the locations where the 
		// original sides of the triangle (lines) intersect with the plane
		out_tri1.p2 = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
		out_tri1.p3 = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

		return 1; // Return the newly formed single triangle
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		// Triangle should be clipped. As two points lie inside the plane,
		// the clipped triangle becomes a "quad". Fortunately, we can
		// represent a quad with two new triangles

		// Copy appearance info to new triangles
		out_tri1.col = FG_RED;//in_tri.col;
		out_tri1.sym = in_tri.sym;

		out_tri2.col = FG_GREEN;//in_tri.col;
		out_tri2.sym = in_tri.sym;

		// The first triangle consists of the two inside points and a new
		// point determined by the location where one side of the triangle
		// intersects with the plane
		out_tri1.p1 = *inside_points[0];
		out_tri1.p2 = *inside_points[1];
		out_tri1.p3 = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

		// The second triangle is composed of one of he inside points, a
		// new point determined by the intersection of the other side of the 
		// triangle and the plane, and the newly created point above
		out_tri2.p1 = *inside_points[1];
		out_tri2.p2 = out_tri1.p3;
		out_tri2.p3 = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

		return 2; // Return two newly formed triangles which form a quad
	}
	return 0;
}
