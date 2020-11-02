#pragma once
#include "Math\Vec.hpp"
#include "Math\Matrix.hpp"

/*
Stores Position/Rotation/Scale information
Provides functions to modify them
*/
class Transform 
{
private:
	Mat4x4 _mat_translation;
	Mat4x4 _mat_rotation;
	Mat4x4 _mat_scale;

public:
	Transform();
	~Transform();

	Vec3D getPosition();
	Mat4x4 getTranslationMatrix();
	void setPosition(float x, float y, float z);
	void setPosition(Vec3D new_pos);

	Mat4x4 getRotationMatrix();
	void rotateX(float theta_rad);
	void rotateY(float theta_rad);
	void rotateZ(float theta_rad);
	void resetRotation();

	Vec3D getFacingVector();
	Vec3D getUpVector();

	Vec3D getScale();
	Mat4x4 getScaleMatrix();
	void setScale(float x, float y, float z);

	Mat4x4 getScaleRotationMatrix();
	Mat4x4 getTransformMatrix();

	Transform operator*(Transform& rhs);//Order: LHS then RHS (eg.:rotation then translation)

private:
	void _setScaleMat(Mat4x4 scale);
	void _setRotationMat(Mat4x4 rot);
	void _setTranslationMat(Mat4x4 pos);

};