#include"driverUniforms.h"
#include "../../log/debugLog.h"
#include "../../wrapper/glWrapper.hpp"

#define UPLOAD(TYPE,VALUE)\
{\
	TYPE v=std::any_cast<TYPE>(VALUE);\
	Upload(v);\
}

#define UPLOAD_ARRAY(TYPE,VALUE)\
{\
	auto v=std::any_cast<std::vector<TYPE>>(VALUE);\
	Upload(static_cast<TYPE*>(v.data()));\
}

UniformBase::UniformBase()noexcept{}

UniformBase::~UniformBase()noexcept{}

//——————————singleUniforms————————————

SingleUniform::SingleUniform(const std::string& id, const GLint& location, const GLenum& type)noexcept:UniformBase() {
	mId = id;
	mLocation = location;
	mType = type;
}

SingleUniform::~SingleUniform()noexcept {}

void SingleUniform::SetValue(
	const std::any& value,
	const DriverTextures::Ptr& textures,
	const std::shared_ptr<DriverUniforms>& driverUniforms){

	switch (mType) {
	case GL_FLOAT:
		UPLOAD(float, value)
			break;
	case GL_FLOAT_VEC2:
		UPLOAD(glm::vec2, value)
			break;
	case GL_FLOAT_VEC3:
		UPLOAD(glm::vec3, value)
			break;
	case GL_FLOAT_VEC4:
		UPLOAD(glm::vec4, value)
			break;
	case GL_INT:
		UPLOAD(int, value)
			break;
	case GL_INT_VEC2:
		UPLOAD(glm::ivec2, value)
			break;
	case GL_INT_VEC3:
		UPLOAD(glm::ivec3, value)
			break;
	case GL_INT_VEC4:
		UPLOAD(glm::ivec4, value)
			break;
	case GL_BOOL:
		UPLOAD(bool, value)
			break;
	case GL_BOOL_VEC2:
		UPLOAD(glm::bvec2, value)
			break;
	case GL_BOOL_VEC3:
		UPLOAD(glm::bvec3, value)
			break;
	case GL_BOOL_VEC4:
		UPLOAD(glm::bvec4, value)
			break;
	case GL_FLOAT_MAT2:
		UPLOAD(glm::mat2, value)
			break;
	case GL_FLOAT_MAT3:
		UPLOAD(glm::mat3, value)
			break;
	case GL_FLOAT_MAT4:
		UPLOAD(glm::mat4, value)
			break;
	case GL_SAMPLER_2D:
		UploadTexture(driverUniforms, textures, value);
		break;
	case GL_SAMPLER_CUBE:
		UploadTexture(driverUniforms, textures, value);
		break;
	default:
		break;
	}
}

void SingleUniform::UploadTexture(
	const std::shared_ptr<DriverUniforms>& driverUniforms,
	const DriverTextures::Ptr& textures,
	const std::any& value) {
	Texture::Ptr texture = nullptr;
	if (value.type() == typeid(Texture::Ptr)) {
		texture = std::any_cast<Texture::Ptr>(value);
	}
	else if (value.type() == typeid(CubeTexture::Ptr)) {
		texture = std::any_cast<CubeTexture::Ptr>(value);
	}

	auto textureSlot = driverUniforms->GetTextureSlot(mLocation);

	if (textureSlot < 0) {
		textureSlot = driverUniforms->AllocateTextureUnits(1)[0];

		driverUniforms->SetTextureSlot(mLocation, textureSlot);
	}

	textures->BindTexture(texture, textureSlot);

	GLint textureIndex = textureSlot - GL_TEXTURE0;
	gl::uniform1i(mLocation, textureIndex);
}

//————————————PureArrayUniform————————————

PureArrayUniform::PureArrayUniform(const std::string& id, const GLint& location, const GLenum& type, GLint size) noexcept:UniformBase() {
	mId = id;
	mLocation = location;
	mType = type;
	mSize = size;
}

PureArrayUniform::~PureArrayUniform() noexcept {}


void PureArrayUniform::SetValue(
	const std::any& value,
	const DriverTextures::Ptr& textures,
	const std::shared_ptr<DriverUniforms>& driverUniforms){
	switch (mType) {
	case GL_FLOAT:
		UPLOAD_ARRAY(float, value)
			break;
	case GL_FLOAT_VEC2:
		UPLOAD_ARRAY(glm::vec2, value)
			break;
	case GL_FLOAT_VEC3:
		UPLOAD_ARRAY(glm::vec3, value)
			break;
	case GL_FLOAT_VEC4:
		UPLOAD_ARRAY(glm::vec4, value)
			break;
	case GL_INT:
		UPLOAD_ARRAY(int, value)
			break;
	case GL_INT_VEC2:
		UPLOAD_ARRAY(glm::ivec2, value)
			break;
	case GL_INT_VEC3:
		UPLOAD_ARRAY(glm::ivec3, value)
			break;
	case GL_INT_VEC4:
		UPLOAD_ARRAY(glm::ivec4, value)
			break;
	case GL_BOOL:
		UPLOAD_ARRAY(int, value)
			break;
	case GL_BOOL_VEC2:
		UPLOAD_ARRAY(glm::ivec2, value)
			break;
	case GL_BOOL_VEC3:
		UPLOAD_ARRAY(glm::ivec3, value)
			break;
	case GL_BOOL_VEC4:
		UPLOAD_ARRAY(glm::ivec4, value)
			break;
	case GL_FLOAT_MAT2:
		UPLOAD_ARRAY(glm::mat2, value)
			break;
	case GL_FLOAT_MAT3:
		UPLOAD_ARRAY(glm::mat3, value)
			break;
	case GL_FLOAT_MAT4:
		UPLOAD_ARRAY(glm::mat4, value)
			break;
	case GL_SAMPLER_2D:
		UploadTexture2DArray(driverUniforms, textures, value);
		break;
	default:
		break;
	}
}

void PureArrayUniform::UploadTexture2DArray(
	const std::shared_ptr<DriverUniforms>& driverUniforms,
	const DriverTextures::Ptr& textures,
	const std::any& value){

	auto textureArray =std::any_cast<std::vector<Texture::Ptr>>(value);

	std::vector<GLint> textureSlots = driverUniforms->GetTextureArraySlot(mLocation);

	if (textureSlots.size() == 0) {

		textureSlots = driverUniforms->AllocateTextureUnits(mSize);
		driverUniforms->SetTextureArraySlot(mLocation, textureSlots);
	}

	for (uint32_t i = 0; i < textureArray.size(); ++i) {
		textures->BindTexture(textureArray[i], textureSlots[i]);
	}

	std::vector<GLint> textureIndices;
	for (int i = 0; i < textureSlots.size(); ++i) {
		textureIndices.push_back(textureSlots[i] - GL_TEXTURE0);
	}

	gl::uniform1iv(mLocation, textureArray.size(), textureIndices.data());
}

//————————————StructuredUniform————————————

StructuredUniform::StructuredUniform(const std::string& id) noexcept: UniformBase(), UniformContainer() {
	mId = id;
}

StructuredUniform::~StructuredUniform() noexcept {}

void StructuredUniform::SetValue(
	const std::any& value,
	const DriverTextures::Ptr& textures,
	const std::shared_ptr<DriverUniforms>& driverUniforms) {

	auto v = std::any_cast<UniformUnitMap>(value);

	for (const auto& iter : mUniformMap) {
		auto name = iter.first;
		auto uniform = iter.second;

		uniform->SetValue(v[name], textures, driverUniforms);
	}
}

//————————————DriverUniforms————————————

DriverUniforms::DriverUniforms(const GLint& program) noexcept {

}

DriverUniforms::~DriverUniforms() noexcept {

}

void DriverUniforms::Upload(UniformHandleMap& uniformHandleMap, const DriverTextures::Ptr& textures) {

}

void DriverUniforms::AddUniform(UniformContainer* container, const UniformBase::Ptr& uniformObject) {

}

void DriverUniforms::SetTextureSlot(const GLint& location, GLuint slot) noexcept {

}

GLint DriverUniforms::GetTextureSlot(const GLint& location) noexcept {

}

void DriverUniforms::SetTextureArraySlot(const GLint& location, std::vector<GLint> slot) noexcept {

}

std::vector<GLint> DriverUniforms::GetTextureArraySlot(const GLint& location) noexcept {

}

std::vector<GLint> DriverUniforms::AllocateTextureUnits(const int& n) {

}