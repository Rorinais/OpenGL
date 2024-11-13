#include <assert.h>
#include"mesh.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include"../texture/texture.h"
//#include "../shader/shaderLighting.h"
#include "../shader/shader.h"
#include"../math3d/math3d.h"
#define SAFE_DELETE(p) if (p) { delete p; p = nullptr; }

Mesh::Mesh() {
	
}

Mesh::~Mesh() {
	Clear();
}
void Mesh::Clear() {
	for (unsigned int i = 0; i < m_Textures.size(); i++) {
		SAFE_DELETE(m_Textures[i]);
	}
}

bool Mesh::LoadMesh(const std::string& Filename) {
	bool Ret = false;
	Assimp::Importer Importer;  // 创建 Assimp 的导入器对象

	// 使用导入器对象读取模型文件，并进行一些预处理操作，如三角化、生成平滑法线、反转纹理坐标等
	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (pScene)
	{
		Ret = InitFromScene(pScene, Filename);  // 如果成功读取了模型文件，则调用 InitFromScene 方法解析数据
	}
	else
	{
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());  // 输出错误信息
		return Ret;
	}

	return Ret; 

}


void FindEmptyNodes(const aiScene* scene, const aiNode* node)
{
	// 检查当前节点是否为空对象
	//if (node->mNumMeshes == 0)
	//{
	//	// 打印或处理空对象节点
	//	std::cout << "Empty node name: " << node->mName.C_Str() << std::endl;

	//}
	aiString string1;

	const char* text = "center";
	string1.Set(text);
	if (node->mNumMeshes >= 0 )
	{
		// 获取空物体节点的变换矩阵
		aiMatrix4x4 transform = node->mTransformation;

		// 提取位移向量
		aiVector3D translation;
		aiQuaternion rotation;
		aiVector3D scaling;
		transform.Decompose(translation, rotation, scaling);

		// 打印空物体节点的名称和位置
		std::cout << "Empty object name: " << node->mName.C_Str() << std::endl;
		std::cout << "Position: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << std::endl;
	}
	// 递归处理子节点
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		FindEmptyNodes(scene, node->mChildren[i]);
	}
}

void Mesh::processNode(const aiNode* node, const aiScene* scene)
{
	// 处理当前节点
	if (std::string(node->mName.C_Str()) == "center") 
	{
		// 处理空物体
		Matrix4f transform;
		aiMatrix4x4 aiTransform = node->mTransformation;
		transform.fromAiMatrix4x4(aiTransform);
		transform.TransMatrix4f();
		Vector3f position(transform.mat[3][0], transform.mat[3][1], transform.mat[3][2]);

		m_center = position;
		//std::cout << "Empty object name: " << node->mName.C_Str() << std::endl;
		//cout <<"position:" << position.x <<"-" << position.y <<"-" << position.z << endl;
	}
	// 遍历子节点
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}



bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename) {

	m_Entries.resize(pScene->mNumMeshes); // 调整 m_Entries 的大小为场景中的网格数目
	m_Textures.resize(pScene->mNumMaterials + 5); // 调整 m_Textures 的大小为场景中的材质数目加上 4

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i]; // 获取场景中的第 i 个网格

		InitMesh(i, paiMesh); // 初始化网格，将网格数据存储到 m_Entries 中的第 i 个元素中
	}

	const aiNode* rootNode = pScene->mRootNode;

	processNode(rootNode, pScene);
	// 查找空对象节点
	//FindEmptyNodes(pScene, rootNode);

	return InitMaterials(pScene, Filename); // 初始化材质，并返回结果

}


void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh) {

	// 获取材质索引
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);



	// 遍历网格的所有顶点
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		// 获取顶点位置
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);

		// 获取顶点法线
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);

		// 获取纹理坐标，如果没有纹理坐标则使用(0, 0, 0)
		const aiVector3D* pTexCoord0 = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		// 将顶点位置、法线、纹理坐标存入 Vertex 结构体
		Vertex v(Vector3f(pPos->x, pPos->y, pPos->z), Vector2f(pTexCoord0->x, pTexCoord0->y), Vector3f(pNormal->x, pNormal->y, pNormal->z));
		Vertices.push_back(v);
	}

	// 遍历网格的所有面
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];

		assert(Face.mNumIndices == 3);

		// 获取面的绘制顺序，即三个顶点的索引
		indices.push_back(Face.mIndices[0]);
		indices.push_back(Face.mIndices[1]);
		indices.push_back(Face.mIndices[2]);
	}

	// 将顶点数据和绘制数据存储到 m_Entries 中的指定位置
	m_Entries[Index].Init(Vertices, indices);

}


void Mesh::GetTexture(aiTextureType TextureType,const aiMaterial* pMaterial, std::string Dir,int i,bool Ret)
{

	aiString diffusePath; // 创建 aiString 对象，用于存储贴图文件路径

	// 使用 pMaterial->GetTexture 方法获取指定类型的第0个贴图的路径信息，并将结果存储在 diffusePath 中。
	// 如果成功获取到贴图信息，则进入 if 语句块。
	if (pMaterial->GetTexture(TextureType, 0, &diffusePath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
	{
		std::string texturePath = diffusePath.data;  // 将 aiString 对象转换为 std::string 类型的 texturePath

		std::string prefix = "g.fbm\\";  // 定义要删除的前缀字符串
		size_t found = texturePath.find(prefix);  // 查找 texturePath 中是否包含指定前缀

		// 如果找到了指定前缀，则从 texturePath 中删除该前缀
		if (found != std::string::npos) {
			texturePath.erase(found, prefix.length());
		}

		std::string FullPath = Dir + "/" + "texture"+"/"+texturePath;  // 将模型文件所在目录与贴图文件路径拼接成完整的贴图文件路径
		// 创建一个 GL_TEXTURE_2D 类型的纹理对象，并使用 FullPath 初始化它
		m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());


		// 调用纹理对象的 load 方法加载纹理数据，并根据返回值判断是否加载成功
		if (!m_Textures[i]->load())
		{
			printf("Error loading texture '%s'\n", FullPath.c_str());
			delete m_Textures[i];
			m_Textures[i] = NULL;
			Ret = false;  // 设置返回值为 false，表示加载纹理失败
		}
		else
		{
			printf("Loaded texture '%s'\n", FullPath.c_str());
		}
	}


}


bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
	// 获取文件路径中最后一个斜杠的位置
	std::string::size_type SlashIndex = Filename.find_last_of("/");

	std::string Dir;

	// 如果没有找到斜杠，则将目录设置为当前目录 "."
	if (SlashIndex == std::string::npos)
	{
		Dir = ".";
	}
	// 如果斜杠在文件路径的开头，则将目录设置为根目录 "/"
	else if (SlashIndex == 0)
	{
		Dir = "/";
	}
	// 否则，截取斜杠之前的部分作为目录
	else
	{
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;
	// 遍历每个材质
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];
		unsigned int diffuseTextureCount = pMaterial->GetTextureCount(aiTextureType_DIFFUSE);
		unsigned int specularTextureCount = pMaterial->GetTextureCount(aiTextureType_SPECULAR);
		m_Textures[i] = NULL;

		// 如果材质有漫反射贴图和镜面反射贴图
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{

			// 获取漫反射贴图
			GetTexture(aiTextureType_DIFFUSE, pMaterial, Dir, i, Ret);
		}
		else
		{
			// 如果材质没有漫反射贴图，则加载默认的贴图
			m_Textures[i] = new Texture(GL_TEXTURE_2D, "assets/texture/brow.png");
			Ret = m_Textures[i]->load();
		}
		if (pMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
		{

			// 获取镜面反射贴图
			GetTexture(aiTextureType_SPECULAR, pMaterial, Dir, i + 5, Ret);
		}
		else
		{
			// 如果材质没有漫反射贴图，则加载默认的贴图
			m_Textures[i] = new Texture(GL_TEXTURE_2D, "assets/texture/eyes.png");
			Ret = m_Textures[i]->load();
		}
	}

	// 返回贴图加载成功与否的布尔值
	return Ret;

}

Mesh::MeshEntry::MeshEntry() {
	VB = GL_INVALID_VALUE;
	IB = GL_INVALID_VALUE;

	NumIndices = 0;
	MaterialIndex = INVALID_MATERIAL;
}

Mesh::MeshEntry::~MeshEntry(){

	if (VB!=GL_INVALID_VALUE)
	{
		glDeleteBuffers(1, &VB);
	}
	if (IB != GL_INVALID_VALUE)
	{
		glDeleteBuffers(1, &IB);
	}
}

void Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices) {

	// 记录索引数量
	NumIndices = Indices.size();

	// 创建顶点缓冲区并绑定到 GL_ARRAY_BUFFER 目标上
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);	// 将顶点数据复制到缓冲区中

	// 创建索引缓冲区并绑定到 GL_ELEMENT_ARRAY_BUFFER 目标上
	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);	// 将索引数据复制到缓冲区中

	// 创建顶点数组对象并绑定
	glGenVertexArrays(1, &VA);
	glBindVertexArray(VA);
}
void Mesh::Render(shader* m_pEffect)
//void Mesh::Render(LightingTechinque* m_pEffect)
{
	glEnableVertexAttribArray(0); // 启用顶点属性数组 0
	glEnableVertexAttribArray(1); // 启用顶点属性数组 1
	glEnableVertexAttribArray(2); // 启用顶点属性数组 2
	glEnableVertexAttribArray(3); // 启用顶点属性数组 3

	m_pEffect->setVec3("gCenter", m_center.x, m_center.y, m_center.z);
	//遍历每个顶点，获取顶点属性
	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB); // 绑定顶点缓冲区

		// 设置顶点属性指针
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // 属性 0，位置属性，每个顶点 3 个浮点数
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12); // 属性 1，纹理坐标属性，每个顶点 2 个浮点数，偏移量 12
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); // 属性 2，纹理坐标属性，每个顶点 2 个浮点数，偏移量 20
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32); // 属性 3，法线属性，每个顶点 3 个浮点数，偏移量 32

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB); // 绑定索引缓冲区

		if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex])
		{
			m_Textures[MaterialIndex]->Bind(GL_TEXTURE0); // 绑定材质纹理到纹理单元 0

			if (m_Textures[MaterialIndex + 5])
			{
				m_Textures[MaterialIndex + 5]->Bind(GL_TEXTURE1); // 绑定额外纹理到纹理单元 1
			}
		}
		m_pEffect->setInt("gMaterialIndex", MaterialIndex);
		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0); // 绘制三角形面片

	}

	glDisableVertexAttribArray(0); // 禁用顶点属性数组 0
	glDisableVertexAttribArray(1); // 禁用顶点属性数组 1
	glDisableVertexAttribArray(2); // 禁用顶点属性数组 2
	glDisableVertexAttribArray(3); // 禁用顶点属性数组 3

}

