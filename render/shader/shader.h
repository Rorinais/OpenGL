
#pragma once
#include <glad/glad.h>
#include <vector> 
#include "../math3d/math3d.h"
#include"../math3d/Utils.h"

struct DirectionalLight
{
	Vector3f Color;
	float AmbientIntensity;

	Vector3f Direction;
	float DiffuseIntensity;
};
class shader
{
public:
	shader(const char* v, const char* f);
	virtual~shader();

	virtual bool Init();

	void Enable(GLuint shader_program_id);

	bool AddShader(GLenum ShaderType, const char* pFilename);

	bool CompileShader();

	GLuint GetshaderProg() { return m_shaderProg; }

	typedef std::vector<GLuint>ShaderObjList;

	GLint GetUniformLocation(const char* PUniformName);


	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_shaderProg, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_shaderProg, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_shaderProg, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	//void setVec2(const std::string& name, const glm::vec2& value) const
	//{
	//	glUniform2fv(glGetUniformLocation(m_shaderProg, name.c_str()), 1, &value[0]);
	//}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_shaderProg, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	//void setVec3(const std::string& name, const Vector3f& value) const
	//{
	//	glUniform3fv(glGetUniformLocation(m_shaderProg, name.c_str()), 1, &value[0]);
	//}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_shaderProg, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	/*void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(m_shaderProg, name.c_str()), 1, &value[0]);
	}*/
	void setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(m_shaderProg, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	//void setMat2(const std::string& name, const glm::mat2& mat) const
	//{
	//	glUniformMatrix2fv(glGetUniformLocation(m_shaderProg, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	//}
	//// ------------------------------------------------------------------------
	//void setMat3(const std::string& name, const glm::mat3& mat) const
	//{
	//	glUniformMatrix3fv(glGetUniformLocation(m_shaderProg, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	//}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const Matrix4f& WVP) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProg, name.c_str()), 1, GL_TRUE, (const GLfloat*)WVP.mat);
	}

public:

	void SetWVP(const Matrix4f& WVP) {

		setMat4("gWVP", WVP);
	}

	void SetWorldMatrix(const Matrix4f& World) {

		setMat4("gWorld", World);
	}

	void SetTextureUnit(unsigned int Texture0, unsigned int Texture1) {

		setInt("gSampler0", Texture0);
		setInt("gSampler1", Texture1);
	}
	void SetDirectionalLight(const DirectionalLight& Light) {
		setVec3("gDirectionalLight.Color", Light.Color.x, Light.Color.y, Light.Color.z);
		setFloat("gDirectionalLight.AmbientIntensity", Light.AmbientIntensity);
		Vector3f Direction = Light.Direction;
		Direction.Normalize();
		setVec3("gDirectionalLight.Direction", Direction.x, Direction.y, Direction.z);
		setFloat("gDirectionalLight.DiffuseIntensity", Light.DiffuseIntensity);
	}
	void SetEyeWorldPos(const Vector3f& EyeWorldPos) {
		setVec3("gEyeWorldPos", EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
	}
	void SetMatSpecularIntensity(float Intensity) {

		setFloat("gMatSpecularIntensity", Intensity);

	}
	void SetMatSpecularPower(float Power) {

		setFloat("gMatSpecularPower", Power);
	}
	void GetMaterialIndex(int MaterialIndex) {
		setInt("gMaterialIndex", MaterialIndex);
	}
	void SetCenter(const Vector3f& m_Center) {
		setVec3("gCenter", m_Center.x, m_Center.y, m_Center.z);
		//glUniform3f(m_CenterLocation, m_Center.x, m_Center.y, m_Center.z);
	}

	bool Finalize();

	GLint GetProgramParam(GLint param);

	const char* VFilename;
	const char* FFilename;
private:

	//ShaderObjList m_shaderObjList;

	GLuint m_shaderProg{ 0 };
};

