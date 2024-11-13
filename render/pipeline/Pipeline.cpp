#include"Pipeline.h"
#include <iostream>
#include "../math3d/math3d.h"

Pipeline::Pipeline()
{
	m_scale = Vector3f(1.0f, 1.0f, 1.0f);
	m_rotation= Vector3f(0.0f, 0.0f, 0.0f);
	m_worldPos= Vector3f(0.0f, 0.0f, 0.0f);
	m_transformation = Matrix4f();
}
Pipeline::~Pipeline()
{

}

void Pipeline::initScaleTrans(Matrix4f& mat)
{
	mat.mat[0][0] = m_scale.x;		mat.mat[0][1] =0.0f;			mat.mat[0][2] = 0.0f;			mat.mat[0][3] = 0.0f;
	mat.mat[1][0] = 0.0f;			mat.mat[1][1] = m_scale.y;		mat.mat[1][2] = 0.0f;			mat.mat[1][3] = 0.0f;
	mat.mat[2][0] = 0.0f;			mat.mat[2][1] = 0.0f;			mat.mat[2][2] = m_scale.z;		mat.mat[2][3] = 0.0f;
	mat.mat[3][0] = 0.0f;			mat.mat[3][1] = 0.0f;			mat.mat[3][2] = 0.0f;			mat.mat[3][3] = 1.0f;
}

void Pipeline::initRotateTrans(Matrix4f& mat)
{
	Matrix4f rx, ry, rz;
	const float x = ToRadian(m_rotation.x);
	const float y = ToRadian(m_rotation.y);
	const float z = ToRadian(m_rotation.z);
//x
	rx.mat[0][0] = 1.0f;		rx.mat[0][1] = 0.0f;		rx.mat[0][2] = 0.0f;		rx.mat[0][3] = 0.0f;
	rx.mat[1][0] = 0.0f;		rx.mat[1][1] = cosf(x);		rx.mat[1][2] = sinf(x);		rx.mat[1][3] = 0.0f;
	rx.mat[2][0] = 0.0f;		rx.mat[2][1] = -sinf(x);	rx.mat[2][2] = cosf(x);		rx.mat[2][3] = 0.0f;
	rx.mat[3][0] = 0.0f;		rx.mat[3][1] = 0.0f;		rx.mat[3][2] = 0.0f;		rx.mat[3][3] = 1.0f;
//y
	ry.mat[0][0] = cosf(y);		ry.mat[0][1] = 0.0f;		ry.mat[0][2] = -sinf(y);	ry.mat[0][3] = 0.0f;
	ry.mat[1][0] = 0.0f;		ry.mat[1][1] = 1.0f;		ry.mat[1][2] = 0.0f;		ry.mat[1][3] = 0.0f;
	ry.mat[2][0] = sinf(y);		ry.mat[2][1] = 0.0f;		ry.mat[2][2] = cosf(y);		ry.mat[2][3] = 0.0f;
	ry.mat[3][0] = 0.0f;		ry.mat[3][1] = 0.0f;		ry.mat[3][2] = 0.0f;		ry.mat[3][3] = 1.0f;
//z
	rz.mat[0][0] = cosf(z);		rz.mat[0][1] = sinf(z);		rz.mat[0][2] = 0.0f;		rz.mat[0][3] = 0.0f;
	rz.mat[1][0] = -sinf(z);	rz.mat[1][1] = cosf(z);		rz.mat[1][2] = 0.0f;		rz.mat[1][3] = 0.0f;
	rz.mat[2][0] = 0.0f;		rz.mat[2][1] = 0.0f;		rz.mat[2][2] = 1.0f;		rz.mat[2][3] = 0.0f;
	rz.mat[3][0] = 0.0f;		rz.mat[3][1] = 0.0f;		rz.mat[3][2] = 0.0f;		rz.mat[3][3] = 1.0f;
	mat = rz * ry * rx;
}

void Pipeline::initTranslationTrans(Matrix4f& mat)
{
	mat.mat[0][0] = 1.0f;		mat.mat[0][1] = 0.0f;		mat.mat[0][2] = 0.0f;		mat.mat[0][3] = m_worldPos.x;
	mat.mat[1][0] = 0.0f;		mat.mat[1][1] = 1.0f;		mat.mat[1][2] = 0.0f;		mat.mat[1][3] = m_worldPos.y;
	mat.mat[2][0] = 0.0f;		mat.mat[2][1] = 0.0f;		mat.mat[2][2] = 1.0f;		mat.mat[2][3] = m_worldPos.z;
	mat.mat[3][0] = 0.0f;		mat.mat[3][1] = 0.0f;		mat.mat[3][2] = 0.0f;		mat.mat[3][3] = 1.0f;
}

void Pipeline::initCameraTransform(Matrix4f& trans, Matrix4f& rot)
{
	trans.mat[0][0] = 1.0f;		trans.mat[0][1] = 0.0f;		trans.mat[0][2] = 0.0f;		trans.mat[0][3] = -m_camera.Pos.x;
	trans.mat[1][0] = 0.0f;		trans.mat[1][1] = 1.0f;		trans.mat[1][2] = 0.0f;		trans.mat[1][3] = -m_camera.Pos.y;
	trans.mat[2][0] = 0.0f;		trans.mat[2][1] = 0.0f;		trans.mat[2][2] = 1.0f;		trans.mat[2][3] = -m_camera.Pos.z;
	trans.mat[3][0] = 0.0f;		trans.mat[3][1] = 0.0f;		trans.mat[3][2] = 0.0f;		trans.mat[3][3] = 1.0f;

	Vector3f N = m_camera.Target;
	N.Normalize();
	Vector3f U = m_camera.Up;
	U.Normalize();
	U = U.Cross(N);
	Vector3f V = N.Cross(U);

	rot.mat[0][0] = U.x;		rot.mat[0][1] = U.y;		rot.mat[0][2] = U.z;		rot.mat[0][3] = 0.0f;
	rot.mat[1][0] = V.x;		rot.mat[1][1] = V.y;		rot.mat[1][2] = V.z;		rot.mat[1][3] = 0.0f;
	rot.mat[2][0] = N.x;		rot.mat[2][1] = N.y;		rot.mat[2][2] = N.z;		rot.mat[2][3] = 0.0f;
	rot.mat[3][0] = 0.0f;		rot.mat[3][1] = 0.0f;		rot.mat[3][2] = 0.0f;		rot.mat[3][3] = 1.0f;
}

void Pipeline::initPersPectiveProjTrans(Matrix4f& mat)
{
	const float ar = m_persProj.Width / m_persProj.Height;
	const float zNear = m_persProj.zNear;
	const float zFar = m_persProj.zFar;
	const float zRange = zFar-zNear;
	const float tanHalfFOV = tanf(ToRadian(m_persProj.FOV / 2));

	mat.mat[0][0] = 1.0f/(tanHalfFOV*ar);	mat.mat[0][1] = 0.0f;				mat.mat[0][2] = 0.0f;					mat.mat[0][3] = 0.0f;
	mat.mat[1][0] = 0.0f;					mat.mat[1][1] = 1.0f/tanHalfFOV;	mat.mat[1][2] = 0.0f;					mat.mat[1][3] = 0.0f;
	mat.mat[2][0] = 0.0f;					mat.mat[2][1] = 0.0f;				mat.mat[2][2] = -(zNear+zFar)/zRange;	mat.mat[2][3] = -2.0f*zFar*zNear/zRange;
	mat.mat[3][0] = 0.0f;					mat.mat[3][1] = 0.0f;				mat.mat[3][2] = -1.0f;					mat.mat[3][3] = 0.0f;
}


const Matrix4f* Pipeline::GetWorldTrans()
{
	Matrix4f ScaleTrans, RotateTrans, TranslationTrans;
	initScaleTrans(ScaleTrans);
	initRotateTrans(RotateTrans);
	initTranslationTrans(TranslationTrans);
	m_transformation =  TranslationTrans * RotateTrans * ScaleTrans;
	return &m_transformation;

}

const Matrix4f* Pipeline::GetTrans()
{
	Matrix4f ScaleTrans, RotateTrans, TranslationTrans, PersProjTrans, CameraTrans, CameraRot;
	initScaleTrans(ScaleTrans);
	initRotateTrans(RotateTrans);
	initTranslationTrans(TranslationTrans);
	initCameraTransform(CameraTrans, CameraRot);
	initPersPectiveProjTrans(PersProjTrans);
	m_transformation = PersProjTrans * CameraRot * CameraTrans * TranslationTrans * RotateTrans * ScaleTrans;
	return &m_transformation;

}