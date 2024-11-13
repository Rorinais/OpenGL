#include "texture.h"
#include"../tools/identity.h"
#include"../global/base.h"
#include"../global/eventDispatcher.h"


Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
	m_textureTarget = TextureTarget;
	m_filename = FileName;

}

bool Texture::load() 
{
	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj); 
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(m_filename.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		glTexImage2D(m_textureTarget, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(m_textureTarget);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return true;
}

void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
	
}

Texture::Texture(
	const uint32_t& width,
	const uint32_t& height,
	const DataType& dataType ,
	const TextureWrapping& wrapS ,
	const TextureWrapping& wrapT ,
	const TextureWrapping& wrapR ,
	const TextureFilter& magFilter ,
	const TextureFilter& minFilter ,
	const TextureFormat& format )noexcept {
	mId = Identity::generateID();
	mWidth = width;
	mHeight = height;
	mDataType = dataType;
	mWrapS = wrapS;
	mWrapT = wrapT;
	mWrapR = wrapR;
	mMagFilter = magFilter;
	mMinFilter = minFilter;
	mFormat = format;
	mTextureType = TextureType::Texture2D;
}
Texture::~Texture()noexcept {
	EventBase::Ptr e = EventBase::create("textureDispose");
	e->mTarget = this;
	EventDispatcher::getInstance()->dispatchEvent(e);

	if (mSource) {
		EventBase::Ptr e = EventBase::create("sourceRelease");
		e->mTarget = mSource.get();
		EventDispatcher::getInstance()->dispatchEvent(e);
	}
}

Texture::Ptr Texture::clone() noexcept {
	auto texture = Texture::Create(mWidth, mHeight, mDataType, mWrapS, mWrapT, mWrapR, mMagFilter, mMinFilter, mFormat);
	texture->mSource = mSource;
	texture->mUsage = mUsage;
	texture->mTextureType = mTextureType;
	texture->mInternalFormat = mInternalFormat;

	return texture;
}