#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include"../math3d/math3d.h"
#include"../core/attribute.h"
#include"../core/geometry.h"
#include<vector>

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;
	Vector3f m_normal;
	Vertex() {}
	Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

class Render {
public:
	Render()
	{
		VBO = GL_INVALID_VALUE;
		EBO = GL_INVALID_VALUE;

	}
	~Render()
	{
		if (VBO != GL_INVALID_VALUE)
		{
			glDeleteBuffers(1, &VBO);
		}
		if (EBO != GL_INVALID_VALUE)
		{
			glDeleteBuffers(1, &EBO);
		}
	}


	void Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);
	void Init(const Geometry::AttributeMap& Vertices, const std::vector<unsigned int>& Indices);
	void render();

private:

	unsigned int NumIndices;
	GLuint VBO;
	GLuint EBO;
	GLuint VAO;
};