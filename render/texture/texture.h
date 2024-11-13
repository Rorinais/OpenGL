#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#ifndef TEXTURE_H
#define TEXTURE_H

#include "../global/base.h"
#include"../global/constant.h"
#include"source.h"

class Texture 
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName);
	bool load();

	void Bind(GLenum TextureUnit);
private:
	std::string m_filename;
	GLenum m_textureTarget;
	GLuint m_textureObj;

public:
	using Ptr = std::shared_ptr<Texture>;
	static Ptr Create(
		const uint32_t &width,
		const uint32_t &height,
		const DataType&dataType=DataType::UnsignedByteType,
		const TextureWrapping&wrapS=TextureWrapping::RepeatWrapping,
		const TextureWrapping& wrapT = TextureWrapping::RepeatWrapping,
		const TextureWrapping& wrapR = TextureWrapping::RepeatWrapping,
		const TextureFilter&magFilter=TextureFilter::LinearFilter,
		const TextureFilter& minFilter = TextureFilter::NearestFilter, 
		const TextureFormat&format=TextureFormat::RGBA){
		return std::make_shared<Texture>(width,height,dataType,wrapS,wrapT,wrapR,magFilter,minFilter,format);}

	Texture(
		const uint32_t& width,
		const uint32_t& height,
		const DataType& dataType = DataType::UnsignedByteType,
		const TextureWrapping& wrapS = TextureWrapping::RepeatWrapping,
		const TextureWrapping& wrapT = TextureWrapping::RepeatWrapping,
		const TextureWrapping& wrapR = TextureWrapping::RepeatWrapping,
		const TextureFilter& magFilter = TextureFilter::LinearFilter,
		const TextureFilter& minFilter = TextureFilter::NearestFilter,
		const TextureFormat& format = TextureFormat::RGBA	)noexcept;
	~Texture()noexcept;

	ID GetId()const noexcept { return mId; }

	TextureUsage GetUsage()const noexcept { return mUsage; }

	virtual Texture::Ptr clone()noexcept;

public:
	TextureFilter	mMinFilter{ TextureFilter::LinearFilter };
	TextureFilter	mMagFilter{ TextureFilter::LinearFilter };
	TextureWrapping	mWrapS{ TextureWrapping::RepeatWrapping };
	TextureWrapping	mWrapT{ TextureWrapping::RepeatWrapping };
	TextureWrapping	mWrapR{ TextureWrapping::RepeatWrapping };

	TextureFormat	mFormat{ TextureFormat::RGBA };
	TextureFormat	mInternalFormat{ TextureFormat::RGBA };
	DataType		mDataType{ DataType::UnsignedByteType };

	uint32_t		mWidth{ 0 };
	uint32_t		mHeight{ 0 };

	Source::Ptr		mSource{ nullptr };

	bool			mNeedsUpdate{ true };

	TextureType		mTextureType{ TextureType::Texture2D };
	TextureUsage	mUsage{ TextureUsage::SamplerTexture };

protected:
	ID mId{0};
};


#endif // !TEXTURE
