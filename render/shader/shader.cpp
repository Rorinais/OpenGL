#include"shader.h"
#include<iostream>
using namespace std;
#include <fstream>
#include<string>
#include "../math3d/math3d.h"
#include"../math3d/Utils.h"

shader::shader(const char* v, const char* f){
	VFilename = v;
	FFilename = f;
	m_shaderProg = 0;
	CompileShader();
}

shader::~shader() {

	cout <<"着色器对象：" << m_shaderProg << endl;
	if (m_shaderProg != 0)
	{
		glUseProgram(0); // 解绑当前的程序对象
		glDeleteProgram(m_shaderProg);
		m_shaderProg = 0;
	}

	glDeleteShader(m_shaderProg);

	//for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
	//{
	//	glDeleteShader(*it);
	//}
}

bool shader::Init() {

	m_shaderProg = glCreateProgram();
	if (m_shaderProg == 0)
	{
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}
	return true;
}
bool shader::AddShader(GLenum ShaderType, const char* pFilename) {

	string s;
	if (!ReadFile(pFilename, s)) {
		return false;
	}

	GLuint shaderObj = glCreateShader(ShaderType);
	if (shaderObj == 0) {
		fprintf(stderr, "Error creating shader type%d\n", ShaderType);
		return false;
	}
	//m_shaderObjList.push_back(shaderObj);

	const GLchar* p[1];
	p[0] = s.c_str();
	GLint Lengths[1] = { (GLint)s.size() };

	glShaderSource(shaderObj, 1, p, Lengths);
	glCompileShader(shaderObj);

	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling  '%s': '%s'\n", pFilename, infoLog);
		return false;
	}
	cout << "着色器对象：" << m_shaderProg << endl;
	glAttachShader(m_shaderProg, shaderObj);
	return true;

}

bool shader::Finalize() {
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(m_shaderProg);

	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
	if (Success == 0)
	{
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		return false;
	}

	glValidateProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		return false;
	}
	return GLCheckError();
	//for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++) 
	//{ 

	//	glDeleteShader(*it); 
	//}
	//m_shaderObjList.clear();

	glDeleteShader(m_shaderProg);
}

GLint shader::GetProgramParam(GLint param) {
	GLint ret;
	glGetProgramiv(m_shaderProg, param, &ret);
	return ret;

}

void shader::Enable(GLuint shader_program_id) 
{ 

	glUseProgram(shader_program_id);
}


bool shader::CompileShader() {
	if (!Init())
	{

		return false;
	}
	if (!AddShader(GL_VERTEX_SHADER, VFilename))
	{

		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, FFilename))
	{

		return false;
	}
	if (!Finalize())
	{

		return false;
	}
}
GLint shader::GetUniformLocation(const char* PUniformName) {

	GLuint Location = glGetUniformLocation(m_shaderProg, PUniformName);

	if (Location == -1)
	{
		fprintf(stderr, "Warning!Unable to get the location of uniform %s\n", PUniformName);
	}
	return Location;
}

