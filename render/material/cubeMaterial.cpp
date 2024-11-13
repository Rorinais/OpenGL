#include "cubeMaterial.h"


CubeMaterial::CubeMaterial() noexcept {
	mType = MaterialName::CubeMaterial;
	mIsCubeMaterial = true;
	mDrawMode = DrawMode::Triangles;
}

CubeMaterial::~CubeMaterial() noexcept {}
