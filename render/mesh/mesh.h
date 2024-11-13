#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>
#include <iostream>
#include <vector>
#include"../texture/texture.h"
#include"../math3d/math3d.h"
//#include "../shader/shaderLighting.h"
#include"../shader/shader.h"
#include"render.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool LoadMesh(const std::string& Filename);
	void Render(shader* m_pEffect);

	//void Render(LightingTechinque* m_pEffect);

private:
#define INVALID_MATERIAL 0XFFFFFFFF

	struct MeshEntry
	{
		MeshEntry();
		~MeshEntry();

		void Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);

		GLuint VB;
		GLuint IB;
		GLuint VA;

		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	void processNode(const aiNode* node, const aiScene* scene);
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void GetTexture(aiTextureType TextureType, const aiMaterial* pMaterial, std::string Dir, int i, bool Ret);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

	Vector3f m_center;
	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;
};