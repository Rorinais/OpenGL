#pragma once
#include"texture.h"
#include"source.h"

class CubeTexture:public Texture {
public:
	using Ptr = std::shared_ptr<CubeTexture>;
	static Ptr Create() {
		return std::make_shared<CubeTexture>();
	}

	CubeTexture()noexcept;
	~CubeTexture()noexcept;

};