#pragma once
#include"../global/base.h"
#include"renderableObject.h"

class Mesh :public RenderableObject
{
public:
	using Ptr = std::shared_ptr<Mesh>;
	static Ptr Create(const Geometry::Ptr& geometry, const Material::Ptr& material)
	{
		return std::make_shared<Mesh>(geometry, material);
	}
	Mesh(const Geometry::Ptr& geometry, const Material::Ptr& material)noexcept;

	~Mesh()noexcept;
};