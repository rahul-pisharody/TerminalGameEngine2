#include "TerminalGameEngine2\MainLoop.hpp"
#include "TerminalGameEngine2\Core\Math\Matrix.hpp"
#include "TerminalGameEngine2\RenderPipeline\Mesh.hpp"
#include "TerminalGameEngine2\GameComponent\MeshRenderer.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <deque>

#include <algorithm>

const float PI = 3.14159f;


class MyGame : public MainLoop
{
private:
	//Returns point of intersection between plane and line
	Vec3D Vector_IntersectPlane(Vec3D &plane_p, Vec3D &plane_n, Vec3D &lineStart, Vec3D &lineEnd)
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

	//Thanks to javaidx9
	int Triangle_ClipAgainstPlane(Vec3D plane_p, Vec3D plane_n, Triangle &in_tri, Triangle &out_tri1, Triangle &out_tri2)
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
			out_tri1.col = in_tri.col;
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
			out_tri1.col = in_tri.col;
			out_tri1.sym = in_tri.sym;

			out_tri2.col = in_tri.col;
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
	}
public:
	MyGame() {}

	void gameInit() override
	{
		//Projection Matrix
		//float near_plane = 0.1f;
		//float far_plane = 1000.0f;
		//float fov = 90.0f;
		//float aspect_ratio = (float)getDisplay()->getDisplayWidth() / (float)getDisplay()->getDisplayHeight();

		//m_mat_proj = MatTransform::getProjectionMat(fov, aspect_ratio, near_plane, far_plane);
		//camera_rotation_mat = MatTransform::getRotationMat(EulerAngle{ 0, 0, 0 });
		//RenderSystem3D::getInstance().setProjectionMatrix(fov, aspect_ratio, near_plane, far_plane);
		//RenderSystem3D::getInstance().setCameraRotationMatrix(camera_rotation_mat);

		theta = 0.0f;
		speed = 1.0f;

		std::unique_ptr<GameObject> new_gob = std::make_unique<GameObject>("Teapot");

		std::unique_ptr<MeshRendererComponent> new_comp = std::make_unique<MeshRendererComponent>();
		std::unique_ptr<Mesh> mesh_tmp = std::make_unique<Mesh>();
		mesh_tmp->loadObjectFromFile("../Resources/teapot.obj");
		new_comp->setMesh(std::move(mesh_tmp));

		new_gob->addGameComponent(std::move(new_comp));
		new_gob->getLocalTransform().setPosition(20.0f, -10.0f, 20.0f);

		//TESTING CAMERA
		/*std::unique_ptr<Camera> gob_camera = std::make_unique<Camera>();
		std::unique_ptr<CameraViewComponent> cam_comp = std::make_unique<CameraViewComponent>(90.f, 256.f / 240.0, 0.1f, 1000.f);
		gob_camera->addGameComponent(std::move(cam_comp));
		dynamic_cast<CameraViewComponent*>(gob_camera->getComponent("CameraView"))->setActive(true);
		gob_camera->getLocalTransform().setPosition(0.0f, 3.0f, -10.0f);
		new_gob->addChild(std::move(gob_camera));*/

		getSceneTree()->addChild(std::move(new_gob));


		std::unique_ptr<GameObject> new_gob2 = std::make_unique<GameObject>("Teapot2");
		std::unique_ptr<MeshRendererComponent> new_comp2 = std::make_unique<MeshRendererComponent>();
		std::unique_ptr<Mesh> mesh_tmp2 = std::make_unique<Mesh>();
		mesh_tmp2->loadObjectFromFile("../Resources/teapot.obj");
		new_comp2->setMesh(std::move(mesh_tmp2));
		new_gob2->addGameComponent(std::move(new_comp2));
		new_gob2->getLocalTransform().setPosition(20.0f, -10.0f, 30.0f);
		getSceneTree()->addChild(std::move(new_gob2));


	}

	void update(int delta) override
	{
		Vec3D cam_pos = getActiveCamera()->getLocalTransform().getPosition();
		Vec3D cam_forward = getActiveCamera()->getLocalTransform().getFacingVector() * (speed * 0.01f * delta);
		Vec3D cam_up = getActiveCamera()->getLocalTransform().getUpVector() * (speed * 0.005f * delta);
		Vec3D cam_right = getActiveCamera()->getLocalTransform().getFacingVector().cross(getActiveCamera()->getLocalTransform().getUpVector()) * (speed * 0.01 * delta);

		EulerAngle rotation = { 0,0,0 };

		//cam_pos = cam_pos + cam_forward;

		if (InputManager::getInstance().isKeyPressed(Inputs::KEY_UP))
		{
			cam_pos = cam_pos + cam_up;
		}
		else if (InputManager::getInstance().isKeyPressed(Inputs::KEY_DOWN))
		{
			cam_pos = cam_pos - cam_up;
		}
		if (InputManager::getInstance().isKeyPressed(Inputs::KEY_LEFT))
		{
			cam_pos = cam_pos + cam_right;
		}
		else if (InputManager::getInstance().isKeyPressed(Inputs::KEY_RIGHT))
		{
			cam_pos = cam_pos - cam_right;
		}

		if (InputManager::getInstance().isKeyPressed('W'))
		{
			rotation.pitch = delta*0.001f;
		}
		else if (InputManager::getInstance().isKeyPressed('S'))
		{
			rotation.pitch = -delta*0.001f;
		}
		if (InputManager::getInstance().isKeyPressed('A'))
		{
			rotation.yaw = delta*0.001f;
		}
		else if(InputManager::getInstance().isKeyPressed('D'))
		{
			rotation.yaw = -delta*0.001f;
		}
		if (InputManager::getInstance().isKeyPressed('Q')) 
		{
			rotation.roll = delta*0.001f;
		}
		else if (InputManager::getInstance().isKeyPressed('E'))
		{
			rotation.roll = -delta*0.001f;
		}
		if (InputManager::getInstance().isKeyPressed('Z'))
		{
			cam_pos = cam_pos + cam_forward;
		}
		else if (InputManager::getInstance().isKeyPressed('X'))
		{
			cam_pos = cam_pos - cam_forward;
		}

		getActiveCamera()->getLocalTransform().rotateZ(rotation.roll);
		getActiveCamera()->getLocalTransform().rotateY(rotation.yaw);
		getActiveCamera()->getLocalTransform().rotateX(rotation.pitch);
		getActiveCamera()->getLocalTransform().setPosition(cam_pos);

	}
private:
	float speed;
	float theta;
};

int main()
{
	std::unique_ptr<MyGame> p_game(new MyGame());
	p_game->initTerminal(256, 240, 2);
	p_game->initCamera(90.f, 256.f / 240.0, 0.1f, 1000.f);
	//p_game->gameInit();
	p_game->run();
	return 0;
}
