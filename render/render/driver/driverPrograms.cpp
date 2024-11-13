#include"driverPrograms.h"

DriverProgram::DriverProgram(const Parameters::Ptr& parameters)noexcept
{
	mId = Identity::generateID();
	std::string versionString = "#version 330 core\n";
	std::string extensionString = GetExtensionString();

	std::string prefixVertex;
	std::string prefixFragment;

	prefixVertex.append(parameters->mHasNormal ? "#define HAS_NORMAL\n" : "");
	prefixVertex.append(parameters->mHasUV ? "#define HAS_UV\n" : "");
	prefixVertex.append(parameters->mHasColor ? "#define HAS_COLOR\n" : "");

	prefixVertex.append(parameters->mShadowMapEnabled ? "#define USE_SHADOWMAP\n" : "");
	prefixVertex.append(parameters->mSkinning ? "#define USE_SKINNING\n" : "");
	prefixVertex.append(parameters->mSkinning ? std::string("#define MAX_BONES ") + std::to_string(parameters->mMaxBones) + "\n" : "");
	prefixVertex.append(parameters->mUseNormalMap ? "#define USE_NORMALMAP\n" : "");
	prefixVertex.append(parameters->mUseTangent ? "#define USE_TANGENT\n" : "");

	prefixFragment.append(parameters->mHasNormal ? "#define HAS_NORMAL\n" : "");
	prefixFragment.append(parameters->mHasUV ? "#define HAS_UV\n" : "");
	prefixFragment.append(parameters->mHasColor ? "#define HAS_COLOR\n" : "");
	prefixFragment.append(parameters->mHasDiffuseMap ? "#define HAS_DIFFUSE_MAP\n" : "");
	prefixFragment.append(parameters->mHasEnvCubeMap ? "#define USE_ENVMAP\n" : "");
	prefixFragment.append(parameters->mHasSpecularMap ? "#define USE_SPECULARMAP\n" : "");

	prefixFragment.append(parameters->mShadowMapEnabled ? "#define USE_SHADOWMAP\n" : "");
	prefixFragment.append(parameters->mDepthPacking == DepthMaterial::RGBADepthPacking ? "#define DEPTH_PACKING_RGBA\n" : "");
	prefixFragment.append(parameters->mUseNormalMap ? "#define USE_NORMALMAP\n" : "");
	prefixFragment.append(parameters->mUseTangent ? "#define USE_TANGENT\n" : "");

	auto vertexString = parameters->mVertex;
	auto fragmentString = parameters->mFragment;

	ReplaceAttributeLocations(vertexString);
	ReplaceAttributeLocations(fragmentString);

	ReplaceLightNumbers(versionString, parameters);
	ReplaceLightNumbers(fragmentString, parameters);

	vertexString = versionString + extensionString + prefixVertex + vertexString;
	fragmentString = versionString + extensionString + prefixFragment + fragmentString;

	auto vertex = vertexString.c_str();
	auto frament = fragmentString.c_str();

	uint32_t vertexId = 0, fragId = 0;
	char infoLog[512];
	int successFlag = 0;

	vertexId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexId, 1, &vertex, NULL);
	glCompileShader(vertexId);

	glGetShaderiv(vertexId, GL_COMPILE_STATUS, &successFlag);
	if (!successFlag)
	{
		glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	fragId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragId, 1, &frament, NULL);
	glCompileShader(fragId);

	glGetShaderiv(fragId, GL_COMPILE_STATUS, &successFlag);
	if (!successFlag)
	{
		glGetShaderInfoLog(fragId, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	mProgram = glCreateProgram();
	glAttachShader(mProgram, vertexId);
	glAttachShader(mProgram, fragId);
	glLinkProgram(mProgram);

	glGetShaderiv(mProgram, GL_COMPILE_STATUS, &successFlag);
	if (!successFlag)
	{
		glGetShaderInfoLog(mProgram, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}
	glDeleteShader(vertexId);
	glDeleteShader(fragId);

	DebugLog::GetInstance()->BeginPrintUniformInfo(parameters->mShaderId);
	mUniforms = DriverUniforms::Create(mProgram);
	DebugLog::GetInstance()->End();
}

DriverProgram::~DriverProgram()noexcept {
	glDeleteProgram(mProgram);
}

void DriverProgram::ReplaceAttributeLocations(std::string& shader)noexcept {
	std::unordered_map<std::string, std::string> replaceMap = {
			{"POSITION_LOCATION", std::to_string(LOCATION_MAP.at("position"))},
			{"NORMAL_LOCATION", std::to_string(LOCATION_MAP.at("normal"))},
			{"UV_LOCATION", std::to_string(LOCATION_MAP.at("uv"))},
			{"COLOR_LOCATION", std::to_string(LOCATION_MAP.at("color"))},
			{"SKINNING_INDICES_LOCATION", std::to_string(LOCATION_MAP.at("skinIndex"))},
			{"SKINNING_WEIGHTS_LOCATION", std::to_string(LOCATION_MAP.at("skinWeight"))},
			{"TANGENT_LOCATION", std::to_string(LOCATION_MAP.at("tangent"))},
			{"BITANGENT_B_LOCATION", std::to_string(LOCATION_MAP.at("bitangent"))},
	};

	for (const auto& iter : replaceMap) {
		std::regex pattern(iter.first);

		shader = std::regex_replace(shader, pattern, iter.second);
	}
}

void DriverProgram::ReplaceLightNumbers(std::string& shader, const Parameters::Ptr& parameters)noexcept {
	std::unordered_map<std::string, std::string> replaceMap = {
	{"NUM_DIR_LIGHTS", std::to_string(parameters->mDirectionalLightCount)},
	{"NUM_DIR_LIGHT_SHADOWS", std::to_string(parameters->mNumDirectionalLightShadows)},
	};

	for (const auto& iter : replaceMap) {
		std::regex pattern(iter.first);
		shader = std::regex_replace(shader, pattern, iter.second);
	}
}

std::string	DriverProgram::GetExtensionString()noexcept {
	std::string extensionString = "";
	extensionString.append("#extension GL_ARB_separate_shader_objects : enable\n");

	return extensionString;
}

void DriverProgram::UploadUniforms(UniformHandleMap& uniformMap, const DriverTextures::Ptr& textures) {
	mUniforms->Upload(uniformMap, textures);
}

//...................................DriverPrograms...................................

DriverPrograms::DriverPrograms()noexcept {}

DriverPrograms::~DriverPrograms()noexcept {}

DriverProgram::Ptr DriverPrograms::AcquireProgram(const DriverProgram::Parameters::Ptr& parameters, HashType cacheKey)noexcept {
	auto iter = mPrograms.find(cacheKey);

	if (iter!=mPrograms.end())
	{
		return iter->second;
	}

	auto program = DriverProgram::Create(parameters);
	program->mCacheKey = cacheKey;
	mPrograms.insert(std::make_pair(cacheKey, program));
	program->mRefCount++;

	return program;
}

UniformHandleMap DriverPrograms::GetUniforms(const Material::Ptr& material)noexcept {
	UniformHandleMap uniforms{};

	auto shaderID = material->GetType();
	auto shaderIter = ShaderLib.find(shaderID);

	if (shaderIter != ShaderLib.end()) {
		uniforms = shaderIter->second.mUniformMap;
	}

	return uniforms;
}

DriverProgram::Parameters::Ptr DriverPrograms::GetParameters(
	const Material::Ptr& material,
	const Object3d::Ptr& object,
	const DriverLights::Ptr& lights,
	const DriverShadowMap::Ptr& shadowMap
)noexcept {
	auto renderObject = std::static_pointer_cast<RenderableObject>(object);
	auto geometry = renderObject->GetGeometry();
	auto parameters = DriverProgram::Parameters::Create();

	auto shaderId = material->GetType();
	auto shaderIter = ShaderLib.find(shaderId);

	if (shaderIter==ShaderLib.end())
	{
		return nullptr;
	}

	parameters->mShaderId = material->GetType();
	parameters->mVertex = shaderIter->second.mVertex;
	parameters->mFragment = shaderIter->second.mFragment;

	parameters->mShadowMapEnabled = shadowMap->mEnabled;
	parameters->mDirectionalLightCount = lights->mState.mDirectionalCount;
	parameters->mNumDirectionalLightShadows = lights->mState.mNumDrirectionalShadows;

	if (geometry->hasAttribute("normal")) {
		parameters->mHasNormal = true;
	}

	if (geometry->hasAttribute("uv")) {
		parameters->mHasUV = true;
	}

	if (geometry->hasAttribute("color")) {
		parameters->mHasColor = true;
	}

	if (material->mDiffuseMap != nullptr) {
		parameters->mHasDiffuseMap = true;
	}

	if (material->mEnvMap != nullptr) {
		parameters->mHasEnvCubeMap = true;
	}

	if (material->mSpecularMap != nullptr) {
		parameters->mHasSpecularMap = true;
	}

	if (material->mNormalMap != nullptr) {
		parameters->mUseNormalMap = true;
	}

	if (object->mIsRenderableObject) {
		auto renderableObject = std::static_pointer_cast<RenderableObject>(object);
		auto geometry = renderableObject->GetGeometry();
		parameters->mUseTangent = geometry->hasAttribute("tangent");
	}

	if (material->mIsDepthMaterial) {
		auto depthMaterial = std::static_pointer_cast<DepthMaterial>(material);
		parameters->mDepthPacking = depthMaterial->mPacking;
	}

	if (object->mIsSkinnedMesh) {
		auto skinnedMesh = std::static_pointer_cast<SkinnedMesh>(object);
		parameters->mSkinning = true;
		parameters->mMaxBones = skinnedMesh->mSkeleton->mBones.size();
	}

	return parameters;
}

HashType DriverPrograms::GetProgramCacheKey(const DriverProgram::Parameters::Ptr& parameters)noexcept {
	std::hash<std::string>hasher;

	std::string keyString;

	keyString.append(parameters->mShaderId);
	keyString.append(parameters->mVertex);
	keyString.append(parameters->mFragment);
	keyString.append(std::to_string(parameters->mHasNormal));
	keyString.append(std::to_string(parameters->mHasUV));
	keyString.append(std::to_string(parameters->mHasColor));
	keyString.append(std::to_string(parameters->mHasDiffuseMap));
	keyString.append(std::to_string(parameters->mHasEnvCubeMap));
	keyString.append(std::to_string(parameters->mHasSpecularMap));
	keyString.append(std::to_string(parameters->mDirectionalLightCount));
	keyString.append(std::to_string(parameters->mNumDirectionalLightShadows));
	keyString.append(std::to_string(parameters->mSkinning));
	keyString.append(std::to_string(parameters->mMaxBones));
	keyString.append(std::to_string(parameters->mUseNormalMap));
	keyString.append(std::to_string(parameters->mUseTangent));
	keyString.append(std::to_string(parameters->mDepthPacking));

	return hasher(keyString);
}

void DriverPrograms::Release(const DriverProgram::Ptr& program)noexcept {
	if (--program->mRefCount == 0) {
		mPrograms.erase(program->mCacheKey);
	}
}
