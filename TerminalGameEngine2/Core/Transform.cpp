#include "Transform.hpp"

Transform::Transform()
{
	setPosition(0.f, 0.f, 0.f);
	resetRotation();
	setScale(1.f, 1.f, 1.f);
}

Transform::~Transform()
{
}

Vec3D Transform::getPosition()
{
	return Vec3D(_mat_translation.mat[3][0], _mat_translation.mat[3][1], _mat_translation.mat[3][2]);
}
Mat4x4 Transform::getTranslationMatrix()
{
	return _mat_translation;
}
void Transform::setPosition(float x, float y, float z)
{
	_mat_translation = MatTransform::getTranslationMat(x, y, z);
}
void Transform::setPosition(Vec3D new_pos)
{
	setPosition(new_pos.X, new_pos.Y, new_pos.Z);
}


Mat4x4 Transform::getRotationMatrix()
{
	return _mat_rotation;
}

void Transform::rotateX(float theta_rad)
{
	_mat_rotation = MatTransform::getRotationMatX(theta_rad).multMatrix(_mat_rotation);
}
void Transform::rotateY(float theta_rad)
{
	_mat_rotation = MatTransform::getRotationMatY(theta_rad).multMatrix(_mat_rotation);
}
void Transform::rotateZ(float theta_rad)
{
	_mat_rotation = MatTransform::getRotationMatZ(theta_rad).multMatrix(_mat_rotation);
}

void Transform::resetRotation()
{
	_mat_rotation = MatTransform::getRotationMatZ(0.0f).multMatrix(MatTransform::getRotationMatY(0.0f).multMatrix(MatTransform::getRotationMatX(0.0f)));
}

Vec3D Transform::getFacingVector()
{
	return _mat_rotation.multVector(Vec3D(0.f,0.f,1.f));
}
Vec3D Transform::getUpVector()
{
	return _mat_rotation.multVector(Vec3D(0.f, 1.f, 0.f));
}

Vec3D Transform::getScale()
{
	return Vec3D(_mat_scale.mat[0][0], _mat_scale.mat[1][1], _mat_scale.mat[2][2]);
}
Mat4x4 Transform::getScaleMatrix()
{
	return _mat_scale;
}
void Transform::setScale(float x, float y, float z)
{
	//TODO: Needs Implementation
	_mat_scale = MatTransform::getIdentityMat();
}


Mat4x4 Transform::getScaleRotationMatrix()
{
	return _mat_scale.multMatrix(_mat_rotation);
}

Mat4x4 Transform::getTransformMatrix()
{
	return _mat_rotation.multMatrix(_mat_translation);
}

Transform Transform::operator*(Transform & rhs)
{
	Transform res;
	//TODO: Mult member vars
	res._setScaleMat(getScaleMatrix().multMatrix(rhs.getScaleMatrix()));
	Mat4x4 mat_scale_rot = getScaleRotationMatrix();
	res._setRotationMat(mat_scale_rot.multMatrix(rhs.getRotationMatrix()));
	res._setTranslationMat((mat_scale_rot.multMatrix(getTranslationMatrix())).multMatrix(rhs.getTranslationMatrix()));
	
	return res;
}


void Transform::_setScaleMat(Mat4x4 scale)
{
	_mat_scale = scale;
}
void Transform::_setRotationMat(Mat4x4 rot)
{
	_mat_rotation = rot;
}
void Transform::_setTranslationMat(Mat4x4 pos)
{
	_mat_translation = pos;
}