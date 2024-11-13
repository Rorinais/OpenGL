#include"renderableObject.h"


RenderableObject::RenderableObject(const Geometry::Ptr& geometry, const Material::Ptr& material) noexcept {
	mGeometry = geometry;
	mMaterial = material;

	mIsRenderableObject = true;
}

RenderableObject::~RenderableObject() noexcept {}

void RenderableObject::OnBeforeRender(Renderer* renderer, Scene* scene, Camera* camera) {
	if (mOnBeforeRenderCallback) {
		mOnBeforeRenderCallback(renderer, scene, camera);
	}
}