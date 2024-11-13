#pragma once
#include"../global/base.h"
#include"../texture/texture.h"
#include"../texture/cubeTexture.h"
#include"../global/constant.h"

class MaterialTypeChecker
{
public:
	bool mIsMaterial = false;
	bool mIsMeshBasicMaterial = false;
	bool mIsMeshPhongMaterial = false;
	bool mIslineBasicMaterial = false;
	bool mIsCubeMaterial = false;
	bool mIsDepthMaterial = false;
};

class Material :public MaterialTypeChecker
{
public:
	using Ptr = std::shared_ptr<Material>;
	static Ptr Create()
	{
		return std::make_shared<Material>();
	}
	Material()noexcept;

	~Material()noexcept;

public:
	std::string GetType()const noexcept { return mType; }
	ID GetId()const noexcept { return mId; }
	bool mNeedsUpdate{ true };
	
	uint32_t mVersion{ 1 };

	FrontFace mFrontFace{ FrontFace::FrontCounterClockWise };
	Side	mSide{ Side::DoubleSide };
	DrawMode mDrawMode{ DrawMode::Triangles };

	bool	mTransparent{ false };
	float	mOpacity{ 1.0f };
	BlendingType	mBlendingType{ BlendingType::CustomBlending };
	BlendingFactor	mBlendSrc{ BlendingFactor::SrcAlpha };
	BlendingFactor	mBlendDst{ BlendingFactor::OneMinusSrcAlpha };
	BlendingEquation	mBlendEquation{ BlendingEquation::AddEquation };
	BlendingFactor	mBlendSrcAlpha{ BlendingFactor::None };
	BlendingFactor	mBlendDstAlpha{ BlendingFactor::Zero };
	BlendingEquation	mBlendEquation{ BlendingEquation::AddEquation };

	bool	mDepthTest{ true };
	bool	mDepthWrite{ true };
	CompareFunction	DepthFunction{ CompareFunction::LessOrEqual };
	double	mDepthClearColor{ 1.0 };

	Texture::Ptr	mDiffuseMap{ nullptr };
	CubeTexture::Ptr	mEnvMap{ nullptr };
	Texture::Ptr	mNormalMap{ nullptr };
	Texture::Ptr	mSpecularMap{ nullptr };


protected:
	ID mId{ 0 };
	std::string mType;
};