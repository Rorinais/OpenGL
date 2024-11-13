#include "meshBasicMaterial.h"

MeshBasicMaterial::MeshBasicMaterial() noexcept {
	mType = MaterialName::MeshBasicMaterial;
	mIsMeshBasicMaterial = true;
	mDrawMode = DrawMode::Triangles;
}

MeshBasicMaterial::~MeshBasicMaterial() noexcept {}