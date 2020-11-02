#include "Matrix.hpp"

Mat4x4 Mat4x4::multMatrix(const Mat4x4 &mat_B)
{
	Mat4x4 res;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			res.mat[r][c] = mat[r][0] * mat_B.mat[0][c] + mat[r][1] * mat_B.mat[1][c] + mat[r][2] * mat_B.mat[2][c] + mat[r][3] * mat_B.mat[3][c];
	return res;
}

Vec3D Mat4x4::multVector(const Vec3D &vec_inp, bool div_4th_col)
{
	Vec3D vec_out;
	vec_out.X = vec_inp.X * mat[0][0] + vec_inp.Y * mat[1][0] + vec_inp.Z * mat[2][0] + mat[3][0];
	vec_out.Y = vec_inp.X * mat[0][1] + vec_inp.Y * mat[1][1] + vec_inp.Z * mat[2][1] + mat[3][1];
	vec_out.Z = vec_inp.X * mat[0][2] + vec_inp.Y * mat[1][2] + vec_inp.Z * mat[2][2] + mat[3][2];
	float w = vec_inp.X * mat[0][3] + vec_inp.Y * mat[1][3] + vec_inp.Z * mat[2][3] + mat[3][3];

	if (div_4th_col && w != 0.0f)
	{
		vec_out.X /= w;
		vec_out.Y /= w;
		vec_out.Z /= w;
	}
	return vec_out;
}




Mat4x4 MatTransform::getRotationMatX(float theta_rad)
{
	Mat4x4 mat_rotX;

	mat_rotX.mat[0][0] = 1;
	mat_rotX.mat[1][1] = cosf(theta_rad);
	mat_rotX.mat[1][2] = sinf(theta_rad);
	mat_rotX.mat[2][1] = -sinf(theta_rad);
	mat_rotX.mat[2][2] = cosf(theta_rad);
	mat_rotX.mat[3][3] = 1;

	return mat_rotX;
}
Mat4x4 MatTransform::getRotationMatY(float rad)
{
	Mat4x4 matrix;
	matrix.mat[0][0] = cosf(rad);
	matrix.mat[0][2] = sinf(rad);
	matrix.mat[2][0] = -sinf(rad);
	matrix.mat[1][1] = 1.0f;
	matrix.mat[2][2] = cosf(rad);
	matrix.mat[3][3] = 1.0f;
	return matrix;
}
Mat4x4 MatTransform::getRotationMatZ(float theta_rad)
{
	Mat4x4 mat_rotZ;
	mat_rotZ.mat[0][0] = cosf(theta_rad);
	mat_rotZ.mat[0][1] = sinf(theta_rad);
	mat_rotZ.mat[1][0] = -sinf(theta_rad);
	mat_rotZ.mat[1][1] = cosf(theta_rad);
	mat_rotZ.mat[2][2] = 1;
	mat_rotZ.mat[3][3] = 1;
	return mat_rotZ;
}

Mat4x4 MatTransform::getRotationMat(float pitch, float roll, float yaw)
{
	Mat4x4 mat_rot;
	mat_rot.mat[0][0] = cosf(roll)*cosf(yaw);
	mat_rot.mat[0][1] = cosf(roll)*sinf(yaw)*sinf(pitch) - sinf(roll)*cosf(pitch);
	mat_rot.mat[0][2] = cosf(roll)*sinf(yaw)*cosf(pitch) + sinf(roll)*sinf(pitch);

	mat_rot.mat[1][0] = sinf(roll)*cosf(yaw);
	mat_rot.mat[1][1] = sinf(roll)*sinf(yaw)*sinf(pitch) + cosf(roll)*cosf(yaw);
	mat_rot.mat[1][2] = sinf(roll)*sinf(yaw)*cosf(pitch) - cosf(roll)*sinf(pitch);

	mat_rot.mat[2][0] = -sinf(yaw);
	mat_rot.mat[2][1] = cosf(yaw)*sinf(pitch);
	mat_rot.mat[2][2] = cosf(yaw)*cosf(pitch);

	return mat_rot;
}
Mat4x4 MatTransform::getRotationMat(EulerAngle euler)
{
	return getRotationMat(euler.pitch, euler.roll, euler.yaw);
}

Mat4x4 MatTransform::getIdentityMat()
{
	Mat4x4 matrix;
	matrix.mat[0][0] = 1.0f;
	matrix.mat[1][1] = 1.0f;
	matrix.mat[2][2] = 1.0f;
	matrix.mat[3][3] = 1.0f;
	return matrix;
}

Mat4x4 MatTransform::getTranslationMat(float x, float y, float z)
{
	Mat4x4 matrix;
	matrix.mat[0][0] = 1.0f;
							matrix.mat[1][1] = 1.0f;
													matrix.mat[2][2] = 1.0f;
																			matrix.mat[3][3] = 1.0f;
	matrix.mat[3][0] = x;	matrix.mat[3][1] = y;	matrix.mat[3][2] = z;
	return matrix;
}

Mat4x4 MatTransform::getProjectionMat(float fov_degrees, float aspect_ratio, float near_plane, float far_plane)
{
	float fov_coeff = 1.0f / tanf(fov_degrees * 0.5f / 180.0f * 3.14159f);
	Mat4x4 matrix;
	matrix.mat[0][0] = aspect_ratio * fov_coeff;
	matrix.mat[1][1] = fov_coeff;
	matrix.mat[2][2] = far_plane / (far_plane - near_plane);
	matrix.mat[3][2] = (-far_plane * near_plane) / (far_plane - near_plane);
	matrix.mat[2][3] = 1.0f;
	matrix.mat[3][3] = 0.0f;
	return matrix;
}

Mat4x4 MatTransform::getPointAtMatrix(Vec3D & pos, Vec3D & target, Vec3D & up)

{
	// New forward direction
	Vec3D forward_new = target - pos;
	forward_new = forward_new.normalized();

	// New Up direction
	Vec3D a = forward_new * up.dot(forward_new);
	Vec3D up_new = up - a;
	up_new = up_new.normalized();

	//New Right direction
	Vec3D right_new = up_new.cross(forward_new);
	right_new = right_new.normalized();

	//Dimensioning and Translation Matrix	
	Mat4x4 matrix;
	matrix.mat[0][0] = right_new.X;		matrix.mat[0][1] = right_new.Y;		matrix.mat[0][2] = right_new.Z;		matrix.mat[0][3] = 0.0f;
	matrix.mat[1][0] = up_new.X;			matrix.mat[1][1] = up_new.Y;			matrix.mat[1][2] = up_new.Z;			matrix.mat[1][3] = 0.0f;
	matrix.mat[2][0] = forward_new.X;	matrix.mat[2][1] = forward_new.Y;	matrix.mat[2][2] = forward_new.Z;	matrix.mat[2][3] = 0.0f;
	matrix.mat[3][0] = pos.X;			matrix.mat[3][1] = pos.Y;			matrix.mat[3][2] = pos.Z;			matrix.mat[3][3] = 1.0f;
	return matrix;
}

Mat4x4 MatTransform::getQuickInverseMatrix(const Mat4x4 & m)
{
	Mat4x4 matrix;
	matrix.mat[0][0] = m.mat[0][0]; matrix.mat[0][1] = m.mat[1][0]; matrix.mat[0][2] = m.mat[2][0]; matrix.mat[0][3] = 0.0f;
	matrix.mat[1][0] = m.mat[0][1]; matrix.mat[1][1] = m.mat[1][1]; matrix.mat[1][2] = m.mat[2][1]; matrix.mat[1][3] = 0.0f;
	matrix.mat[2][0] = m.mat[0][2]; matrix.mat[2][1] = m.mat[1][2]; matrix.mat[2][2] = m.mat[2][2]; matrix.mat[2][3] = 0.0f;
	matrix.mat[3][0] = -(m.mat[3][0] * matrix.mat[0][0] + m.mat[3][1] * matrix.mat[1][0] + m.mat[3][2] * matrix.mat[2][0]);
	matrix.mat[3][1] = -(m.mat[3][0] * matrix.mat[0][1] + m.mat[3][1] * matrix.mat[1][1] + m.mat[3][2] * matrix.mat[2][1]);
	matrix.mat[3][2] = -(m.mat[3][0] * matrix.mat[0][2] + m.mat[3][1] * matrix.mat[1][2] + m.mat[3][2] * matrix.mat[2][2]);
	matrix.mat[3][3] = 1.0f;
	return matrix;
}
