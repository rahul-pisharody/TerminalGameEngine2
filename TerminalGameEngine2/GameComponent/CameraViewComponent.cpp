#include "CameraViewComponent.hpp"

CameraViewComponent::CameraViewComponent(float fov_degrees, float aspect_ratio, float near_plane, float far_plane) : GameComponent("CameraView")
{
	if ((fov_degrees <= 0) || (aspect_ratio <= 1e-6) || (far_plane < near_plane))
	{
		throw CameraComponentCreationException();
	}
	_fov = fov_degrees;
	_aspect_ratio = aspect_ratio;
	_near_plane = near_plane;
	_far_plane = far_plane;

	_mat_projection = MatTransform::getProjectionMat(fov_degrees, aspect_ratio, near_plane, far_plane);
}

CameraViewComponent::~CameraViewComponent()
{
	setActive(false);
}

void CameraViewComponent::update(int delta)
{
	_updateViewMatrix();
}

Mat4x4 CameraViewComponent::getViewMatrix()
{
	return _mat_view;
}

Mat4x4 CameraViewComponent::getProjectionMatrix()
{
	return _mat_projection;
}

bool CameraViewComponent::isActive()
{
	return _active;
}

void CameraViewComponent::setActive(bool act)
{
	if (_active == act) return;
	if (!_active && act)
	{
		if (getParentGameObject() == nullptr)//Component not attached to GameObject
		{
			return;
		}
		RenderSystem3D::getInstance().setActiveCamera(static_cast<Camera*>(getParentGameObject()));
	}
	else //if (_active && !act)
	{
		RenderSystem3D::getInstance().setActiveCamera(nullptr);
	}
	_active = act;
}

void CameraViewComponent::trackObject(GameObject * g_obj)
{
	//TODO: Needs Implementation
}

void CameraViewComponent::_updateViewMatrix()
{
	if (!getParentGameObject())//Not Attached to GameObject
	{
		return;
	}
	Transform trf_global = getParentGameObject()->getGlobalTransform();
	Vec3D camera_look_dir = trf_global.getFacingVector();
	Vec3D camera_up_dir = trf_global.getUpVector();

	//Camera-view matrix

	Vec3D target = trf_global.getPosition() + camera_look_dir;
	Mat4x4 mat_camera = MatTransform::getPointAtMatrix(trf_global.getPosition(), target, camera_up_dir);

	_mat_view = MatTransform::getQuickInverseMatrix(mat_camera);
}
