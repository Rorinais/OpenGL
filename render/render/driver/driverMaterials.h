#pragma once
#include"../../global/base.h"
#include"../../global/constant.h"
#include"../../global/eventDispatcher.h"
#include "../../material/material.h"
#include "../../material/cubeMaterial.h"
#include "../../material/meshBasicMaterial.h"
#include "../../material/meshPhongMaterial.h"
#include"driverPrograms.h"
#include"driverUniforms.h"
#include"driverTextures.h"
#include"../shaders/shaderLib.h"

class DriverMaterial {
public:
	using Ptr = std::shared_ptr<DriverMaterial>;
	static Ptr Create() { return std::make_shared<DriverMaterial>(); }

	DriverMaterial()noexcept;

	~DriverMaterial()noexcept;
public:
	uint32_t			mVersion{ 0 };
	bool				mInstancing{ false };
	DriverProgram::Ptr	mCurrentProgram{ nullptr };

	Texture::Ptr			mDiffuseMap{ nullptr };
	Texture::Ptr			mEnvMap{ nullptr };
	Texture::Ptr			mNormalMap{ nullptr };
	Texture::Ptr			mSpecularMap{ nullptr };

	bool					mNeedsLight{ nullptr };
	uint32_t				mLightsStateVersion{ 0 };

	bool					mSkinning{ false };
	uint32_t				mMaxBones{ 0 };

	std::unordered_map<HashType, DriverProgram::Ptr> mPrograms{};

	UniformHandleMap		mUniforms{};
};

class DriverMaterials {
public:
	using Ptr = std::shared_ptr<DriverMaterials>;
	static Ptr Create(const DriverPrograms::Ptr& programs) { return std::make_shared<DriverMaterials>(programs); }

	DriverMaterials(const DriverPrograms::Ptr& programs) noexcept;

	~DriverMaterials() noexcept;


	DriverMaterial::Ptr Get(const Material::Ptr& material) noexcept;

	void OnMaterialDispose(const EventBase::Ptr& event);


	static void RefreshMaterialUniforms(UniformHandleMap& uniformHandleMap, const Material::Ptr& material);

	static void RefreshMaterialPhong(UniformHandleMap& uniformHandleMap, const MeshPhongMaterial::Ptr& material);

	static void RefreshMaterialBasic(UniformHandleMap& uniformHandleMap, const MeshBasicMaterial::Ptr& material);

	static void RefreshMaterialCube(UniformHandleMap& uniformHandleMap, const CubeMaterial::Ptr& material);

private:
	DriverPrograms::Ptr mPrograms{ nullptr };

	std::unordered_map<ID, DriverMaterial::Ptr> mMaterials{};
};