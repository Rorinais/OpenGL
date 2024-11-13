#pragma once
#include <math.h>
#include <iostream>
#include <assimp/matrix4x4.h>
using namespace std;

#define M_PI 3.14159265
#define ToRadian(x)(float)(((x)*M_PI/180.0f))
#define ToDegree(x)(float)(((x)*180.0f/M_PI))
class Vector3f
{
public:
	float x, y, z;
	Vector3f() {};
	Vector3f(float _x, float _y, float _z){
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f Cross(const Vector3f& v)const
	{
		const float _x = y * v.z - z * v.y;
		const float _y = z * v.x - x * v.z;
		const float _z = x * v.y - y * v.x;

		return Vector3f(_x, _y, _z);
	}
	Vector3f& Normalize()
	{
		const float Length = sqrtf(x * x + y * y + z * z);
		x /= Length;
		y /= Length;
		z /= Length;

		return *this;
	}

	void Rotate(float Angle, const Vector3f& Axe);

	Vector3f& operator+=(const Vector3f& r)
	{
		x += r.x;
		y += r.y;
		z += r.z;
		return *this;
	}

	Vector3f& operator-=(const Vector3f& r)
	{
		x -= r.x;
		y -= r.y;
		z -= r.z;
		return *this;
	}

	Vector3f& operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	Vector3f operator-(const Vector3f& v) const
	{
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

};
inline Vector3f operator*(const Vector3f& l, float f)
{
	Vector3f Ret(l.x * f, l.y * f, l.z * f);
	return Ret;
}
class Vector2f
{
public:
	float x, y;
	Vector2f() {};
	Vector2f(float _x, float _y) {
		x = _x;
		y = _y;
	}
};

class Matrix4f
{
public:
	float mat[4][4];

	inline Matrix4f operator*(const Matrix4f& Right)
	{
		Matrix4f Ret;
		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				Ret.mat[i][j] =	mat[i][0] * Right.mat[0][j] + mat[i][1] * Right.mat[1][j] +
								mat[i][2] * Right.mat[2][j] + mat[i][3] * Right.mat[3][j];
			}
		}
		return Ret;
	}
	void TransMatrix4f()
	{
		float temp;
		for (int i = 0; i < 4; i++)
		{
			for (int j = i + 1; j < 4; j++)
			{
				temp = mat[i][j];
				mat[i][j] = mat[j][i];
				mat[j][i] = temp;
			}
		}
	}

	void fromAiMatrix4x4(const aiMatrix4x4& aiMat)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				mat[i][j] = aiMat[i][j];
			}
		}
	}

};

class Quaternion
{
public:
	float x, y, z, w;

	Quaternion(float _x, float _y, float _z, float _w);
	void Normalize();
	Quaternion Conjugate();
	Quaternion operator*(const Quaternion& q) const;
};
