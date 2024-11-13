#pragma once
#include"../global/base.h"
#include"../core/object3d.h"
#include"../material/material.h"
#include"../texture/cubeTexture.h"

class Scene :public Object3d
{
public:
	using Ptr = std::shared_ptr<Scene>;
	static Ptr Create()
	{
		return std::make_shared<Scene>();
	}

	Scene()noexcept;

	~Scene()noexcept;

	Material::Ptr mOverrideMaterial{ nullptr };
	//CubeTexture::Ptr mBackground{ nullptr };
};