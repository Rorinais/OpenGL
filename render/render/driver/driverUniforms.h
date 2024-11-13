#pragma once
#include "../../global/base.h"
#include "../../global/constant.h"
#include "driverTextures.h"
#include "../shaders/uniformsLib.h"

class DriverUniforms;

class UniformBase {
public:
	using Ptr = std::shared_ptr<UniformBase>;

	UniformBase()noexcept;

	~UniformBase()noexcept;

	std::string mId;

	virtual void SetValue(
		const std::any&value,
		const DriverTextures::Ptr&textures,
		const std::shared_ptr<DriverUniforms>&driverUniforms){}
};

class SingleUniform :public UniformBase {
public:
	using Ptr = std::shared_ptr<SingleUniform>;

	static Ptr Create(const std::string& id, const GLint& location, const GLenum& type) {
		return std::make_shared<SingleUniform>(id, location, type);
	}

	SingleUniform(const std::string& id, const GLint& location, const GLenum& type)noexcept;

	~SingleUniform()noexcept;

public:
	GLint		mLocation{ 0 };
	GLenum		mType;

public:
	void SetValue(
		const std::any&value,
		const DriverTextures::Ptr&textures,
		const std::shared_ptr<DriverUniforms>&driverUniforms)override;

	void UploadTexture(
		const std::shared_ptr<DriverUniforms>& driverUniforms,
		const DriverTextures::Ptr& textures,
		const std::any& value);


	template<typename T>
	void Upload(const T& value){}

	template<>
	void Upload<float>(const float& value);

	template<>
	void Upload<glm::vec2>(const glm::vec2& value);

	template<>
	void Upload<glm::vec3>(const glm::vec3& value);

	template<>
	void Upload<glm::vec4>(const glm::vec4& value);

	template<>
	void Upload<int>(const int& value);

	template<>
	void Upload<glm::ivec2>(const glm::ivec2& value);

	template<>
	void Upload<glm::ivec3>(const glm::ivec3& value);

	template<>
	void Upload<glm::ivec4>(const glm::ivec4& value);

	template<>
	void Upload<bool>(const bool& value);

	template<>
	void Upload<glm::bvec2>(const glm::bvec2& value);

	template<>
	void Upload<glm::bvec3>(const glm::bvec3& value);

	template<>
	void Upload<glm::bvec4>(const glm::bvec4& value);

	template<>
	void Upload<glm::mat2>(const glm::mat2& value);

	template<>
	void Upload<glm::mat3>(const glm::mat3& value);

	template<>
	void Upload<glm::mat4>(const glm::mat4& value);
};

class PureArrayUniform : public UniformBase {
public:
	using Ptr = std::shared_ptr<PureArrayUniform>;
	static Ptr Create(const std::string& id, const GLint& location, const GLenum& type, GLint size) {
		return std::make_shared<PureArrayUniform>(id, location, type, size);
	}

	PureArrayUniform(const std::string& id, const GLint& location, const GLenum& type, GLint size) noexcept;

	~PureArrayUniform() noexcept;

	GLint		mLocation{ 0 };
	GLenum		mType;
	GLint		mSize{ 0 };

public:
	void SetValue(
		const std::any& value,
		const DriverTextures::Ptr& textures,
		const std::shared_ptr<DriverUniforms>& driverUniforms) override;

	void UploadTexture2DArray(
		const std::shared_ptr<DriverUniforms>& driverUniforms,
		const DriverTextures::Ptr& textures,
		const std::any& value);

	template<typename T>
	void Upload(const T* value) {}

	template<>
	void Upload<float>(const float* value);

	template<>
	void Upload<glm::vec2>(const glm::vec2* value);

	template<>
	void Upload<glm::vec3>(const glm::vec3* value);

	template<>
	void Upload<glm::vec4>(const glm::vec4* value);

	template<>
	void Upload<int>(const int* value);

	template<>
	void Upload<glm::ivec2>(const glm::ivec2* value);

	template<>
	void Upload<glm::ivec3>(const glm::ivec3* value);

	template<>
	void Upload<glm::ivec4>(const glm::ivec4* value);

	template<>
	void Upload<glm::mat2>(const glm::mat2* value);

	template<>
	void Upload<glm::mat3>(const glm::mat3* value);

	template<>
	void Upload<glm::mat4>(const glm::mat4* value);

};

class UniformContainer {
public:
	using Ptr = std::shared_ptr<UniformContainer>;
	std::unordered_map<std::string, UniformBase::Ptr> mUniformMap;

	virtual void Fake() {}
};

class StructuredUniform :public UniformBase, public UniformContainer {
public:
	using Ptr = std::shared_ptr<StructuredUniform>;
	static Ptr Create(const std::string& id) {
		return std::make_shared<StructuredUniform>(id);
	}

	StructuredUniform(const std::string& id) noexcept;

	~StructuredUniform() noexcept;

	void SetValue(
		const std::any& value,
		const DriverTextures::Ptr& textures,
		const std::shared_ptr<DriverUniforms>& driverUniforms) override;
};

class DriverUniforms :public UniformContainer, public std::enable_shared_from_this<DriverUniforms> {
public:
	using Ptr = std::shared_ptr<DriverUniforms>;
	static Ptr Create(const GLint& program) { return std::make_shared<DriverUniforms>(program); }

	DriverUniforms(const GLint& program) noexcept;

	~DriverUniforms() noexcept;

	void Upload(UniformHandleMap& uniformHandleMap, const DriverTextures::Ptr& textures);

	void AddUniform(UniformContainer* container, const UniformBase::Ptr& uniformObject);

	void SetTextureSlot(const GLint& location, GLuint slot) noexcept;

	GLint GetTextureSlot(const GLint& location) noexcept;

	void SetTextureArraySlot(const GLint& location, std::vector<GLint> slot) noexcept;

	std::vector<GLint> GetTextureArraySlot(const GLint& location) noexcept;

	std::vector<GLint> AllocateTextureUnits(const int& n);

private:
	std::unordered_map<GLint, GLuint> mTextureSlots{};

	std::unordered_map<GLint, std::vector<GLint>> mTextureArraySlots{};

	GLint	mCurrentTextureSlots{ 0 };
};

template<>
void SingleUniform::Upload<float>(const float& value) {
	glUniform1f(mLocation, value);
}

template<>
void SingleUniform::Upload<glm::vec2>(const glm::vec2& value) {
	glUniform2fv(mLocation, 1, glm::value_ptr(value));
}

template<>
void SingleUniform::Upload<glm::vec3>(const glm::vec3& value) {
	glUniform3fv(mLocation, 1, glm::value_ptr(value));
}

template<>
void SingleUniform::Upload<glm::vec4>(const glm::vec4& value) {
	glUniform4fv(mLocation, 1, glm::value_ptr(value));
}

template<>
void SingleUniform::Upload<int>(const int& value) {
	glUniform1i(mLocation, value);
}

template<>
void SingleUniform::Upload<glm::ivec2>(const glm::ivec2& value) {
	glUniform2i(mLocation, value.x, value.y);
}

template<>
void SingleUniform::Upload<glm::ivec3>(const glm::ivec3& value) {
	glUniform3i(mLocation, value.x, value.y, value.z);
}

template<>
void SingleUniform::Upload<glm::ivec4>(const glm::ivec4& value) {
	glUniform4i(mLocation, value.x, value.y, value.z, value.w);
}

template<>
void SingleUniform::Upload<bool>(const bool& value) {
	int v = value;
	glUniform1i(mLocation, v);
}

template<>
void SingleUniform::Upload<glm::bvec2>(const glm::bvec2& value) {
	glm::ivec2 v = value;
	glUniform2i(mLocation, v.x, v.y);
}

template<>
void SingleUniform::Upload<glm::bvec3>(const glm::bvec3& value) {
	glm::ivec3 v = value;
	glUniform3i(mLocation, v.x, v.y, v.z);
}

template<>
void SingleUniform::Upload<glm::bvec4>(const glm::bvec4& value) {
	glm::ivec4 v = value;
	glUniform4i(mLocation, v.x, v.y, v.z, v.w);
}

template<>
void SingleUniform::Upload<glm::mat2>(const glm::mat2& value) {
	glUniformMatrix2fv(mLocation, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void SingleUniform::Upload<glm::mat3>(const glm::mat3& value) {
	glUniformMatrix3fv(mLocation, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void SingleUniform::Upload<glm::mat4>(const glm::mat4& value) {
	glUniformMatrix4fv(mLocation, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void PureArrayUniform::Upload<float>(const float* value) {
	glUniform1fv(mLocation, mSize, value);
}

template<>
void PureArrayUniform::Upload<glm::vec2>(const glm::vec2* value) {
	glUniform2fv(mLocation, mSize, glm::value_ptr(value[0]));
}

template<>
void PureArrayUniform::Upload<glm::vec3>(const glm::vec3* value) {
	glUniform3fv(mLocation, mSize, glm::value_ptr(value[0]));
}

template<>
void PureArrayUniform::Upload<glm::vec4>(const glm::vec4* value) {
	glUniform4fv(mLocation, mSize, glm::value_ptr(value[0]));
}

template<>
void PureArrayUniform::Upload<int>(const int* value) {
	glUniform1iv(mLocation, mSize, value);
}

template<>
void PureArrayUniform::Upload<glm::ivec2>(const glm::ivec2* value) {
	glUniform2iv(mLocation, mSize, glm::value_ptr(value[0]));
}

template<>
void PureArrayUniform::Upload<glm::ivec3>(const glm::ivec3* value) {
	glUniform3iv(mLocation, mSize, glm::value_ptr(value[0]));
}

template<>
void PureArrayUniform::Upload<glm::ivec4>(const glm::ivec4* value) {
	glUniform4iv(mLocation, mSize, glm::value_ptr(value[0]));
}

template<>
void PureArrayUniform::Upload<glm::mat2>(const glm::mat2* value) {
	glUniformMatrix2fv(mLocation, mSize, GL_FALSE, glm::value_ptr(value[0]));
}

template<>
void PureArrayUniform::Upload<glm::mat3>(const glm::mat3* value) {
	glUniformMatrix3fv(mLocation, mSize, GL_FALSE, glm::value_ptr(value[0]));
}

template<>
void PureArrayUniform::Upload<glm::mat4>(const glm::mat4* value) {
	glUniformMatrix4fv(mLocation, mSize, GL_FALSE, glm::value_ptr(value[0]));
}
