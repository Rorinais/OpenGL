#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <cstring>
#include <cassert>
#include<string>
#include <iostream>
#include <fstream>
#include<map>
#include<vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include <math.h>
#include "render/Application/Application.h"
#include "render/math3d/Utils.h"
#include "render/math3d/math3d.h"
#include "render/mesh/mesh.h"
#include "render/pipeline/Pipeline.h"
#include "render/pipeline/Camera.h"
#include"render/shader/shader.h"
//#include "render/shader/shaderLighting.h"
#include"render/texture/texture.h"
#include"render/wrapper/checkError.h"
#include"render/mesh/render.h"
using namespace std;

Camera* pGameCamera = NULL;
Mesh* m_mesh = NULL;
shader* m_pEffect = NULL;
DirectionalLight Light;
Render* m = NULL;



void OnResize(int width, int height);
void OnKey(int key, int scancode, int action, int mods);
void KeyPressFunc(int Key);
void OnMouse(double x, double y);
void OnScroll(double xoffset, double yoffset);
void render();


int main()
{

	if (!App->init(800, 600))
	{
		return -1;
	}
	App->setResizeCallback(OnResize);
	App->setKeyBoardCallback(OnKey);
	App->setScrollCallback(OnScroll);
	App->setMouseCallback(OnMouse);


	pGameCamera = new Camera(800, 600, App->mWindow);

	m_mesh = new Mesh;
	m_mesh->LoadMesh("assets/g.fbx");

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> indices;

	Vertices.push_back(Vertex(Vector3f(-0.5f, -0.5f, 0.5f), Vector2f(0.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f)));
	Vertices.push_back(Vertex(Vector3f(0.5f, -0.5f, 0.5f), Vector2f(1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f)));
	Vertices.push_back(Vertex(Vector3f(0.5f, 0.5f, 0.5f), Vector2f(1.0f, 1.0f), Vector3f(0.0f, 0.0f, 1.0f)));
	Vertices.push_back(Vertex(Vector3f(-0.5f, 0.5f, 0.5f), Vector2f(0.0f, 1.0f), Vector3f(0.0f, 0.0f, 1.0f)));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	m = new Render;

	//m->Init(Vertices, indices);

	//shader s("assets/vert.glsl", "assets/frag.glsl");

	m_pEffect = new shader("assets/vert.glsl", "assets/frag.glsl");

	//Texture* Textures = new Texture(GL_TEXTURE_2D, "D:\\face.png");
	//Textures->load();


	Light.Color = Vector3f(1.0f, 1.0f, 1.0f);
	Light.AmbientIntensity = 0.5;
	Light.DiffuseIntensity = 0.5f;
	Light.Direction = Vector3f(1.0f, 0.0f, 0.0f);
	while (App->update())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pEffect->Enable(m_pEffect->GetshaderProg());
		render();
	}

	App->destroy();
	return 0;


}

void render()
{
	static float Scale = 0.0f;
	Scale += 0.5f;


	//设置模型矩阵
	Pipeline P;
	P.worldPos(0, -1, 2);

	P.Rotate(90, Scale, 0);

	//设置视图矩阵
	P.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
	//设置透视投影矩阵
	P.SetPerspectivePro(pGameCamera->GetFOV(), 800, 600, 1.0f, 100.0f);

	//设置MVP矩阵
	m_pEffect->setMat4("gWVP", *P.GetTrans());

	//设置世界矩阵
	const Matrix4f& WorldTransform = *P.GetWorldTrans();
	m_pEffect->setMat4("gWorld", WorldTransform);

	m_pEffect->setInt("gSampler0", 0);
	m_pEffect->setInt("gSampler1", 1);

	m_pEffect->setVec3("gDirectionalLight.Color", Light.Color.x, Light.Color.y, Light.Color.z);
	m_pEffect->setFloat("gDirectionalLight.AmbientIntensity", Light.AmbientIntensity);
	Vector3f Direction = Light.Direction;
	Direction.Normalize();
	m_pEffect->setVec3("gDirectionalLight.Direction", Direction.x, Direction.y, Direction.z);
	m_pEffect->setFloat("gDirectionalLight.DiffuseIntensity", Light.DiffuseIntensity);

	m_pEffect->setVec3("gEyeWorldPos", pGameCamera->GetPos().x, pGameCamera->GetPos().y, pGameCamera->GetPos().z);

	m_pEffect->setFloat("gMatSpecularIntensity", 1.0f);

	m_pEffect->setFloat("gMatSpecularPower", 32);

	//m->render();
	m_mesh->Render(m_pEffect);
}

void OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
}

void OnKey(int key, int scancode, int action, int mods)
{
	pGameCamera->OnKeyboard(App->mWindow,key);
	KeyPressFunc(key);
}

void KeyPressFunc(int Key)
{
	switch (Key)
	{
	case GLFW_KEY_UP:
		Light.AmbientIntensity += 0.05f;
		break;
	case GLFW_KEY_DOWN:
		Light.AmbientIntensity -= 0.05f;
		break;
	case GLFW_KEY_LEFT:
		Light.DiffuseIntensity += 0.05f;
		break;
	case GLFW_KEY_RIGHT:
		Light.DiffuseIntensity -= 0.05f;
		break;
	default:
		break;
	}
}
void OnMouse(double x, double y)
{
	pGameCamera->OnMouse(App->mWindow,x, y);
}

void OnScroll(double xoffset, double yoffset)
{
	pGameCamera->OnScroll(xoffset, yoffset);
}


