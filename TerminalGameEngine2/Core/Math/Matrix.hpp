/*
Matrices and their related functions

Some of the code here is from Javidx9's Console Game Engine. (Though, it is pretty straighforward)
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

#include "Angle.hpp"
#include "Vec.hpp"

struct Mat4x4
{
	float mat[4][4] = { 0.0f };

	Mat4x4 multMatrix(const Mat4x4 &mat_B);
	Vec3D multVector(const Vec3D &vec_inp, bool div_4th_col = true);
};

/*
TODO: Turn into namespace (or something)
*/
struct MatTransform : public Mat4x4
{
	MatTransform() { }
	MatTransform(Mat4x4 m) { }
	static Mat4x4 getRotationMatX(float theta_rad);
	static Mat4x4 getRotationMatY(float rad);
	static Mat4x4 getRotationMatZ(float theta_rad);
		
	static Mat4x4 getRotationMat(float pitch, float roll, float yaw);

	static Mat4x4 getRotationMat(EulerAngle rad);
	//static EulerAngle getEulerAngle(Mat4x4 rotation_mat);
		//
	static Mat4x4 getIdentityMat();
		//
	static Mat4x4 getTranslationMat(float x, float y, float z);
		//
	static Mat4x4 getProjectionMat(float fov_degrees, float aspect_ratio, float near_plane, float far_plane);
		//
	static Mat4x4 getPointAtMatrix(Vec3D &pos, Vec3D &target, Vec3D &up);
		//
	static Mat4x4  getQuickInverseMatrix(const Mat4x4 &m); // Only for Rotation/Translation Matrices
};