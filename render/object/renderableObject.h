#pragma once
#include "../core/object3d.h"
#include "../core/geometry.h"
#include "../material/material.h"
#include "../global/base.h"
#include "../global/constant.h"

class Renderer;
class Scene;
class Camera;

class RenderableObject :public Object3d
{
public:
	using OnBeforeRenderCallback = std::function<void(Renderer*, Scene*, Camera*)>;

	using Ptr = std::shared_ptr<RenderableObject>;
	static Ptr Create(const Geometry::Ptr& geometry, const Material::Ptr& material)
	{
		return std::make_shared<RenderableObject>(geometry, material);
	}

	RenderableObject(const Geometry::Ptr& geometry, const Material::Ptr& material)noexcept;

	~RenderableObject()noexcept;

	auto GetGeometry()const noexcept { return mGeometry; }
	auto GetMaterial()const noexcept { return mMaterial; }

	void OnBeforeRender(Renderer* renderer, Scene* scene, Camera* camera);

public:
	OnBeforeRenderCallback mOnBeforeRenderCallback{ nullptr };

protected:
	Geometry::Ptr mGeometry{nullptr};
	Material::Ptr mMaterial{nullptr};
};

