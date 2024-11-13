#pragma once 
#include "../../global/base.h"
#include "../../global/constant.h"
#include "../../textures/texture.h"
#include "../../textures/cubeTexture.h"

class UniformHandle {
public:
	UniformHandle() noexcept {}

	~UniformHandle() noexcept {}

	std::any	mValue;
	bool		mNeedsUpdate{ false };
};

using UniformUnitMap = std::unordered_map<std::string, std::any>;

using UniformHandleMap = std::unordered_map<std::string, UniformHandle>;

static const std::unordered_map<std::string, UniformHandleMap> UniformsLib =
{
	{
		"common", {
			{"diffuseMap", UniformHandle()},
			{"opacity", UniformHandle()}
		}
	},
	{
		"envMap", {
			{"envMap", UniformHandle()}
		}
	},
	{
		"specularMap", {
			{"specularMap", UniformHandle()}
		}
	},
	{
		"normalMap", {
			{"normalMap", UniformHandle()}
		}
	}

};

static UniformHandleMap merge(const std::vector<UniformHandleMap>& uniformGroups) {
	UniformHandleMap mergedGroup;

	for (const auto& group : uniformGroups) {
		for (const auto& uniformIter : group) {
			auto name = uniformIter.first;
			auto handle = uniformIter.second;

			mergedGroup.insert(std::make_pair(name, handle));
		}
	}

	return std::move(mergedGroup);
}

static UniformUnitMap* setArrayStructuredUniform(const uint32_t& index, UniformUnitMap* array) noexcept {
	auto indexString = std::to_string(index);
	(*array)[indexString] = UniformUnitMap();


	return std::any_cast<UniformUnitMap>(&(*array)[indexString]);
}

static UniformUnitMap* getArrayStructuredUniform(const uint32_t& index, UniformUnitMap* array) noexcept {
	auto indexString = std::to_string(index);
	return std::any_cast<UniformUnitMap>(&(*array)[indexString]);
}


template<typename T>
static void clearPureArrayUniform(std::vector<T>* array) {
	array->clear();
}

template<typename T>
static void pushPureArrayUniform(T value, std::vector<T>* array) {
	array->push_back(value);
}

