#pragma once
#include"../../global/base.h"
#include"../../object/renderableObject.h"
#include"../../material/material.h"
#include"../../material/depthMaterial.h"
#include"../../log/debugLog.h"
#include"driverUniforms.h"
#include"driverLights.h"
#include"driverShadowMap.h"
#include"../shaders/shaderLib.h"

class DriverPrograms;
class DriverProgram {
	friend DriverPrograms;
public:
	
	struct Parameters
	{
		using Ptr = std::shared_ptr<Parameters>;
		static Ptr Create() {
			return std::make_shared<Parameters>();
		}
		std::string mShaderId;
		std::string mVertex;
		std::string mFragment;

		bool		mInstancing{ false };
		bool		mHasNormal{ false };
		bool		mHasUV{ false };
		bool		mHasColor{ false };
		bool		mHasDiffuseMap{ false };
		bool		mHasEnvCubeMap{ false };
		bool		mHasSpecularMap{ false };

		bool		mShadowMapEnabled{ false };
		uint32_t	mDirectionalLightCount{ 0 };
		uint32_t	mNumDirectionalLightShadows{ 0 };

		bool		mUseTangent{ false };
		bool		mUseNormalMap{ false };

		bool		mSkinning{ false };
		uint32_t	mMaxBones{ 0 };

		uint32_t	mDepthPacking{ 0 };
	};

	using Ptr = std::shared_ptr<DriverProgram>;
	static Ptr Create(const Parameters::Ptr& parameters) {
		return std::make_shared<DriverProgram>(parameters);
	}

	DriverProgram(const Parameters::Ptr& parameters)noexcept;
	~DriverProgram()noexcept;

	auto GetId()const noexcept { return mId; }

	auto GetCasheKey()const noexcept { return mCacheKey; }

	GLuint		mProgram{ 0 };

	void		UploadUniforms(UniformHandleMap& uniformMap, const DriverTextures::Ptr& textures);

private:
	void		ReplaceAttributeLocations(std::string& shader)noexcept;
	void		ReplaceLightNumbers(std::string& shader, const Parameters::Ptr& parameters)noexcept;
	
	std::string	GetExtensionString()noexcept;
private:
	uint32_t		mId{ 0 };
	HashType		mCacheKey{ 0 };
	uint32_t		mRefCount{ 0 };
	DriverUniforms::Ptr mUniforms = nullptr;
};

class DriverPrograms {
public:
	using Ptr = std::shared_ptr<DriverPrograms>;

	static Ptr Create() {
		return std::make_shared<DriverPrograms>();
	}

	DriverPrograms()noexcept;

	~DriverPrograms()noexcept;

	DriverProgram::Ptr AcquireProgram(const DriverProgram::Parameters::Ptr& parameters, HashType cacheKey)noexcept;

	UniformHandleMap GetUniforms(const Material::Ptr& material)noexcept;

	DriverProgram::Parameters::Ptr GetParameters(
		const Material::Ptr&material,
		const Object3d::Ptr&object,
		const DriverLights::Ptr&lights,
		const DriverShadowMap::Ptr&shadowMap)noexcept;

	HashType GetProgramCacheKey(const DriverProgram::Parameters::Ptr& parameters)noexcept;

	void Release(const DriverProgram::Ptr& program)noexcept;

private:
	std::unordered_map<HashType, DriverProgram::Ptr>mPrograms{};
};