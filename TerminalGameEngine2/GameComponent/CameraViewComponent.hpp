#pragma once
#include <exception>

#include "../RenderPipeline/RenderSystem3D.hpp"
#include "../Core/GameObject.hpp"
#include "GameComponent.hpp"

struct CameraComponentCreationException : std::exception
{
	const char* what() const noexcept { return "Camera Component Creation Error"; }
};

/*
Component that provides camera functionality to the parent object
*/
class CameraViewComponent : public GameComponent
{
public:
	CameraViewComponent(float fov_degrees, float aspect_ratio, float near_plane, float far_plane);
	~CameraViewComponent();

	void update(int delta);
	/*void render(Mat4x4 mat_transform);*/

	Mat4x4 getViewMatrix();
	Mat4x4 getProjectionMatrix();

	bool isActive();
	void setActive(bool act);

	void trackObject(GameObject *g_obj);

private:
	void _updateViewMatrix();

private:
	Mat4x4 _mat_view;
	Mat4x4 _mat_projection;
	bool _active;

	float _fov;
	float _aspect_ratio;
	float _near_plane;
	float _far_plane;

};