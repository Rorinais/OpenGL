#include"../../global/base.h"
#include"../../texture/texture.h"
#include"../../texture/cubeTexture.h"
class DriverTextures {
public:
	using Ptr = std::shared_ptr<DriverTextures>;
	static Ptr Create() {
		return std::make_shared<DriverTextures>();
	}

	DriverTextures()noexcept;

	~DriverTextures()noexcept;

	void BindTexture(const Texture::Ptr& texture, GLenum textureUnit);
};