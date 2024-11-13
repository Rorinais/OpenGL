#include "shaderLighting.h"
#include "../math3d/math3d.h"

LightingTechinque::LightingTechinque(GLuint shaderProg) {
	m_shaderProg = shaderProg;
}

LightingTechinque::~LightingTechinque() {

}

GLint LightingTechinque::GetUniformLocation(const char* PUniformName) {

	GLuint Location = glGetUniformLocation(m_shaderProg, PUniformName);

	if (Location == -1)
	{
		fprintf(stderr, "Warning!Unable to get the location of uniform %s\n", PUniformName);
	}
	return Location;
}

bool LightingTechinque::SetUniformLocation() {

	m_WVPLocation = GetUniformLocation("gWVP");
	m_WorldMatrixLocation = GetUniformLocation("gWorld");
	m_samperLocation0 = GetUniformLocation("gSampler0");
	m_samperLocation1 = GetUniformLocation("gSampler1");

	m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Color");
	m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.AmbientIntensity");
	m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.DiffuseIntensity");

	m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
	m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
	m_matSpecularPowerLocation = GetUniformLocation("gMatSpecularPower");
	m_MaterialIndexLocation = GetUniformLocation("gMaterialIndex");
	m_CenterLocation = GetUniformLocation("gCenter");

	return true;
}

void LightingTechinque::SetWVP(const Matrix4f& WVP) {

	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.mat);

}
void LightingTechinque::SetWorldMatrix(const Matrix4f& World) {

	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)World.mat);

}

void LightingTechinque::SetDirectionalLight(const DirectionalLight& Light) {

	glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);

	Vector3f Direction = Light.Direction;
	Direction.Normalize();
	glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void LightingTechinque::SetTextureUnit(unsigned int Texture0, unsigned int Texture1) {

	glUniform1i(m_samperLocation0, Texture0);
	glUniform1i(m_samperLocation1, Texture1);

}

void LightingTechinque::SetEyeWorldPos(const Vector3f& EyeWorldPos)
{
	glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}

void LightingTechinque::SetMatSpecularIntensity(float Intensity)
{
	glUniform1f(m_matSpecularIntensityLocation, Intensity);
}

void LightingTechinque::SetMatSpecularPower(float Power)
{
	glUniform1f(m_matSpecularPowerLocation, Power);
}

void LightingTechinque::GetMaterialIndex(int MaterialIndex)
{
	glUniform1i(m_MaterialIndexLocation, MaterialIndex);
}
void LightingTechinque::SetCenter(const Vector3f& m_Center)
{
	glUniform3f(m_CenterLocation, 1, 1, 1);
	//glUniform3f(m_CenterLocation, m_Center.x, m_Center.y, m_Center.z);
}