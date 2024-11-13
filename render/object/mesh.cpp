#include"mesh.h"

Mesh::Mesh(const Geometry::Ptr& geometry, const Material::Ptr& material)noexcept
{
	mIsMesh = true;
}

Mesh::~Mesh()noexcept {}