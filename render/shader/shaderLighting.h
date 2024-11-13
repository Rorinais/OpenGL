#pragma once
#include "../math3d/math3d.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
struct DirectionalLight
{
	Vector3f Color;
	float AmbientIntensity;

	Vector3f Direction;
	float DiffuseIntensity;
};

class LightingTechinque
{
public:
	LightingTechinque(GLuint shaderProg);

	~LightingTechinque();

	virtual bool SetUniformLocation();

	GLint GetUniformLocation(const char* PUniformName);
	GLuint m_shaderProg;

	void SetWVP(const Matrix4f& WVP);
	void SetWorldMatrix(const Matrix4f& World);
	void SetTextureUnit(unsigned int Texture0, unsigned int Texture1);
	void SetDirectionalLight(const DirectionalLight& Light);

	void SetEyeWorldPos(const Vector3f& EyeWorldPos);
	void SetMatSpecularIntensity(float Intensity);
	void SetMatSpecularPower(float Power);
	void GetMaterialIndex(int MaterialIndex);
	void SetCenter(const Vector3f& m_Center);

private:
	GLuint m_WVPLocation;
	GLuint m_WorldMatrixLocation;
	GLuint m_samperLocation0;
	GLuint m_samperLocation1;
	GLuint m_MaterialIndexLocation;

	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	GLuint m_CenterLocation;
	struct
	{
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint Direction;
		GLuint DiffuseIntensity;

	}m_dirLightLocation;
};

