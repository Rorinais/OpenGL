#pragma once
#include"../math3d/math3d.h"

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	void Scale(float x, float y, float z)
	{
		m_scale.x = x;
		m_scale.y = y;
		m_scale.z = z;
	}
	void Rotate(float x, float y, float z)
	{
		m_rotation.x = x;
		m_rotation.y = y;
		m_rotation.z = z;
	}
	void worldPos(float x, float y, float z)
	{
		m_worldPos.x = x;
		m_worldPos.y = y;
		m_worldPos.z = z;
	}

	void SetPerspectivePro(float FOV, float Width, float Height, float zNear, float zFar) 
	{
		m_persProj.FOV = FOV;
		m_persProj.Height = Height;
		m_persProj.Width = Width;
		m_persProj.zFar = zFar;
		m_persProj.zNear = zNear;
	}

	void SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
	{
		m_camera.Pos = Pos;
		m_camera.Target = Target;
		m_camera.Up = Up;

	}

	const Matrix4f* GetTrans();
	const Matrix4f* GetWorldTrans();

private:

	void initScaleTrans(Matrix4f& mat);
	void initRotateTrans(Matrix4f& mat);
	void initTranslationTrans(Matrix4f& mat);
	void initPersPectiveProjTrans(Matrix4f& mat);
	void initCameraTransform(Matrix4f& trans, Matrix4f& rot);

	Vector3f m_scale;
	Vector3f m_rotation;
	Vector3f m_worldPos;

	struct {
		float FOV;
		float Width;
		float Height;
		float zNear;
		float zFar;
	}m_persProj;

	struct 
	{
		Vector3f Pos;
		Vector3f Target;
		Vector3f Up;
	}m_camera;

	Matrix4f m_transformation;

};
