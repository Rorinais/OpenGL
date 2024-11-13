#include "meshPhongMaterial.h"

MeshPhongMaterial::MeshPhongMaterial() noexcept {
	mType = MaterialName::MeshPhongMaterial;
	mIsMeshPhongMaterial = true;
	mDrawMode = DrawMode::Triangles;
}

MeshPhongMaterial::~MeshPhongMaterial() noexcept {}