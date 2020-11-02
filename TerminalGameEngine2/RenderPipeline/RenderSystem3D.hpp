/*
GRAPHICS LIBRARY SPECIFIC


Huge Thanks to Javidx9 and his 3D Console Game Engine series!
https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h

License
~~~~~~~
One Lone Coder Console Game Engine  Copyright (C) 2018  Javidx9
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions; See license for details.
Original works located at:
https://www.github.com/onelonecoder
https://www.onelonecoder.com
https://www.youtube.com/javidx9
GNU GPLv3
https://github.com/OneLoneCoder/videos/blob/master/LICENSE

*/
#pragma once
#include <deque>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>

#include "Mesh.hpp"
#include "RenderPrimitive.hpp"
#include "../Core/Math/Matrix.hpp"
#include "../Display.hpp"
#include "../Core/Camera.hpp"
#include "../GameComponent/CameraViewComponent.hpp"

/*
Rendering system to project meshes onto the terminal screen
*/
class RenderSystem3D
{
public:
	RenderSystem3D();
	~RenderSystem3D();

	static RenderSystem3D& getInstance();

	int setActiveCamera(Camera * cam_new_active);
	Camera* getActiveCamera();

	int renderMesh(Mesh &mesh, Mat4x4 world_tranform);

	void setDisplay(Display* d);

private:
	int spaceTransform(Triangle &tri_to_trans, Triangle &tri_transformed, Mat4x4 transform);
	int normalCalc(Triangle & tri, Vec3D & norm);
	int applyLighting(Triangle & tri, Vec3D & vec_norm);

	std::vector<Triangle> vertexProcess(Triangle tri, Mat4x4 &world_tranform, Mat4x4 &view_transform);
	int rasterizeTriangle(Triangle tri_to_raster);// Triangle &tri_rasterized);
	
	int clipView(Triangle &tri_to_clip, std::deque<Triangle>& list_triangles);
	int clipScreen(Triangle tri_to_clip, std::deque<Triangle>& list_triangles);
	
	//Returns point of intersection between plane and line
	Vec3D Vector_IntersectPlane(Vec3D &plane_p, Vec3D &plane_n, Vec3D &lineStart, Vec3D &lineEnd);

	//Thanks to javaidx9
	int Triangle_ClipAgainstPlane(Vec3D plane_p, Vec3D plane_n, Triangle &in_tri, Triangle &out_tri1, Triangle &out_tri2);

public:

	Camera* _cam_active;
	Display* display;
};

