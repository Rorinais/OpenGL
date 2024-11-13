#pragma once
#include"loader.h"
#include"../texture/texture.h"

class TextureLoader:public Loader
{
public:
	TextureLoader()noexcept;

	~TextureLoader()noexcept;

	static Texture::Ptr loader(const std::string&path,unsigned char*dataIn=nullptr,uint32_t widthIn=0,uint32_t heithIn=0);
};