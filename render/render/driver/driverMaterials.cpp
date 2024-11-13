#include"driverMaterials.h"
#include "../../textures/texture.h"

DriverMaterial::DriverMaterial()noexcept{}

DriverMaterial::~DriverMaterial()noexcept{}

DriverMaterials::DriverMaterials(const DriverPrograms::Ptr& programs) noexcept {
	mPrograms = programs;
	EventDispatcher::getInstance()->addEventListener("materialDispose", this, DriverMaterials::OnMaterialDispose);
}

DriverMaterials::~DriverMaterials() noexcept {
	EventDispatcher::getInstance()->removeEventListener("materialDispose", this, DriverMaterials::OnMaterialDispose);
}

DriverMaterial::Ptr DriverMaterials::Get(const Material::Ptr& material) noexcept {
	auto iter = mMaterials.find(material->GetId());

	if (iter==mMaterials.end())
	{
		iter = mMaterials.insert(std::make_pair(material->GetId(), DriverMaterial::Create())).first;
	}
	return iter->second;
}

void DriverMaterials::OnMaterialDispose(const EventBase::Ptr& event) {
	auto material = (Material*)event->mTarget;
	auto iter = mMaterials.find(material->GetId());
	if (iter==mMaterials.end())
	{
		return;
	}

	auto dMaterial = iter->second;
	auto programs = dMaterial->mPrograms;
	for (const auto&pIter:programs)
	{
		auto program = pIter.second;
		mPrograms->Release(program);
	}
	mMaterials.erase(iter);
}

void DriverMaterials::RefreshMaterialUniforms(UniformHandleMap& uniformHandleMap, const Material::Ptr& material) {
	uniformHandleMap["opacity"].mValue = material->mOpacity;
	uniformHandleMap["opacity"].mNeedsUpdate = true;

	if (material->mIsMeshBasicMaterial) {
		auto basicMaterial = std::static_pointer_cast<MeshBasicMaterial>(material);
		RefreshMaterialBasic(uniformHandleMap, basicMaterial);
	}

	if (material->mIsMeshPhongMaterial) {
		auto phongMaterial = std::static_pointer_cast<MeshPhongMaterial>(material);
		RefreshMaterialPhong(uniformHandleMap, phongMaterial);
	}

	if (material->mIsCubeMaterial) {
		auto cubeMaterial = std::static_pointer_cast<CubeMaterial>(material);
		RefreshMaterialCube(uniformHandleMap, cubeMaterial);
	}
}

void DriverMaterials::RefreshMaterialPhong(UniformHandleMap& uniformHandleMap, const MeshPhongMaterial::Ptr& material) {
	uniformHandleMap["shininess"].mValue = material->mShininess;
	uniformHandleMap["shininess"].mNeedsUpdate = true;

	if ((material->mDiffuseMap && material->mDiffuseMap->mNeedsUpdate) || material->mNeedsUpdate) {
		uniformHandleMap["diffuseMap"].mValue = material->mDiffuseMap;
		uniformHandleMap["diffuseMap"].mNeedsUpdate = true;
	}

	if ((material->mNormalMap && material->mNormalMap->mNeedsUpdate) || material->mNeedsUpdate) {
		uniformHandleMap["normalMap"].mValue = material->mNormalMap;
		uniformHandleMap["normalMap"].mNeedsUpdate = true;
	}

	if ((material->mSpecularMap && material->mSpecularMap->mNeedsUpdate) || material->mNeedsUpdate) {
		uniformHandleMap["specularMap"].mValue = material->mSpecularMap;
		uniformHandleMap["specularMap"].mNeedsUpdate = true;
	}
}

void DriverMaterials::RefreshMaterialBasic(UniformHandleMap& uniformHandleMap, const MeshBasicMaterial::Ptr& material) {
	if ((material->mDiffuseMap && material->mDiffuseMap->mNeedsUpdate) || material->mNeedsUpdate) {
		uniformHandleMap["diffuseMap"].mValue = material->mDiffuseMap;
		uniformHandleMap["diffuseMap"].mNeedsUpdate = true;
	}
}

void DriverMaterials::RefreshMaterialCube(UniformHandleMap& uniformHandleMap, const CubeMaterial::Ptr& material) {
	if ((material->mEnvMap && material->mEnvMap->mNeedsUpdate) || material->mNeedsUpdate) {
		uniformHandleMap["envMap"].mValue = material->mEnvMap;
		uniformHandleMap["envMap"].mNeedsUpdate = true;
	}
}
